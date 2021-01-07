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

#include "cupsPageAnalyzer.h"

CupsPageAnalyzer::CupsPageAnalyzer(LogMode *logMode)
    : FileAnalyzer(logMode)
    , mCupsPageRegex(QStringLiteral("(\\S*) (\\S*) (\\S*) \\[(.*)\\] (\\S*) (\\S*) (\\S*)"))
{
}

LogViewColumns CupsPageAnalyzer::initColumns()
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

LogFileReader *CupsPageAnalyzer::createLogFileReader(const LogFile &logFile)
{
    return new LocalLogFileReader(logFile);
}

Analyzer::LogFileSortMode CupsPageAnalyzer::logFileSortMode()
{
    return Analyzer::AscendingSortedLogFile;
}

LogLine *CupsPageAnalyzer::parseMessage(const QString &logLine, const LogFile &originalLogFile)
{
    const int firstPosition = mCupsPageRegex.indexIn(logLine);
    if (firstPosition == -1) {
        logDebug() << "Unable to parse line " << logLine;
        return nullptr;
    }

    QStringList capturedTexts = mCupsPageRegex.capturedTexts();

    // Remove full line
    capturedTexts.removeAt(0);

    const QDateTime dateTime = ParsingHelper::instance()->parseHttpDateTime(capturedTexts.takeAt(3));

    return new LogLine(mLogLineInternalIdGenerator++, dateTime, capturedTexts,
                       originalLogFile.url().toLocalFile(), Globals::instance().informationLogLevel(), mLogMode);
}
