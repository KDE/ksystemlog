/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#include "xorgAnalyzer.h"
#include <KLocalizedString>

XorgAnalyzer::XorgAnalyzer(LogMode *logMode)
    : FileAnalyzer(logMode)
    , mCurrentDateTime(QDateTime::currentDateTime())
{
    initializeTypeName();
}

XorgAnalyzer::~XorgAnalyzer()
{
}

LogViewColumns XorgAnalyzer::initColumns()
{
    LogViewColumns columns;

    columns.addColumn(LogViewColumn(i18n("Line"), false, false));
    columns.addColumn(LogViewColumn(i18n("Type"), false, false));
    columns.addColumn(LogViewColumn(i18n("Message"), false, false));

    columns.setGroupByDay(false);
    columns.setGroupByHour(false);

    return columns;
}

LogFileReader *XorgAnalyzer::createLogFileReader(const LogFile &logFile)
{
    return new LocalLogFileReader(logFile);
}

Analyzer::LogFileSortMode XorgAnalyzer::logFileSortMode()
{
    return Analyzer::AscendingSortedLogFile;
}

LogLine *XorgAnalyzer::parseMessage(const QString &logLine, const LogFile &originalFile)
{
    QString string(logLine);

    const QString type = string.left(4);

    LogLevel *logLineType = findTypeName(type);

    // If the type is not empty, the log message has a type, so we can delete it
    if (logLineType) {
        string.remove(0, 5);
    } else {
        logLineType = Globals::instance().informationLogLevel();
    }

    QStringList list;
    list.append(logLineType->name());
    list.append(string);

    return new LogLine(mLogLineInternalIdGenerator++, mCurrentDateTime, list, originalFile.url().toLocalFile(), logLineType, mLogMode);
}

void XorgAnalyzer::initializeTypeName()
{
    mXorgLevels[QStringLiteral("(--)")] = new LogLevel(1001, i18n("Probed"), QStringLiteral(PROBED_LOG_LEVEL_ICON), QColor(246, 206, 30), this);

    mXorgLevels[QStringLiteral("(**)")] = new LogLevel(1002, i18n("From config file"), QStringLiteral(CONFIG_FILE_LOG_LEVEL_ICON), QColor(161, 133, 240), this);

    mXorgLevels[QStringLiteral("(==)")] =
        new LogLevel(1003, i18n("Default setting"), QStringLiteral(DEFAULT_SETTING_LOG_LEVEL_ICON), QColor(169, 189, 165), this);

    mXorgLevels[QStringLiteral("(++)")] =
        new LogLevel(1004, i18n("From command Line"), QStringLiteral(COMMAND_LINE_LOG_LEVEL_ICON), QColor(179, 181, 214), this);

    mXorgLevels[QStringLiteral("(!!)")] = Globals::instance().noticeLogLevel();
    mXorgLevels[QStringLiteral("(II)")] = Globals::instance().informationLogLevel();
    mXorgLevels[QStringLiteral("(WW)")] = Globals::instance().warningLogLevel();
    mXorgLevels[QStringLiteral("(EE)")] = Globals::instance().errorLogLevel();

    mXorgLevels[QStringLiteral("(NI)")] =
        new LogLevel(1005, i18n("Not implemented"), QStringLiteral(NOT_IMPLEMENTED_LOG_LEVEL_ICON), QColor(136, 146, 240), this);

    mXorgLevels[QStringLiteral("(\?\?)")] = Globals::instance().noLogLevel();
}

LogLevel *XorgAnalyzer::findTypeName(const QString &type)
{
    QMap<QString, LogLevel *>::iterator it;

    it = mXorgLevels.find(type);
    if (it != mXorgLevels.end()) {
        return *it;
    } else {
        return nullptr;
    }
}
