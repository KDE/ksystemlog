/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *   SPDX-FileCopyrightText: 2015 Vyacheslav Matyushin *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
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

