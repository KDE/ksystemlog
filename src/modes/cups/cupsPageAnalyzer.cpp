/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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

    return new LogLine(mLogLineInternalIdGenerator++,
                       dateTime,
                       capturedTexts,
                       originalLogFile.url().toLocalFile(),
                       Globals::instance().informationLogLevel(),
                       mLogMode);
}
