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

#include "logFile.h"

#include "logging.h"

LogFile::LogFile()
{
    // Nothing to do
}

LogFile::LogFile(const LogFile &logFile)
{
    mUrl = logFile.url();
    mDefaultLogLevel = logFile.defaultLogLevel();
}

LogFile::LogFile(const QUrl &url, LogLevel *defaultLogLevel)
{
    mUrl = url;
    mDefaultLogLevel = defaultLogLevel;
}

LogFile::~LogFile()
{
}

bool LogFile::operator==(const LogFile &other) const
{
    if (mUrl == other.url() && mDefaultLogLevel == other.defaultLogLevel()) {
        return true;
    }

    return false;
}

LogFile &LogFile::operator=(const LogFile &logFile)
{
    mUrl = logFile.url();
    mDefaultLogLevel = logFile.defaultLogLevel();

    return *this;
}

QUrl LogFile::url() const
{
    return mUrl;
}

LogLevel *LogFile::defaultLogLevel() const
{
    return mDefaultLogLevel;
}

QDataStream &operator<<(QDataStream &out, const LogFile &logFile)
{
    out << logFile.url().toLocalFile();
    return out;
}

QDebug &operator<<(QDebug &out, const LogFile &logFile)
{
    out << logFile.url().toLocalFile();
    return out;
}
