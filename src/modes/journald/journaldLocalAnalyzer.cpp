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

#include "journaldLocalAnalyzer.h"
#include "journaldConfiguration.h"
#include "ksystemlogConfig.h"
#include "logViewModel.h"
#include "logging.h"

#include <KLocalizedString>

#include <QtConcurrent>

JournaldLocalAnalyzer::JournaldLocalAnalyzer(LogMode *mode, QString filter)
    : JournaldAnalyzer(mode)
{
    // Initialize journal access flags and open the journal.
    mJournalFlags = 0;
    auto *configuration = mode->logModeConfiguration<JournaldConfiguration *>();
    switch (configuration->entriesType()) {
    case JournaldConfiguration::EntriesAll:
        break;
    case JournaldConfiguration::EntriesCurrentUser:
        mJournalFlags |= SD_JOURNAL_CURRENT_USER;
        break;
    case JournaldConfiguration::EntriesSystem:
        mJournalFlags |= SD_JOURNAL_SYSTEM;
        break;
    }
    const int ret = sd_journal_open(&mJournal, mJournalFlags);
    if (ret < 0) {
        logWarning() << "Journald analyzer failed to open system journal";
        return;
    }

    const qintptr fd = sd_journal_get_fd(mJournal);
    mJournalNotifier = new QSocketNotifier(fd, QSocketNotifier::Read);
    mJournalNotifier->setEnabled(false);
    connect(mJournalNotifier, &QSocketNotifier::activated, this, &JournaldLocalAnalyzer::journalDescriptorUpdated);

    if (configuration->displayCurrentBootOnly()) {
        QFile file(QLatin1String("/proc/sys/kernel/random/boot_id"));
        if (file.open(QIODevice::ReadOnly | QFile::Text)) {
            QTextStream stream(&file);
            mCurrentBootID = stream.readAll().trimmed();
            mCurrentBootID.remove(QChar::fromLatin1('-'));
            mFilters << QStringLiteral("_BOOT_ID=%1").arg(mCurrentBootID);
        } else {
            logWarning() << "Journald analyzer failed to open /proc/sys/kernel/random/boot_id";
        }
    }

    if (!filter.isEmpty()) {
        mFilters << filter;
        mFilterName = filter.section(QChar::fromLatin1('='), 1);
    }
}

JournaldLocalAnalyzer::~JournaldLocalAnalyzer()
{
    watchLogFiles(false);
    sd_journal_close(mJournal);
    delete mJournalNotifier;
}

void JournaldLocalAnalyzer::watchLogFiles(bool enabled)
{
    if (!mJournalNotifier) {
        return;
    }
    mJournalNotifier->setEnabled(enabled);

    mWorkerMutex.lock();
    mForgetWatchers = enabled;
    mWorkerMutex.unlock();

    if (enabled) {
        auto watcher = new JournalWatcher();
        mWorkerMutex.lock();
        mJournalWatchers.append(watcher);
        mWorkerMutex.unlock();
        connect(watcher, &JournalWatcher::finished, this, &JournaldLocalAnalyzer::readJournalInitialFinished);
        watcher->setFuture(QtConcurrent::run(this, &JournaldLocalAnalyzer::readJournal, mFilters));
    } else {
        for (JournalWatcher *watcher : mJournalWatchers) {
            watcher->waitForFinished();
        }
        qDeleteAll(mJournalWatchers);
        mJournalWatchers.clear();

        if (mCursor) {
            free(mCursor);
            mCursor = nullptr;
        }
    }
}

QStringList JournaldLocalAnalyzer::units() const
{
    return JournaldLocalAnalyzer::unitsStatic();
}

QStringList JournaldLocalAnalyzer::unitsStatic()
{
    return getUniqueFieldValues(QStringLiteral("_SYSTEMD_UNIT"));
}

QStringList JournaldLocalAnalyzer::syslogIdentifiers() const
{
    return JournaldLocalAnalyzer::syslogIdentifiersStatic();
}

QStringList JournaldLocalAnalyzer::syslogIdentifiersStatic()
{
    return getUniqueFieldValues(QStringLiteral("SYSLOG_IDENTIFIER"));
}

void JournaldLocalAnalyzer::readJournalInitialFinished()
{
    readJournalFinished(FullRead);
}

void JournaldLocalAnalyzer::readJournalUpdateFinished()
{
    readJournalFinished(UpdatingRead);
}

void JournaldLocalAnalyzer::readJournalFinished(ReadingMode readingMode)
{
    auto watcher = static_cast<JournalWatcher *>(sender());
    if (!watcher) {
        return;
    }

    QList<JournalEntry> entries = watcher->result();

    if (mParsingPaused) {
        logDebug() << "Parsing is paused, discarding journald entries.";
    } else if (entries.empty()) {
        logDebug() << "Received no entries.";
    } else {
        mInsertionLocking.lock();
        mLogViewModel->startingMultipleInsertions();

        if (FullRead == readingMode) {
            Q_EMIT statusBarChanged(i18n("Reading journald entries..."));
            // Start displaying the loading bar.
            Q_EMIT readFileStarted(*mLogMode, LogFile(), 0, 1);
        }

        // Add journald entries to the model.
        int entriesInserted = updateModel(entries, readingMode);

        mLogViewModel->endingMultipleInsertions(readingMode, entriesInserted);

        if (FullRead == readingMode) {
            Q_EMIT statusBarChanged(i18n("Journald entries loaded successfully."));

            // Stop displaying the loading bar.
            Q_EMIT readEnded();
        }

        // Inform LogManager that new lines have been added.
        Q_EMIT logUpdated(entriesInserted);

        mInsertionLocking.unlock();
    }

    mWorkerMutex.lock();
    if (mForgetWatchers) {
        mJournalWatchers.removeAll(watcher);
        watcher->deleteLater();
    }
    mWorkerMutex.unlock();
}

void JournaldLocalAnalyzer::journalDescriptorUpdated(int fd)
{
    logDebug() << "Journal was updated.";
    QFile file;
    file.open(fd, QIODevice::ReadOnly);
    file.readAll();
    file.close();

    if (mParsingPaused) {
        logDebug() << "Parsing is paused, will not fetch new journald entries.";
        return;
    }

    auto watcher = new JournalWatcher();
    mWorkerMutex.lock();
    mJournalWatchers.append(watcher);
    mWorkerMutex.unlock();
    connect(watcher, &JournalWatcher::finished, this, &JournaldLocalAnalyzer::readJournalUpdateFinished);
    watcher->setFuture(QtConcurrent::run(this, &JournaldLocalAnalyzer::readJournal, mFilters));
}

QList<JournaldLocalAnalyzer::JournalEntry> JournaldLocalAnalyzer::readJournal(const QStringList &filters)
{
    QMutexLocker mutexLocker(&mWorkerMutex);
    QList<JournalEntry> entryList;
    sd_journal *journal;

    if (!mFilterName.isEmpty()) {
        Q_EMIT statusChanged(mFilterName);
    }

    int res = sd_journal_open(&journal, mJournalFlags);
    if (res < 0) {
        logWarning() << "Failed to access the journal.";
        return QList<JournalEntry>();
    }

    if (prepareJournalReading(journal, filters)) {
        // Iterate over filtered entries.
        forever {
            JournalEntry entry;
            res = sd_journal_next(journal);
            if (res < 0) {
                logWarning() << "Failed to access next journal entry.";
                break;
            }
            if (res == 0) {
                // Reached last journal entry.
                break;
            }
            entry = readJournalEntry(journal);
            entryList.append(entry);
        }

        free(mCursor);
        sd_journal_get_cursor(journal, &mCursor);
    }

    sd_journal_close(journal);
    if (!entryList.empty()) {
        logDebug() << "Read" << entryList.size() << "journal entries.";
    }
    return entryList;
}

bool JournaldLocalAnalyzer::prepareJournalReading(sd_journal *journal, const QStringList &filters)
{
    int res;

    // Set entries filter.
    for (const QString &filter : filters) {
        res = sd_journal_add_match(journal, filter.toUtf8().constData(), 0);
        if (res < 0) {
            logWarning() << "Failed to set journal filter.";
            return false;
        }
    }

    // Go to the latest journal entry.
    res = sd_journal_seek_tail(journal);
    if (res < 0) {
        logWarning() << "Failed to seek journal tail.";
        return false;
    }

    // Read number of entries allowed by KSystemLog configuration.
    int maxEntriesNum = KSystemLogConfig::maxLines();

    // Seek to cursor.
    if (mCursor) {
        int entriesNum = 0;
        // Continue searching for the oldest entry until
        // either cursor is found or maximum number of entries is traversed.
        while (entriesNum < maxEntriesNum) {
            entriesNum++;

            res = sd_journal_previous(journal);
            if (res < 0) {
                logWarning() << "Failed to seek previous journal entry.";
                return false;
            }

            res = sd_journal_test_cursor(journal, mCursor);
            if (res > 0) {
                if (entriesNum == 1) {
                    // No new entries are found.
                    return false;
                }
                // Latest journal entry before journal update is found.
                break;
            }
        }
    } else {
        // Jump over maxEntriesNum entries backwards from the end of the journal.
        res = sd_journal_previous_skip(journal, maxEntriesNum + 1);
        if (res < 0) {
            // Seek failed. Read entries from the beginning.
            res = sd_journal_seek_head(journal);
            if (res < 0) {
                logWarning() << "Failed to seek journal head.";
                return false;
            }
        }
    }
    return true;
}

JournaldLocalAnalyzer::JournalEntry JournaldLocalAnalyzer::readJournalEntry(sd_journal *journal) const
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
        entry.unit = QString::fromUtf8((const char *)data, length).section(QChar::fromLatin1('='), 1);
    } else {
        res = sd_journal_get_data(journal, "_SYSTEMD_UNIT", &data, &length);
        if (res == 0) {
            entry.unit = QString::fromUtf8((const char *)data, length).section(QChar::fromLatin1('='), 1);
        }
    }

    res = sd_journal_get_data(journal, "MESSAGE", &data, &length);
    if (res == 0) {
        entry.message = QString::fromUtf8((const char *)data, length).section(QChar::fromLatin1('='), 1);
        entry.message.remove(QRegularExpression(QLatin1String(ConsoleColorEscapeSequence)));
    }

    res = sd_journal_get_data(journal, "PRIORITY", &data, &length);
    if (res == 0) {
        entry.priority = QString::fromUtf8((const char *)data, length).section(QChar::fromLatin1('='), 1).toInt();
    }

    res = sd_journal_get_data(journal, "_BOOT_ID", &data, &length);
    if (res == 0) {
        entry.bootID = QString::fromUtf8((const char *)data, length).section(QChar::fromLatin1('='), 1);
    }

    return entry;
}

QStringList JournaldLocalAnalyzer::getUniqueFieldValues(const QString &id, int flags)
{
    QStringList units;
    sd_journal *journal;
    int res = sd_journal_open(&journal, flags);
    if (res == 0) {
        const void *data;
        size_t length;

        // Get all unique field values. The order is not defined.
        res = sd_journal_query_unique(journal, id.toUtf8().constData());
        if (res == 0) {
            SD_JOURNAL_FOREACH_UNIQUE(journal, data, length)
            {
                const QString unit = QString::fromUtf8((const char *)data, length).section(QChar::fromLatin1('='), 1);
                if (unit.startsWith(QLatin1String("systemd-coredump@"))) {
                    continue; // these never contain any log information, and can easily fill up menu
                }
                units.append(unit);
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
