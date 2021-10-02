/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#define PROCESS_OUTPUT_UPDATER_INTERVAL 10000

#include <QProcess>

#include "logFile.h"

#include "logFileReader.h"


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

