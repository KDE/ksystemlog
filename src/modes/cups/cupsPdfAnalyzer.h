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

#ifndef CUPS_PDF_ANALYZER_H
#define CUPS_PDF_ANALYZER_H

#include <KLocalizedString>

#include "fileAnalyzer.h"

#include "localLogFileReader.h"
#include "logging.h"
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

#endif // _CUPS_PDF_ANALYZER_H
