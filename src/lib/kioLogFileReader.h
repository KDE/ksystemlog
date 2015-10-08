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

#ifndef _KIO_LOG_FILE_READER_H_
#define _KIO_LOG_FILE_READER_H_

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
    KioLogFileReader(const LogFile &logFile);

    virtual ~KioLogFileReader();

    void open();
    void close();

signals:
    void lineRead(const QString &);

private slots:
    void openDone(KIO::Job *job);
    void closeDone(KIO::Job *job);
    void dataReceived(KIO::Job *job, const QByteArray &data);
    void mimetypeReceived(KIO::Job *job, const QString &type);

    void watchFile(const QString &path);

private:
    void emitCompleteLines();

    KioLogFileReaderPrivate *const d;
};

#endif // _KIO_LOG_FILE_READER_H_
