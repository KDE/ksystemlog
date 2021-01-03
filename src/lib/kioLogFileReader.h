 /***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   Copyright (C) 2007 by Nicolas Ternisien                               *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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

#ifndef KIO_LOG_FILE_READER_H
#define KIO_LOG_FILE_READER_H

#include <QObject>
#include <QByteArray>
#include <QString>

#include "logFile.h"

namespace KIO
{
class Job;
}

class KioLogFileReaderPrivate;

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

    KioLogFileReaderPrivate *const d;
};

#endif // _KIO_LOG_FILE_READER_H
