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

#include "logModeConfiguration.h"

#include "defaults.h"

#include "journaldLogMode.h"
#include "journaldTypes.h"

#include "ksystemlogConfig.h"

using namespace JournaldTypes;

class JournaldConfiguration : public LogModeConfiguration
{
    Q_OBJECT

public:
    enum EntriesType { EntriesAll = 0, EntriesCurrentUser = 1, EntriesSystem = 2 };

    JournaldConfiguration();

    bool displayCurrentBootOnly() const;
    void setDisplayCurrentBootOnly(bool displayCurrentBootOnly);

    EntriesType entriesType();
    void setEntriesType(EntriesType entriesType);

    QVector<JournalAddress> remoteJournals() const;
    void setRemoteJournals(const QVector<JournalAddress> &remoteJournals);

private:
    bool mDisplayCurrentBootOnly;
    int mEntriesType;
    QStringList mRemoteJournals;
};

