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

#include "cupsPdfAnalyzer.h"

CupsPdfAnalyzer::CupsPdfAnalyzer(LogMode *logMode)
    : FileAnalyzer(logMode)
    , mCupsPdfRegex(QLatin1String("\\S* ") + ParsingHelper::instance()->syslogDateTimeRegexp()
                    + QLatin1String("[ ]+\\[(\\w*)\\][ ]+(.*)"))
{ // \\[(.*)\\] (\\S*) (\\S*) (\\S*)
}

LogViewColumns CupsPdfAnalyzer::initColumns()
{
    LogViewColumns columns;

    columns.addColumn(LogViewColumn(i18n("Date"), true, false));
    columns.addColumn(LogViewColumn(i18n("Message"), true, false));

    return columns;
}

LogLine *CupsPdfAnalyzer::parseMessage(const QString &logLine, const LogFile &originalLogFile)
{
    int firstPosition = mCupsPdfRegex.indexIn(logLine);
    if (firstPosition == -1) {
        logDebug() << "Unable to parse line " << logLine;
        return nullptr;
    }

    QStringList capturedTexts = mCupsPdfRegex.capturedTexts();

    /*
  logDebug() << "------------------------------------------";
        foreach(QString cap, capturedTexts) {
    logDebug() << cap;
        }
  logDebug() << "------------------------------------------";
        */

    // Remove full line
    capturedTexts.removeAt(0);

    const QDateTime dateTime = ParsingHelper::instance()->parseSyslogDateTime(capturedTexts.takeAt(0));
    LogLevel *logLevel = findLogLevel(capturedTexts.takeAt(0));

    return new LogLine(mLogLineInternalIdGenerator++, dateTime, capturedTexts,
                       originalLogFile.url().toLocalFile(), logLevel, mLogMode);
}

LogLevel *CupsPdfAnalyzer::findLogLevel(const QString &level)
{
    if (level == QLatin1String("ERROR")) {
        return Globals::instance().errorLogLevel();
    }

    // level == "STATUS"
    return Globals::instance().informationLogLevel();
}
