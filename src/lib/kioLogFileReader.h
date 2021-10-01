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

#include <QByteArray>
#include <QObject>
#include <QString>

#include "logFile.h"

namespace KIO
{
class Job;
class FileJob;
}
class KDirWatch;
/**
 * TODO Inherits from LogFileReader
 */
class KioLogFileReader : public QObject
{
    Q_OBJECT

public:
    explicit KioLogFileReader(const LogFile &logFile);

    ~KioLogFileReader() override;

    void open();
    void close();

Q_SIGNALS:
    void lineRead(const QString &);

private Q_SLOTS:
    void openDone(KIO::Job *job);
    void closeDone(KIO::Job *job);
    void dataReceived(KIO::Job *job, const QByteArray &data);
    void mimetypeReceived(KIO::Job *job, const QString &type);

    void watchFile(const QString &path);

private:
    void emitCompleteLines();
    const LogFile mLogFile;

    KIO::FileJob *mFileJob = nullptr;

    QString mBuffer;
    qulonglong mTotalRead = 0;

    KDirWatch *const mFileWatch;
};

