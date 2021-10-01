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

/**
 * Journald Log Mode Identifier
 */
#define JOURNALD_LOG_MODE_ID "journaldLogMode"

/**
 * Journald Log Icon
 */
#define JOURNALD_MODE_ICON "preferences-system-startup"

#include <QList>

#include "logFile.h"
#include "logMode.h"

#include "journaldTypes.h"

using namespace JournaldTypes;

Q_DECLARE_METATYPE(JournaldAnalyzerOptions)

class JournaldLogMode : public LogMode
{
    Q_OBJECT

public:
    explicit JournaldLogMode();

    Analyzer *createAnalyzer(const QVariant &analyzerOptions = QVariant()) override;

    QVector<LogFile> createLogFiles() override;

    void updateJournalFilters(const JournalAddress &address, const JournalFilters &filters);
    JournalFilters filters(const JournalAddress &address) const;

private:
    QMap<JournalAddress, JournalFilters> mRemoteJournalFilters;
};

