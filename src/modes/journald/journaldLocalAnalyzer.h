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

#ifndef JOURNALD_LOCAL_ANALYZER_H
#define JOURNALD_LOCAL_ANALYZER_H

#include "journaldAnalyzer.h"

#include <QFutureWatcher>
#include <QSocketNotifier>

#include <systemd/sd-journal.h>

class JournaldLocalAnalyzer : public JournaldAnalyzer
{
    Q_OBJECT

public:
    explicit JournaldLocalAnalyzer(LogMode *logMode, QString filter = QString());

    virtual ~JournaldLocalAnalyzer();

    virtual void watchLogFiles(bool enabled);

    virtual QStringList units() const;

    virtual QStringList syslogIdentifiers() const;

    static QStringList unitsStatic();

    static QStringList syslogIdentifiersStatic();

private Q_SLOTS:
    void readJournalInitialFinished();
    void readJournalUpdateFinished();
    void journalDescriptorUpdated(int fd);

private:
    typedef QFutureWatcher<QList<JournalEntry>> JournalWatcher;

    void readJournalFinished(ReadingMode readingMode);
    QList<JournalEntry> readJournal(const QStringList &filters);
    bool prepareJournalReading(sd_journal *journal, const QStringList &filters);
    JournalEntry readJournalEntry(sd_journal *journal) const;

    static QStringList getUniqueFieldValues(const QString id, int flags = 0);

    QStringList m_filters;
    QString m_filterName;
    sd_journal *m_journal;
    int m_journalFlags;
    QString m_currentBootID;

    char *m_cursor;
    QMutex m_workerMutex;
    QSocketNotifier *m_journalNotifier;

    bool m_forgetWatchers;
    QList<JournalWatcher *> m_journalWatchers;
};

#endif // _JOURNALD_LOCAL_ANALYZER_H
