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

#include <QString>
#include <QStringList>

#include "logFile.h"

#include "logFileReader.h"

class LocalLogFileReaderPrivate;

class LocalLogFileReader : public LogFileReader
{
    Q_OBJECT

public:
    explicit LocalLogFileReader(const LogFile &logFile);

    ~LocalLogFileReader() override;

    void watchFile(bool enable) override;

    void setPreviousFilePosition(long previousFilePosition);

private Q_SLOTS:
    void logFileModified();

private:
    void init();

    QIODevice *open();
    void close(QIODevice *inputDevice);

    QStringList readContent(QIODevice *inputDevice);

protected:
    LocalLogFileReader(LocalLogFileReaderPrivate &dd, const LogFile &logFile);

private:
    Q_DECLARE_PRIVATE(LocalLogFileReader)
};

