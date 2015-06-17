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

    // TODO: initialize necessary flags.
    m_journalFlags = 0;
    sd_journal_open(&m_journal, m_journalFlags);

    m_futureWatcher = new QFutureWatcher<QList<JournalEntry>>;
    connect(m_futureWatcher, SIGNAL(finished()), this, SLOT(readJournalInitialFinished()));

    int fd = sd_journal_get_fd(m_journal);
    m_journalNotifier = new QSocketNotifier(fd, QSocketNotifier::Read);
    connect(m_journalNotifier, SIGNAL(activated(int)), this, SLOT(journalDescriptorUpdated(int)));
}

JournaldAnalyzer::~JournaldAnalyzer()
{
    m_journalNotifier->setEnabled(false);
    sd_journal_close(m_journal);
    free(m_cursor);
    delete m_journalNotifier;
    delete m_futureWatcher;
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
    if (enabled) {
        m_future = QtConcurrent::run(this, &JournaldAnalyzer::readJournal, QStringList(), m_cursor);
        m_futureWatcher->setFuture(m_future);
    }
    m_journalNotifier->setEnabled(enabled);
}

void JournaldAnalyzer::readJournalInitialFinished()
{
    if (parsingPaused) {
        logDebug() << "Parsing is paused, discarding journald entries.";
        return;
    }

    if (m_future.result().size() == 0) {
        logDebug() << "Received no entries";
        return;
    }

    m_entries = m_future.result();

    insertionLocking.lock();
    logViewModel->startingMultipleInsertions();

    emit statusBarChanged(i18n("Reading journald entries..."));

    // Start the loading bar.
    emit readFileStarted(*logMode, LogFile(), 0, 1);

    // Add journald entries to the model.
    int entriesInserted = updateModel();

    emit statusBarChanged(i18n("Journald entires loaded successfully."));

    logViewModel->endingMultipleInsertions(FullRead, entriesInserted);

    // Stop the loading bar.
    emit readEnded();

    // Inform LogManager that new lines have been added.
    emit logUpdated(entriesInserted);

    insertionLocking.unlock();
}

void JournaldAnalyzer::readJournalUpdateFinished()
{
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

//    // Read new entries in another thread
//    if (!m_futureWatcher->isRunning()) {
//        m_future = QtConcurrent::run(this, &JournaldAnalyzer::readJournal, QStringList(), m_cursor);
//        m_futureWatcher->setFuture(m_future);
//    }
}

QList<JournaldAnalyzer::JournalEntry> JournaldAnalyzer::readJournal(const QStringList &filters, char *cursor)
{
    QList<JournalEntry> entryList;
    int res;
    sd_journal *journal;
    res = sd_journal_open(&journal, m_journalFlags);
    if (res < 0) {
        logWarning() << "Failed to access journald.";
        return QList<JournalEntry>();
    }

    // Set entries filter.
    for (const QString &filter : filters) {
        res = sd_journal_add_match(journal, filter.toLatin1(), 0);
        if (res < 0) {
            logWarning() << "Failed to set journald filter.";
            return QList<JournalEntry>();
        }
    }

    // Seek to cursor.
    if (cursor) {
        res = sd_journal_seek_cursor(journal, cursor);
        if (res < 0) {
            logWarning() << "Failed to seek journald cursor:" << res;
            return QList<JournalEntry>();
        }

        res = sd_journal_test_cursor(journal, cursor);
        if (res <= 0) {
            logWarning() << "Journald cursor test failed:" << res;
            return QList<JournalEntry>();
        }
    }

    // If journal contains lots of entries, read maximum allowed number of latest entries.
    int allowedEntriesNum = KSystemLogConfig::maxLines();
    int entriesNum = 0;
    // Determine number of entries from cursor to the end of the journal.
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

    if (entriesNum > allowedEntriesNum) {
        logDebug() << "Journald contains" << entriesNum << "entries, will read last" << allowedEntriesNum
                   << "entries.";
        int skipEntriesNum = entriesNum - allowedEntriesNum;
        if (cursor) {
            // Jump over skipEntriesNum entries forward starting from cursor.
            sd_journal_seek_cursor(journal, cursor);
            res = sd_journal_next_skip(journal, skipEntriesNum);
        } else {
            sd_journal_seek_tail(journal);
            // Jump over skipEntriesNum entries backwards from the end of the journal.
            res = sd_journal_previous_skip(journal, allowedEntriesNum + 1);
        }
        if (res < 0) {
            logWarning() << "Failed to skip journald entries.";
            return QList<JournalEntry>();
        }
    }

    // Iterate over filtered entries.
    forever {
        JournalEntry entry;
        res = sd_journal_next(journal);
        if (res < 0) {
            logWarning() << "Failed to iterate to next journald entry.";
            break;
        }
        if (res == 0) {
            // Reached the end.
            break;
        }
        entry = readJournalEntry(journal);
        entryList.append(entry);
    }

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

int JournaldAnalyzer::updateModel()
{
    int entriesNum = m_entries.size();
    for (int i = 0; i < entriesNum; i++) {
        const JournalEntry &entry = m_entries.at(i);
        QStringList itemComponents;
        itemComponents << entry.unit << entry.message;
        LogLine *line = new LogLine(logLineInternalIdGenerator++, entry.date, itemComponents, QString(),
                                    Globals::instance()->logLevelByPriority(entry.priority), logMode);
        line->setRecent(false);
        logViewModel->insertNewLogLine(line);
        informOpeningProgress(i, entriesNum);
    }
    return entriesNum;
}
