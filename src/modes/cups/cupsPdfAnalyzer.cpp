/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "cupsPdfAnalyzer.h"

CupsPdfAnalyzer::CupsPdfAnalyzer(LogMode *logMode)
    : FileAnalyzer(logMode)
    , mCupsPdfRegex(QLatin1String("\\S* ") + ParsingHelper::instance()->syslogDateTimeRegexp() + QLatin1String(R"([ ]+\[(\w*)\][ ]+(.*))"))
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
        qCDebug(KSYSTEMLOG) << "Unable to parse line " << logLine;
        return nullptr;
    }

    QStringList capturedTexts = mCupsPdfRegex.capturedTexts();

    /*
  qCDebug(KSYSTEMLOG) << "------------------------------------------";
        foreach(QString cap, capturedTexts) {
    qCDebug(KSYSTEMLOG) << cap;
        }
  qCDebug(KSYSTEMLOG) << "------------------------------------------";
        */

    // Remove full line
    capturedTexts.removeAt(0);

    const QDateTime dateTime = ParsingHelper::instance()->parseSyslogDateTime(capturedTexts.takeAt(0));
    LogLevel *logLevel = findLogLevel(capturedTexts.takeAt(0));

    return new LogLine(mLogLineInternalIdGenerator++, dateTime, capturedTexts, originalLogFile.url().toLocalFile(), logLevel, mLogMode);
}

LogLevel *CupsPdfAnalyzer::findLogLevel(const QString &level)
{
    if (level == QLatin1String("ERROR")) {
        return Globals::instance().errorLogLevel();
    }

    // level == "STATUS"
    return Globals::instance().informationLogLevel();
}
