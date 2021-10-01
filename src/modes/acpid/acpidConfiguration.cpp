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

#include "acpidConfiguration.h"

AcpidConfiguration::AcpidConfiguration()
{
    mConfiguration->setCurrentGroup(QStringLiteral("AcpidLogMode"));

    const QStringList defaultAcpidPaths{QStringLiteral("/var/log/acpid")};
    mConfiguration->addItemStringList(QStringLiteral("LogFilesPaths"), mAcpidPaths, defaultAcpidPaths, QStringLiteral("LogFilesPaths"));
}

AcpidConfiguration::~AcpidConfiguration()
{
}

QStringList AcpidConfiguration::acpidPaths() const
{
    return mAcpidPaths;
}

void AcpidConfiguration::setAcpidPaths(const QStringList &acpidPaths)
{
    mAcpidPaths = acpidPaths;
}
