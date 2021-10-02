/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KLocalizedString>

#include "fileAnalyzer.h"

#include "ksystemlog_debug.h"
#include "localLogFileReader.h"
#include "parsingHelper.h"

#include "apacheAccessLogMode.h"

class ApacheAccessAnalyzer : public FileAnalyzer
{
    Q_OBJECT

public:
    explicit ApacheAccessAnalyzer(LogMode *logMode);

    ~ApacheAccessAnalyzer() override
    {
    }

    LogViewColumns initColumns() override;

protected:
    LogFileReader *createLogFileReader(const LogFile &logFile) override;

    Analyzer::LogFileSortMode logFileSortMode() override;

    LogLine *parseMessage(const QString &logLine, const LogFile &originalLogFile) override;
};

