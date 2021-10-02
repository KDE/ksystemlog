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

#include "cupsPdfLogMode.h"

class CupsPdfAnalyzer : public FileAnalyzer
{
    Q_OBJECT

public:
    // Fri Sep 30 21:58:37 2005  [ERROR] failed to create spool directory (/var/spool/cups-pdf/SPOOL)
    explicit CupsPdfAnalyzer(LogMode *logMode);

    ~CupsPdfAnalyzer() override
    {
    }

    LogViewColumns initColumns() override;

protected:
    const QRegExp mCupsPdfRegex;

    LogFileReader *createLogFileReader(const LogFile &logFile) override
    {
        return new LocalLogFileReader(logFile);
    }

    Analyzer::LogFileSortMode logFileSortMode() override
    {
        return Analyzer::AscendingSortedLogFile;
    }

    /*
     * https://pawn.physik.uni-wuerzburg.de/~vrbehr/cups-pdf/documentation.shtml (cups-pdf_log)
     *
     * Thu Jun 14 12:40:35 2007 [STATUS] identification string sent
     * Thu Jun 14 12:43:07 2007 [ERROR] failed to set file mode for PDF file (non fatal)
     *(/var/spool/cups-pdf/root/Test_Pdf.pdf)
     * Thu Jun 14 12:43:07 2007 [STATUS] PDF creation successfully finished (root)
     * Fri Sep 30 21:58:37 2005  [ERROR] failed to create spool directory (/var/spool/cups-pdf/SPOOL)
     * Sat Oct  1 09:11:45 2005  [ERROR] failed to create spool directory (/var/spool/cups-pdf/SPOOL)
     */
    LogLine *parseMessage(const QString &logLine, const LogFile &originalLogFile) override;

    LogLevel *findLogLevel(const QString &level);
};

