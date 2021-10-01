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

#pragma once

#include "syslogAnalyzer.h"

#include "authenticationConfiguration.h"
#include "authenticationLogMode.h"

class AuthenticationAnalyzer : public SyslogAnalyzer
{
    Q_OBJECT

public:
    explicit AuthenticationAnalyzer(LogMode *logMode);

    ~AuthenticationAnalyzer() override
    {
    }

    LogLine *parseMessage(const QString &logLine, const LogFile &originalLogFile) override;

private:
    bool hasWarningKeywords(const QString &message);

    bool hasErrorKeywords(const QString &message);

    bool hasKeywords(const QString &message, const QStringList &keywords);
};

