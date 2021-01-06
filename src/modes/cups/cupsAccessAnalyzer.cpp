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

#include "cupsAccessAnalyzer.h"

CupsAccessAnalyzer::CupsAccessAnalyzer(LogMode *logMode)
    : FileAnalyzer(logMode)
    , cupsAccessRegex(
          QStringLiteral("(\\S*) (\\S*) (\\S*) \\[(.*)\\] \"(.*)\" (\\S*) (\\S*) (\\S*) (\\S*)"))
{
}

LogViewColumns CupsAccessAnalyzer::initColumns()
{
    LogViewColumns columns;

    columns.addColumn(LogViewColumn(i18n("Date"), true, false));
    columns.addColumn(LogViewColumn(i18n("Host"), true, true));
    columns.addColumn(LogViewColumn(i18n("Group"), true, true));
    columns.addColumn(LogViewColumn(i18n("User"), true, true));
    columns.addColumn(LogViewColumn(i18n("HTTP Request"), true, false));
    columns.addColumn(LogViewColumn(i18n("Status"), true, true));
    columns.addColumn(LogViewColumn(i18n("Bytes"), true, false));
    columns.addColumn(LogViewColumn(i18n("IPP Operation"), true, false));
    columns.addColumn(LogViewColumn(i18n("IPP Status"), true, false));

    return columns;
}

LogFileReader *CupsAccessAnalyzer::createLogFileReader(const LogFile &logFile) { return new LocalLogFileReader(logFile); }

Analyzer::LogFileSortMode CupsAccessAnalyzer::logFileSortMode() { return Analyzer::AscendingSortedLogFile; }

LogLine *CupsAccessAnalyzer::parseMessage(const QString &logLine, const LogFile &originalLogFile)
{
    int firstPosition = cupsAccessRegex.indexIn(logLine);
    if (firstPosition == -1) {
        logDebug() << "Unable to parse line " << logLine;
        return nullptr;
    }

    QStringList capturedTexts = cupsAccessRegex.capturedTexts();

    // Remove full line
    capturedTexts.removeAt(0);

    capturedTexts.replace(5, ParsingHelper::instance()->parseHttpResponse(capturedTexts.at(5)));
    capturedTexts.replace(6, ParsingHelper::instance()->parseSize(capturedTexts.at(6)));

    QDateTime dateTime = ParsingHelper::instance()->parseHttpDateTime(capturedTexts.takeAt(3));

    LogLevel *logLevel = findLevel(capturedTexts.at(capturedTexts.count() - 1));

    return new LogLine(mLogLineInternalIdGenerator++, dateTime, capturedTexts,
                       originalLogFile.url().toLocalFile(), logLevel, mLogMode);
}
