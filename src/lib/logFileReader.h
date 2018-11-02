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

#ifndef _LOG_FILE_READER_H_
#define _LOG_FILE_READER_H_

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

    virtual ~LogFileReader();

    virtual void watchFile(bool enable) = 0;

    LogFile logFile() const;

Q_SIGNALS:
    void contentChanged(LogFileReader *origin, Analyzer::ReadingMode readingMode,
                        const QStringList &newLines);

    void statusBarChanged(const QString &message);
    void errorOccured(const QString &title, const QString &message);

protected:
    LogFileReaderPrivate *const d_ptr;
    LogFileReader(LogFileReaderPrivate &dd, const LogFile &logFile);

private:
    Q_DECLARE_PRIVATE(LogFileReader)
};

#endif // _LOG_FILE_READER_H_
