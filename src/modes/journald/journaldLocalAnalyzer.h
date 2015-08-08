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

#ifndef _JOURNALD_ANALYZER_H_
#define _JOURNALD_ANALYZER_H_

#include "analyzer.h"
#include "logFile.h"

#include <QFutureWatcher>
#include <QSocketNotifier>

#include <systemd/sd-journal.h>

class JournaldLocalAnalyzer : public Analyzer
{
    Q_OBJECT

public:
    explicit JournaldLocalAnalyzer(LogMode *logMode);

    virtual ~JournaldLocalAnalyzer();

    virtual LogViewColumns initColumns();

    virtual void setLogFiles(const QList<LogFile> &logFiles);

    virtual void watchLogFiles(bool enabled);

    static QStringList units();
    static QStringList syslogIdentifiers();

private slots:
    void readJournalInitialFinished();
    void readJournalUpdateFinished();
    void journalDescriptorUpdated(int fd);

private:
    struct JournalEntry {
        QDateTime date;
        QString unit;
        QString message;
        int priority;
        QString bootID;
    };

    typedef QFutureWatcher<QList<JournalEntry>> JournalWatcher;

    void readJournalFinished(ReadingMode readingMode);
    QList<JournalEntry> readJournal(const QStringList &filters);
    JournalEntry readJournalEntry(sd_journal *journal) const;
    int updateModel(QList<JournalEntry> &entries, ReadingMode readingMode);

    static QStringList getUniqueFieldValues(const QString id, int flags = 0);
    void fillCurrentBootID();

    sd_journal *m_journal;
    int m_journalFlags;
    QString m_currentBootID;

    char *m_cursor;
    QMutex m_workerMutex;
    QSocketNotifier *m_journalNotifier;

    bool m_forgetWatchers;
    QList<JournalWatcher *> m_journalWatchers;
};

#endif // _JOURNALD_ANALYZER_H_
