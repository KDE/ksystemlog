/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "cupsAccessAnalyzer.h"

CupsAccessAnalyzer::CupsAccessAnalyzer(LogMode *logMode)
    : FileAnalyzer(logMode)
    , mCupsAccessRegex(QStringLiteral("(\\S*) (\\S*) (\\S*) \\[(.*)\\] \"(.*)\" (\\S*) (\\S*) (\\S*) (\\S*)"))
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

LogFileReader *CupsAccessAnalyzer::createLogFileReader(const LogFile &logFile)
{
    return new LocalLogFileReader(logFile);
}

Analyzer::LogFileSortMode CupsAccessAnalyzer::logFileSortMode()
{
    return Analyzer::AscendingSortedLogFile;
}

LogLine *CupsAccessAnalyzer::parseMessage(const QString &logLine, const LogFile &originalLogFile)
{
    int firstPosition = mCupsAccessRegex.indexIn(logLine);
    if (firstPosition == -1) {
        qCDebug(KSYSTEMLOG) << "Unable to parse line " << logLine;
        return nullptr;
    }

    QStringList capturedTexts = mCupsAccessRegex.capturedTexts();

    // Remove full line
    capturedTexts.removeAt(0);

    capturedTexts.replace(5, ParsingHelper::instance()->parseHttpResponse(capturedTexts.at(5)));
    capturedTexts.replace(6, ParsingHelper::instance()->parseSize(capturedTexts.at(6)));

    const QDateTime dateTime = ParsingHelper::instance()->parseHttpDateTime(capturedTexts.takeAt(3));

    LogLevel *logLevel = findLevel(capturedTexts.at(capturedTexts.count() - 1));

    return new LogLine(mLogLineInternalIdGenerator++, dateTime, capturedTexts, originalLogFile.url().toLocalFile(), logLevel, mLogMode);
}
