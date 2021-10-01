/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
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

#include "systemConfiguration.h"

SystemConfiguration::SystemConfiguration()
    : GenericLogModeConfiguration(QStringLiteral(SYSTEM_LOG_MODE_ID),
                                  QStringList() << QStringLiteral("/var/log/syslog"),
                                  QList<int>() << Globals::INFORMATION_LOG_LEVEL_ID)
{
    /*
        <default>/var/log/messages,/var/log/syslog,/var/log/debug</default>
        <default>2,2,1</default>
        */
}
