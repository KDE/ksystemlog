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

#pragma once

#define PROCESS_OUTPUT_UPDATER_INTERVAL 10000

#include <QProcess>

#include "logFile.h"

#include "logFileReader.h"

class QProcess;

class ProcessOutputLogFileReaderPrivate;

class ProcessOutputLogFileReader : public LogFileReader
{
    Q_OBJECT

public:
    explicit ProcessOutputLogFileReader(const LogFile &logFile);

    ~ProcessOutputLogFileReader() override;

    void watchFile(bool enable) override;

    void setPreviousFilePosition(long previousFilePosition);

private Q_SLOTS:
    void startProcess();

    void logFileModified();

    void emitProcessOutput(int, QProcess::ExitStatus);

private:
    void init();

    void closeProcess();

    void emptyBuffer();

protected:
    ProcessOutputLogFileReader(ProcessOutputLogFileReaderPrivate &dd, const LogFile &logFile);

private:
    Q_DECLARE_PRIVATE(ProcessOutputLogFileReader)
};

