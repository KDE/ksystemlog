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

#include "authenticationConfiguration.h"

AuthenticationConfiguration::AuthenticationConfiguration()
{
    mConfiguration->setCurrentGroup(QStringLiteral("AuthenticationLogMode"));

    const QString defaultAuthenticationPath{QStringLiteral("/var/log/auth.log")};
    mConfiguration->addItemString(QStringLiteral("LogFilePath"), mAuthenticationPath, defaultAuthenticationPath, QStringLiteral("LogFilePath"));

    const QStringList defaultWarningKeywords{QStringLiteral("failed")};
    mConfiguration->addItemStringList(QStringLiteral("WarningKeywords"), mWarningKeywords, defaultWarningKeywords, QStringLiteral("WarningKeywords"));

    const QStringList defaultErrorKeywords{QStringLiteral("error")};
    mConfiguration->addItemStringList(QStringLiteral("ErrorKeywords"), mErrorKeywords, defaultErrorKeywords, QStringLiteral("ErrorKeywords"));
}

AuthenticationConfiguration::~AuthenticationConfiguration()
{
}

QString AuthenticationConfiguration::authenticationPath() const
{
    return mAuthenticationPath;
}

void AuthenticationConfiguration::setAuthenticationPath(const QString &authenticationPath)
{
    mAuthenticationPath = authenticationPath;
}

QStringList AuthenticationConfiguration::warningKeywords() const
{
    return mWarningKeywords;
}

QStringList AuthenticationConfiguration::errorKeywords() const
{
    return mErrorKeywords;
}
