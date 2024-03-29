/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "syslogAnalyzer.h"

#include <QDateTime>
#include <QStringList>

#include <KLocalizedString>

#include "globals.h"
#include "ksystemlog_debug.h"

#include "localLogFileReader.h"
#include "logLevel.h"
#include "logLine.h"
#include "logMode.h"
#include "logViewWidget.h"

#include "logViewModel.h"

#include "parsingHelper.h"

#include "ksystemlogConfig.h"

SyslogAnalyzer::SyslogAnalyzer(LogMode *logMode)
    : FileAnalyzer(logMode)
{
}

SyslogAnalyzer::~SyslogAnalyzer()
{
}

LogViewColumns SyslogAnalyzer::initColumns()
{
    LogViewColumns columns;
    columns.addColumn(LogViewColumn(i18n("Date"), true, false));
    columns.addColumn(LogViewColumn(i18n("Host"), true, true));
    columns.addColumn(LogViewColumn(i18n("Process"), true, true));
    columns.addColumn(LogViewColumn(i18n("Message"), true, false));

    return columns;
}

LogFileReader *SyslogAnalyzer::createLogFileReader(const LogFile &logFile)
{
    return new LocalLogFileReader(logFile);
}

Analyzer::LogFileSortMode SyslogAnalyzer::logFileSortMode()
{
    return Analyzer::AscendingSortedLogFile;
}

/**
 * TODO Improve speed of this method (with KRegExp class for example)
 */
LogLine *SyslogAnalyzer::parseMessage(const QString &logLine, const LogFile &originalFile)
{
    // qCDebug(KSYSTEMLOG) << QTime::currentTime() << " : Reading line : " << logLine << " from " <<
    // originalFile.url.path();

    // 15 is the default date size format
    if (logLine.length() < 15) {
        qCDebug(KSYSTEMLOG) << "Too short line";
        return undefinedLogLine(logLine, originalFile);
    }

    const int year = QDate::currentDate().year();

    // Month number
    QString const month(logLine.left(3));

    QString line(logLine);

    line.remove(0, 4);
    const int monthNum = ParsingHelper::instance()->parseSyslogMonth(month);

    // Day number
    QString const day(line.left(2));
    const int dayNum = day.toInt();

    line.remove(0, 3);

    // Time
    QString stringTime(line.left(8));
    const int h = QStringView(stringTime).left(2).toInt();
    stringTime.remove(0, 3);
    const int m = QStringView(stringTime).left(2).toInt();
    stringTime.remove(0, 3);
    const int s = QStringView(stringTime).left(2).toInt();
    const QDateTime dateTime(QDate(year, monthNum, dayNum), QTime(h, m, s));
    if (!dateTime.isValid()) {
        qCDebug(KSYSTEMLOG) << "Malformed date and time";
        return undefinedLogLine(logLine, originalFile);
    }

    line.remove(0, 9);

    QString hostname;

    const int nextSpace = line.indexOf(QLatin1Char(' '));
    int nextDoubleDot = line.indexOf(QLatin1Char(':'));

    // Normal case or no process name
    if (nextSpace < nextDoubleDot || nextDoubleDot == -1) {
        // Host name
        hostname = line.left(nextSpace);
        line.remove(0, nextSpace + 1);
    }
    // No host name case (very rare)
    else {
        // Host name
        hostname = undefinedHostName();
    }

    // Refresh double dot once the line has been substr'ed
    nextDoubleDot = line.indexOf(QLatin1Char(':'));

    QString process;
    QString message;

    // Process name
    if (nextDoubleDot != -1) {
        process = line.left(nextDoubleDot);

        // If the delete process identifier option is enabled
        if (KSystemLogConfig::deleteProcessIdentifier()) {
            const int squareBracket = process.indexOf(QLatin1Char('['));

            // If we find a bracket, we remove the useless part
            if (squareBracket != -1) {
                process.truncate(squareBracket);
            }
        }
        line.remove(0, nextDoubleDot + 1);

        message = line.remove(0, 1);
    }
    // If we can't find any ':' character, it means that this line is a
    // internal message of syslogd
    else {
        if (line.contains(QLatin1String("last message repeated")) || line.contains(QLatin1String("-- MARK --"))) {
            process = QStringLiteral("syslog");
        } else {
            process = undefinedProcess();
        }

        message = line;
    }

    QStringList list;
    list.append(hostname);
    list.append(process);
    list.append(message);

    return new LogLine(mLogLineInternalIdGenerator++, dateTime, list, originalFile.url().toLocalFile(), originalFile.defaultLogLevel(), mLogMode);
}

inline LogLine *SyslogAnalyzer::undefinedLogLine(const QString &message, const LogFile &originalFile)
{
    QStringList items;
    items << undefinedHostName() << undefinedProcess() << message;
    return new LogLine(mLogLineInternalIdGenerator++,
                       QDateTime::currentDateTime(),
                       items,
                       originalFile.url().toLocalFile(),
                       originalFile.defaultLogLevel(),
                       mLogMode);
}

inline QString SyslogAnalyzer::undefinedHostName()
{
    // i18nc("Undefined host name", "undefined");
    return QLatin1String("");
}

inline QString SyslogAnalyzer::undefinedProcess()
{
    // i18nc("Undefined process", "undefined");
    return QLatin1String("");
}

#include "moc_syslogAnalyzer.cpp"
