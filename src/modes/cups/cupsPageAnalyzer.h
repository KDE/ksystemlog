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

#ifndef _CUPS_PAGE_ANALYZER_H_
#define _CUPS_PAGE_ANALYZER_H_

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
    explicit CupsPageAnalyzer(LogMode *logMode)
        : FileAnalyzer(logMode)
        , cupsPageRegex(QStringLiteral("(\\S*) (\\S*) (\\S*) \\[(.*)\\] (\\S*) (\\S*) (\\S*)"))
    {
    }

    virtual ~CupsPageAnalyzer() {}

    LogViewColumns initColumns()
    {
        LogViewColumns columns;

        columns.addColumn(LogViewColumn(i18n("Date"), true, false));
        columns.addColumn(LogViewColumn(i18n("Printer"), true, true));
        columns.addColumn(LogViewColumn(i18n("User"), true, true));
        columns.addColumn(LogViewColumn(i18n("Job Id"), true, true));
        columns.addColumn(LogViewColumn(i18n("Page Number"), true, false));
        columns.addColumn(LogViewColumn(i18n("Num Copies"), true, true));
        columns.addColumn(LogViewColumn(i18n("Job Billing"), true, false));

        return columns;
    }

protected:
    QRegExp cupsPageRegex;

    LogFileReader *createLogFileReader(const LogFile &logFile) { return new LocalLogFileReader(logFile); }

    Analyzer::LogFileSortMode logFileSortMode() { return Analyzer::AscendingSortedLogFile; }

    /*
     * http://www.cups.org/documentation.php/ref-page_log.html
     * Format : printer user job-id date-time page-number num-copies job-billing
     *
     * DeskJet root 2 [20/May/1999:19:21:05 +0000] 1 1 acme-123
     * DeskJet root 2 [20/May/1999:19:21:05 +0000] 2 1 acme-123
     */
    LogLine *parseMessage(const QString &logLine, const LogFile &originalLogFile)
    {
        QString line(logLine);

        int firstPosition = cupsPageRegex.indexIn(logLine);
        if (firstPosition == -1) {
            logDebug() << "Unable to parse line " << logLine;
            return NULL;
        }

        QStringList capturedTexts = cupsPageRegex.capturedTexts();

        // Remove full line
        capturedTexts.removeAt(0);

        QDateTime dateTime = ParsingHelper::instance()->parseHttpDateTime(capturedTexts.takeAt(3));

        return new LogLine(logLineInternalIdGenerator++, dateTime, capturedTexts,
                           originalLogFile.url().path(), Globals::instance().informationLogLevel(), logMode);
    }
};

#endif // _CUPS_PAGE_ANALYZER_H_
