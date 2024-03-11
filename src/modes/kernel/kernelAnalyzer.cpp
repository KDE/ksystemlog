/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kernelAnalyzer.h"
#include <QFile>
#include <QRegularExpression>

KernelAnalyzer::KernelAnalyzer(LogMode *logMode)
    : FileAnalyzer(logMode)
{
    startupTime();
}

LogViewColumns KernelAnalyzer::initColumns()
{
    LogViewColumns columns;
    columns.addColumn(LogViewColumn(i18n("Date"), true, false));
    columns.addColumn(LogViewColumn(i18n("Component"), true, false));
    columns.addColumn(LogViewColumn(i18n("Message"), true, false));

    return columns;
}

LogFileReader *KernelAnalyzer::createLogFileReader(const LogFile &logFile)
{
    return new ProcessOutputLogFileReader(logFile);
}

void KernelAnalyzer::startupTime()
{
    QFile file(QStringLiteral(UPTIME_FILE));

    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream in(&file);
    const QString line = in.readLine();

    // Format : 1618.72 1382.98 (uptime / something)
    const QStringList times = line.split(QLatin1Char(' '));

    const QString secondsString = times.at(0);
    const QString pureSecondsString = secondsString.left(secondsString.indexOf(QLatin1Char('.')));
    const long updateSeconds = pureSecondsString.toLong();

    mStartupDateTime = QDateTime::currentDateTime().addSecs(-updateSeconds);
    qCDebug(KSYSTEMLOG) << "Startup time : " << mStartupDateTime;
}

LogLine *KernelAnalyzer::parseMessage(const QString &logLine, const LogFile &originalLogFile)
{
    const QRegularExpression timeRegex(QStringLiteral("\\[\\ *(\\d*)\\.(\\d*)\\]\\s+(.*)"));

    //			QRegExp componentRegexp(timeRegex + "([^\\s:]{,20})[:\\s\\t]+(.*)");
    //			QRegExp messageRegexp(timeRegex + "(.*)");

    QDateTime dateTime(mStartupDateTime);
    QStringList messages;

    auto const timeMatch = timeRegex.match(logLine);

    // If we have the date, we are able to update the start date
    if (timeMatch.hasMatch()) {
        // qCDebug(KSYSTEMLOG) << componentRegexp.cap(1).toInt() << "and" << componentRegexp.cap(2).toInt();
        dateTime = dateTime.addSecs(timeMatch.capturedView(1).toInt());
        dateTime = dateTime.addMSecs(timeMatch.capturedView(2).toInt() / 1000);

        parseComponentMessage(timeMatch.captured(3), messages);
    }
    // Else, the date will never change
    else {
        parseComponentMessage(logLine, messages);
    }

    /*
  qCDebug(KSYSTEMLOG) << "--------------------------------";
  qCDebug(KSYSTEMLOG) << logLine;
  qCDebug(KSYSTEMLOG) << "Secs : " << dateTime.time().second();
  qCDebug(KSYSTEMLOG) << "MSec : " << dateTime.time().msec();
  qCDebug(KSYSTEMLOG) << "Comp : " << messages.at(0);
  qCDebug(KSYSTEMLOG) << "Msg  : " << messages.at(1);
  qCDebug(KSYSTEMLOG) << "--------------------------------";
        */

    auto line = new LogLine(mLogLineInternalIdGenerator++,
                            dateTime,
                            messages,
                            originalLogFile.url().toLocalFile(),
                            Globals::instance().informationLogLevel(),
                            mLogMode);

    return line;
}

#include "moc_kernelAnalyzer.cpp"
