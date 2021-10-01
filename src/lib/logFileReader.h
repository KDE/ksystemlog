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

#include <QObject>
#include <QString>
#include <QStringList>

#include "analyzer.h"
#include "logFile.h"

class LogFileReaderPrivate;

class LogFileReader : public QObject
{
    Q_OBJECT

public:
    explicit LogFileReader(const LogFile &logFile);

    ~LogFileReader() override;

    virtual void watchFile(bool enable) = 0;

    LogFile logFile() const;

Q_SIGNALS:
    void contentChanged(LogFileReader *origin, Analyzer::ReadingMode readingMode, const QStringList &newLines);

    void statusBarChanged(const QString &message);
    void errorOccured(const QString &title, const QString &message);

protected:
    LogFileReaderPrivate *const d_ptr;
    LogFileReader(LogFileReaderPrivate &dd, const LogFile &logFile);

private:
    Q_DECLARE_PRIVATE(LogFileReader)
};

