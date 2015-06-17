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

class JournaldAnalyzer : public Analyzer
{
    Q_OBJECT

public:
    explicit JournaldAnalyzer(LogMode *logMode);

    virtual ~JournaldAnalyzer();

    virtual LogViewColumns initColumns();

    virtual void setLogFiles(const QList<LogFile> &logFiles);

    virtual void watchLogFiles(bool enabled);

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

    QList<JournalEntry> readJournal(const QStringList &filters, char *cursor);
    JournalEntry readJournalEntry(sd_journal *journal) const;
    int updateModel();

    sd_journal *m_journal;
    int m_journalFlags;
    char *m_cursor;

    QFutureWatcher<QList<JournalEntry>> *m_futureWatcher;
    QFuture<QList<JournalEntry>> m_future;
    QSocketNotifier *m_journalNotifier;
    QList<JournalEntry> m_entries;
};

#endif // _JOURNALD_ANALYZER_H_
