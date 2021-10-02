/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KLocalizedString>

#include "fileAnalyzer.h"

#include "acpidLogMode.h"
#include "localLogFileReader.h"
#include "parsingHelper.h"

class AcpidAnalyzer : public FileAnalyzer
{
    Q_OBJECT

public:
    explicit AcpidAnalyzer(LogMode *logMode);

    ~AcpidAnalyzer() override
    {
    }

    LogViewColumns initColumns() override;

protected:
    LogFileReader *createLogFileReader(const LogFile &logFile) override
    {
        return new LocalLogFileReader(logFile);
    }

    Analyzer::LogFileSortMode logFileSortMode() override
    {
        return Analyzer::AscendingSortedLogFile;
    }

    LogLine *parseMessage(const QString &logLine, const LogFile &originalFile) override;
};

