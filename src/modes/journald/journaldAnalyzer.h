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

#include "analyzer.h"
#include "logFile.h"

class JournaldAnalyzer : public Analyzer
{
    Q_OBJECT

public:
    explicit JournaldAnalyzer(LogMode *mode);

    LogViewColumns initColumns() override;

    void setLogFiles(const QVector<LogFile> &logFiles) override;

    virtual QStringList units() const = 0;

    virtual QStringList syslogIdentifiers() const = 0;

protected:
    struct JournalEntry {
        QDateTime date;
        QString unit;
        QString message;
        int priority;
        QString bootID;
    };

    int updateModel(QList<JournalEntry> &entries, ReadingMode readingMode);
};

