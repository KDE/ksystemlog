/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "cupsAnalyzer.h"
#include <KLocalizedString>

CupsAnalyzer::CupsAnalyzer(LogMode *logMode)
    : FileAnalyzer(logMode)
{
    initializeTypeLevels();
}

CupsAnalyzer::~CupsAnalyzer()
{
}

LogViewColumns CupsAnalyzer::initColumns()
{
    LogViewColumns columns;
    columns.addColumn(LogViewColumn(i18n("Date"), true, false));
    columns.addColumn(LogViewColumn(i18n("Message"), true, false));

    return columns;
}

LogFileReader *CupsAnalyzer::createLogFileReader(const LogFile &logFile)
{
    return new LocalLogFileReader(logFile);
}

Analyzer::LogFileSortMode CupsAnalyzer::logFileSortMode()
{
    return Analyzer::AscendingSortedLogFile;
}

LogLine *CupsAnalyzer::parseMessage(const QString &logLine, const LogFile &originalLogFile)
{
    QString line(logLine);

    const QChar level = logLine[0];

    const QDateTime dateTime = ParsingHelper::instance()->parseHttpDateTime(logLine.mid(3, 26));

    const QString message = line.remove(0, 31);

    const QStringList list{message};

    return new LogLine(mLogLineInternalIdGenerator++, dateTime, list, originalLogFile.url().toLocalFile(), findLogLevel(level), mLogMode);
}

void CupsAnalyzer::initializeTypeLevels()
{
    mMapTypeLevels[QLatin1Char('d')] = new LogLevel(20, i18n("debug 2"), QStringLiteral(DEBUG2_LOG_LEVEL_ICON), QColor(169, 189, 165), this);
    mMapTypeLevels[QLatin1Char('D')] = Globals::instance().debugLogLevel();
    mMapTypeLevels[QLatin1Char('I')] = Globals::instance().informationLogLevel();
    mMapTypeLevels[QLatin1Char('N')] = Globals::instance().noticeLogLevel();
    mMapTypeLevels[QLatin1Char('W')] = Globals::instance().warningLogLevel();
    mMapTypeLevels[QLatin1Char('E')] = Globals::instance().errorLogLevel();
    mMapTypeLevels[QLatin1Char('C')] = Globals::instance().criticalLogLevel();
    mMapTypeLevels[QLatin1Char('A')] = Globals::instance().alertLogLevel();
    mMapTypeLevels[QLatin1Char('X')] = Globals::instance().emergencyLogLevel();
    mMapTypeLevels[QLatin1Char(' ')] = Globals::instance().noLogLevel();
}

LogLevel *CupsAnalyzer::findLogLevel(QChar type)
{
    QMap<QChar, LogLevel *>::iterator it;

    it = mMapTypeLevels.find(type);
    if (it != mMapTypeLevels.end()) {
        return *it;
    } else {
        qCCritical(KSYSTEMLOG) << i18n("New Log Level detected: Please send this log file to the KSystemLog developer to add it.");
        return Globals::instance().noLogLevel();
    }
}

#include "moc_cupsAnalyzer.cpp"
