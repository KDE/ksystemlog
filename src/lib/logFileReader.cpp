/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
