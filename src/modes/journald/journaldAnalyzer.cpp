/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   Copyright (C) 2007 by Nicolas Ternisien                               *
 *   nicolas.ternisien@gmail.com                                           *
 *   Copyright (C) 2015 by Vyacheslav Matyushin                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#include "journaldAnalyzer.h"
#include "journaldConfiguration.h"
#include "ksystemlogConfig.h"
#include "logging.h"
#include "logViewModel.h"
#include "logFile.h"

#include <KLocalizedString>

#include <QtConcurrent/QtConcurrent>

JournaldAnalyzer::JournaldAnalyzer(LogMode *logMode)
    : Analyzer(logMode)
{
    m_cursor = nullptr;
    m_forgetWatchers = true;

    // Initialize journal access flags and open the journal.
    m_journalFlags = 0;
    JournaldConfiguration *configuration = logMode->logModeConfiguration<JournaldConfiguration *>();
    if (configuration->displayCurrentUserProcesses())
        m_journalFlags |= SD_JOURNAL_CURRENT_USER;
    if (configuration->displaySystemServices())
        m_journalFlags |= SD_JOURNAL_SYSTEM;
    sd_journal_open(&m_journal, m_journalFlags);

    int fd = sd_journal_get_fd(m_journal);
    m_journalNotifier = new QSocketNotifier(fd, QSocketNotifier::Read);
    m_journalNotifier->setEnabled(false);
    connect(m_journalNotifier, SIGNAL(activated(int)), this, SLOT(journalDescriptorUpdated(int)));
}

JournaldAnalyzer::~JournaldAnalyzer()
{
    m_journalNotifier->setEnabled(false);
    sd_journal_close(m_journal);
    free(m_cursor);
    delete m_journalNotifier;
}

LogViewColumns JournaldAnalyzer::initColumns()
{
    LogViewColumns columns;
    columns.addColumn(LogViewColumn(i18n("Date"), true, true));
    columns.addColumn(LogViewColumn(i18n("Unit"), true, true));
    columns.addColumn(LogViewColumn(i18n("Message"), true, true));
    return columns;
}

void JournaldAnalyzer::setLogFiles(const QList<LogFile> &logFiles)
{
    Q_UNUSED(logFiles)
    // Do nothing.
}

void JournaldAnalyzer::watchLogFiles(bool enabled)
{
    m_journalNotifier->setEnabled(enabled);

    m_workerMutex.lock();
    m_forgetWatchers = enabled;
    m_workerMutex.unlock();

    if (enabled) {
        JournalWatcher *watcher = new JournalWatcher();
        m_workerMutex.lock();
        m_journalWatchers.append(watcher);
        m_workerMutex.unlock();
        connect(watcher, SIGNAL(finished()), this, SLOT(readJournalInitialFinished()));
        watcher->setFuture(QtConcurrent::run(this, &JournaldAnalyzer::readJournal, QStringList()));
    } else {
        for (JournalWatcher *watcher : m_journalWatchers) {
            watcher->waitForFinished();
        }
        qDeleteAll(m_journalWatchers);
        m_journalWatchers.clear();
    }
}

QStringList JournaldAnalyzer::units() const
{
    return getUniqueFieldValues("_SYSTEMD_UNIT");
}

QStringList JournaldAnalyzer::syslogIdentifiers() const
{
    return getUniqueFieldValues("SYSLOG_IDENTIFIER");
}

void JournaldAnalyzer::readJournalInitialFinished()
{
    readJournalFinished(FullRead);
}

void JournaldAnalyzer::readJournalUpdateFinished()
{
    readJournalFinished(UpdatingRead);
}

void JournaldAnalyzer::readJournalFinished(ReadingMode readingMode)
{
    JournalWatcher *watcher = static_cast<JournalWatcher *>(sender());
    if (!watcher)
        return;

    QList<JournalEntry> entries = watcher->result();

    if (parsingPaused) {
        logDebug() << "Parsing is paused, discarding journald entries.";
    } else if (entries.size() == 0) {
        logDebug() << "Received no entries.";
    } else {
        insertionLocking.lock();
        logViewModel->startingMultipleInsertions();

        if (FullRead == readingMode) {
            emit statusBarChanged(i18n("Reading journald entries..."));
            // Start displaying the loading bar.
            emit readFileStarted(*logMode, LogFile(), 0, 1);
        }

        // Add journald entries to the model.
        int entriesInserted = updateModel(entries, readingMode);

        logViewModel->endingMultipleInsertions(readingMode, entriesInserted);

        if (FullRead == readingMode) {
            emit statusBarChanged(i18n("Journald entries loaded successfully."));

            // Stop displaying the loading bar.
            emit readEnded();
        }

        // Inform LogManager that new lines have been added.
        emit logUpdated(entriesInserted);

        insertionLocking.unlock();
    }

    m_workerMutex.lock();
    if (m_forgetWatchers) {
        m_journalWatchers.removeAll(watcher);
        watcher->deleteLater();
    }
    m_workerMutex.unlock();
}

void JournaldAnalyzer::journalDescriptorUpdated(int fd)
{
    logDebug() << "Journal updated";
    QFile file;
    file.open(fd, QIODevice::ReadOnly);
    file.readAll();
    file.close();

    if (parsingPaused) {
        logDebug() << "Parsing is paused, will not fetch new journald entries.";
        return;
    }

    JournalWatcher *watcher = new JournalWatcher();
    m_workerMutex.lock();
    m_journalWatchers.append(watcher);
    m_workerMutex.unlock();
    connect(watcher, SIGNAL(finished()), this, SLOT(readJournalUpdateFinished()));
    watcher->setFuture(QtConcurrent::run(this, &JournaldAnalyzer::readJournal, QStringList()));
}

QList<JournaldAnalyzer::JournalEntry> JournaldAnalyzer::readJournal(const QStringList &filters)
{
    QMutexLocker mutexLocker(&m_workerMutex);
    QList<JournalEntry> entryList;
    sd_journal *journal;

    int res = sd_journal_open(&journal, m_journalFlags);
    if (res < 0) {
        logWarning() << "Failed to access journald.";
        return QList<JournalEntry>();
    }

    // Set entries filter.
    for (const QString &filter : filters) {
        res = sd_journal_add_match(journal, filter.toLatin1(), 0);
        if (res < 0) {
            logWarning() << "Failed to set journald filter.";
            sd_journal_close(journal);
            return QList<JournalEntry>();
        }
    }

    // Seek to cursor.
    if (m_cursor) {
        res = sd_journal_seek_cursor(journal, m_cursor);
        if (res < 0) {
            logWarning() << "Failed to seek journald cursor:" << res;
            sd_journal_close(journal);
            return QList<JournalEntry>();
        }

        res = sd_journal_next(journal);
        if (res < 0) {
            logWarning() << "Failed to access journald entry after seeking the cursor:" << res;
            sd_journal_close(journal);
            return QList<JournalEntry>();
        }

        res = sd_journal_test_cursor(journal, m_cursor);
        if (res <= 0) {
            logWarning() << "Journald cursor test failed:" << res;
            sd_journal_close(journal);
            return QList<JournalEntry>();
        }
    }

    // Determine number of entries from cursor to the end of the journal.
    int entriesNum = 0;
    forever {
        res = sd_journal_next(journal);
        if (res < 0) {
            logWarning() << "Failed to iterate to next journald entry.";
            break;
        }
        if (res == 0) {
            // Reached the end.
            break;
        }
        entriesNum++;
    }

    if (!entriesNum) {
        sd_journal_close(journal);
        return QList<JournalEntry>();
    }

    // Read number of entries allowed by KSystemLog configuration.
    int maxEntriesNum = KSystemLogConfig::maxLines();
    if (entriesNum > maxEntriesNum) {
        logDebug() << "Journald contains" << entriesNum << "entries, will read last" << maxEntriesNum;
        int skipEntriesNum = entriesNum - maxEntriesNum;
        if (m_cursor) {
            // Jump over skipEntriesNum entries forward starting from cursor.
            sd_journal_seek_cursor(journal, m_cursor);
            res = sd_journal_next_skip(journal, skipEntriesNum);
        } else {
            sd_journal_seek_tail(journal);
            // Jump over skipEntriesNum entries backwards from the end of the journal.
            res = sd_journal_previous_skip(journal, maxEntriesNum + 1);
        }
        if (res < 0) {
            logWarning() << "Failed to skip journald entries.";
            sd_journal_close(journal);
            return QList<JournalEntry>();
        }
    } else {
        if (m_cursor) {
            // Return to the first new entry.
            sd_journal_seek_cursor(journal, m_cursor);
            sd_journal_next(journal);
        }
    }

    // Iterate over filtered entries.
    forever {
        JournalEntry entry;
        res = sd_journal_next(journal);
        if (res < 0) {
            logWarning() << "Failed to access next journald entry.";
            break;
        }
        if (res == 0) {
            // Reached the end.
            break;
        }
        entry = readJournalEntry(journal);
        entryList.append(entry);
    }

    if (m_cursor)
        free(m_cursor);
    res = sd_journal_get_cursor(journal, &m_cursor);
    sd_journal_close(journal);
    if (entryList.size() > 0)
        logDebug() << "Read" << entryList.size() << "journald entries.";
    return entryList;
}

JournaldAnalyzer::JournalEntry JournaldAnalyzer::readJournalEntry(sd_journal *journal) const
{
    // Reads a single journal entry into JournalEntry structure.
    JournalEntry entry;
    const void *data;
    size_t length;
    uint64_t time;
    int res;

    res = sd_journal_get_realtime_usec(journal, &time);
    if (res == 0) {
        entry.date.setMSecsSinceEpoch(time / 1000);
    }

    res = sd_journal_get_data(journal, "SYSLOG_IDENTIFIER", &data, &length);
    if (res == 0) {
        entry.unit = QString::fromLatin1((const char *)data, length).section("=", 1);
    } else {
        res = sd_journal_get_data(journal, "_SYSTEMD_UNIT", &data, &length);
        if (res == 0) {
            entry.unit = QString::fromLatin1((const char *)data, length).section("=", 1);
        }
    }

    res = sd_journal_get_data(journal, "MESSAGE", &data, &length);
    if (res == 0) {
        entry.message = QString::fromLatin1((const char *)data, length).section("=", 1);
    }

    res = sd_journal_get_data(journal, "PRIORITY", &data, &length);
    if (res == 0) {
        entry.priority = QString::fromLatin1((const char *)data, length).section("=", 1).toInt();
    }

    res = sd_journal_get_data(journal, "_BOOT_ID", &data, &length);
    if (res == 0) {
        entry.bootID = QString::fromLatin1((const char *)data, length).section("=", 1);
    }

    return entry;
}

int JournaldAnalyzer::updateModel(QList<JournalEntry> &entries, ReadingMode readingMode)
{
    int entriesNum = entries.size();
    for (int i = 0; i < entriesNum; i++) {
        const JournalEntry &entry = entries.at(i);
        QStringList itemComponents;
        itemComponents << entry.unit << entry.message;
        LogLine *line = new LogLine(logLineInternalIdGenerator++, entry.date, itemComponents, QString(),
                                    Globals::instance().logLevelByPriority(entry.priority), logMode);
        line->setRecent(readingMode == UpdatingRead);
        logViewModel->insertNewLogLine(line);

        if (readingMode == FullRead)
            informOpeningProgress(i, entriesNum);
    }
    return entriesNum;
}

QStringList JournaldAnalyzer::getUniqueFieldValues(const QString id) const
{
    QStringList units;
    sd_journal *journal;
    int res = sd_journal_open(&journal, m_journalFlags);
    if (res == 0) {
        const void *data;
        size_t length;

        // Get all unique field values. The order is not defined.
        res = sd_journal_query_unique(journal, id.toLatin1().constData());
        if (res == 0) {
            SD_JOURNAL_FOREACH_UNIQUE(journal, data, length)
            {
                units.append(QString::fromLatin1((const char *)data, length).section("=", 1));
            }
        }

        units.removeDuplicates();
        units.sort();
        sd_journal_close(journal);
    } else {
        logWarning() << "Failed to open the journal and extract unique values for field" << id;
    }
    return units;
}
