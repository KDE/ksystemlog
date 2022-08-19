/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QString>

#include "fileAnalyzer.h"

#include "logFile.h"

class LogFileReader;

class LogMode;
class LogLine;

class SyslogAnalyzer : public FileAnalyzer
{
    Q_OBJECT

public:
    explicit SyslogAnalyzer(LogMode *logMode);

    ~SyslogAnalyzer() override;

    LogViewColumns initColumns() override;

protected:
    LogFileReader *createLogFileReader(const LogFile &logFile) override;
    Analyzer::LogFileSortMode logFileSortMode() override;
    LogLine *parseMessage(const QString &logLine, const LogFile &originalFile) override;

private:
    inline QString undefinedHostName();
    inline QString undefinedProcess();
    inline LogLine *undefinedLogLine(const QString &message, const LogFile &originalFile);
};
