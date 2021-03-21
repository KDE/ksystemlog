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

#pragma once

#include "journaldAnalyzer.h"

#include <QFutureWatcher>
#include <QSocketNotifier>

#include <systemd/sd-journal.h>

class JournaldLocalAnalyzer : public JournaldAnalyzer
{
    Q_OBJECT

public:
    explicit JournaldLocalAnalyzer(LogMode *mode, QString filter = QString());

    ~JournaldLocalAnalyzer() override;

    void watchLogFiles(bool enabled) override;

    QStringList units() const override;

    QStringList syslogIdentifiers() const override;

    static QStringList unitsStatic();

    static QStringList syslogIdentifiersStatic();

private Q_SLOTS:
    void readJournalInitialFinished();
    void readJournalUpdateFinished();
    void journalDescriptorUpdated(int fd);

private:
    using JournalWatcher = QFutureWatcher<QList<JournalEntry>>;

    void readJournalFinished(ReadingMode readingMode);
    QList<JournalEntry> readJournal(const QStringList &filters);
    bool prepareJournalReading(sd_journal *journal, const QStringList &filters);
    JournalEntry readJournalEntry(sd_journal *journal) const;

    static QStringList getUniqueFieldValues(const QString &id, int flags = 0);

    QStringList mFilters;
    QString mFilterName;
    sd_journal *mJournal = nullptr;
    int mJournalFlags = 0;
    QString mCurrentBootID;

    char *mCursor = nullptr;
    QMutex mWorkerMutex;
    QSocketNotifier *mJournalNotifier = nullptr;

    bool mForgetWatchers = true;
    QList<JournalWatcher *> mJournalWatchers;
};

