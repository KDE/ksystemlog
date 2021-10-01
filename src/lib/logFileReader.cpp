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

#include "logFileReader.h"

#include "logFileReaderPrivate.h"

#include "logging.h"

LogFileReader::LogFileReader(const LogFile &logFile)
    : d_ptr(new LogFileReaderPrivate)
{
    Q_D(LogFileReader);
    d->logFile = logFile;
}

LogFileReader::LogFileReader(LogFileReaderPrivate &dd, const LogFile &logFile)
    : d_ptr(&dd)
{
    Q_D(LogFileReader);
    d->logFile = logFile;
}

LogFileReader::~LogFileReader()
{
    delete d_ptr;
}

LogFile LogFileReader::logFile() const
{
    // const LogFileReaderPrivate * const d = d_func();
    Q_D(const LogFileReader);
    return d->logFile;
}
