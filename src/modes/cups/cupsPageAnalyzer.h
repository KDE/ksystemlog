/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KLocalizedString>

#include "fileAnalyzer.h"

#include "localLogFileReader.h"
#include "logging.h"
#include "parsingHelper.h"

#include "cupsPageLogMode.h"

class CupsPageAnalyzer : public FileAnalyzer
{
    Q_OBJECT

public:
    explicit CupsPageAnalyzer(LogMode *logMode);

    ~CupsPageAnalyzer() override
    {
    }

    LogViewColumns initColumns() override;

protected:
    const QRegExp mCupsPageRegex;

    LogFileReader *createLogFileReader(const LogFile &logFile) override;

    Analyzer::LogFileSortMode logFileSortMode() override;

    /*
     * https://www.cups.org/doc/man-cupsd-logs.html
     * Format : printer user job-id date-time page-number num-copies job-billing
     *
     * DeskJet root 2 [20/May/1999:19:21:05 +0000] 1 1 acme-123
     * DeskJet root 2 [20/May/1999:19:21:05 +0000] 2 1 acme-123
     */
    LogLine *parseMessage(const QString &logLine, const LogFile &originalLogFile) override;
};

