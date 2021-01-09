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

#include "xsessionAnalyzer.h"

XSessionAnalyzer::XSessionAnalyzer(LogMode *logMode)
    : FileAnalyzer(logMode)
    , mCurrentDateTime(QDateTime::currentDateTime())
{
}

LogViewColumns XSessionAnalyzer::initColumns()
{
    LogViewColumns columns;

    columns.addColumn(LogViewColumn(i18n("Line"), true, false));
    columns.addColumn(LogViewColumn(i18n("Program"), true, false));
    columns.addColumn(LogViewColumn(i18n("Message"), true, false));

    columns.setGroupByDay(false);
    columns.setGroupByHour(false);

    return columns;
}

Analyzer::LogFileSortMode XSessionAnalyzer::logFileSortMode()
{
    auto *configuration = mLogMode->logModeConfiguration<XSessionConfiguration *>();
    if (configuration->isIgnoreXorgErrors()) {
        return Analyzer::FilteredLogFile;
    } else {
        return Analyzer::AscendingSortedLogFile;
    }
}

LogLine *XSessionAnalyzer::parseMessage(const QString &logLine, const LogFile &originalFile)
{
    const int classPrototypePosition = logLine.indexOf(QLatin1String("::"));
    int programPos = logLine.indexOf(QLatin1Char(':'));

    // If the first found : is the begin of a :: (example: QFile::at:) then we move to the next :
    if (classPrototypePosition != -1 && programPos == classPrototypePosition) {
        programPos = logLine.indexOf(QLatin1Char(':'), classPrototypePosition + 2);
    }

    QString program;
    QString message;
    if (programPos == -1) {
        program = QLatin1String("");
        message = logLine.simplified();
    } else {
        program = logLine.left(programPos);
        message = logLine.right(logLine.length() - programPos - 1);
    }

    message = message.simplified();

    // Do not add this line if this is a X error that the user wants to ignore
    if (isXorgError(program)) {
        return nullptr;
    }

    // Find the right log level
    LogLevel *logLevel;
    if (hasErrorKeywords(message)) {
        logLevel = Globals::instance().errorLogLevel();
    } else if (hasWarningKeywords(message)) {
        logLevel = Globals::instance().warningLogLevel();
    } else {
        logLevel = Globals::instance().informationLogLevel();
    }

    return new LogLine(mLogLineInternalIdGenerator++, mCurrentDateTime, QStringList() << program << message,
                       originalFile.url().toLocalFile(), logLevel, mLogMode);
}

bool XSessionAnalyzer::isXorgError(const QString &program)
{
    auto *configuration = mLogMode->logModeConfiguration<XSessionConfiguration *>();
    if (configuration->isIgnoreXorgErrors() && configuration->xorgErrorKeywords().contains(program)) {
        return true;
    }

    return false;
}

bool XSessionAnalyzer::hasWarningKeywords(const QString &message)
{
    auto *configuration = mLogMode->logModeConfiguration<XSessionConfiguration *>();
    return hasKeywords(message, configuration->warningKeywords());
}

bool XSessionAnalyzer::hasErrorKeywords(const QString &message)
{
    auto *configuration = mLogMode->logModeConfiguration<XSessionConfiguration *>();
    return hasKeywords(message, configuration->errorKeywords());
}

bool XSessionAnalyzer::hasKeywords(const QString &message, const QStringList &keywords)
{
    for (const QString &keyword : keywords) {
        if (message.contains(keyword, Qt::CaseInsensitive)) {
            return true;
        }
    }

    return false;
}
