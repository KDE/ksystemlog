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

#ifndef _KERNEL_ANALYZER_H_
#define _KERNEL_ANALYZER_H_

#include <QRegExp>
#include <QFile>

#include <KLocalizedString>

#include "logging.h"

#include "fileAnalyzer.h"

#include "localLogFileReader.h"
#include "processOutputLogFileReader.h"
#include "kernelLogMode.h"

class LogMode;

class KernelAnalyzer : public FileAnalyzer
{
    Q_OBJECT

public:
    KernelAnalyzer(LogMode *logMode)
        : FileAnalyzer(logMode)
    {
        startupTime();
    }

    virtual ~KernelAnalyzer() {}

    LogViewColumns initColumns()
    {
        LogViewColumns columns;
        columns.addColumn(LogViewColumn(i18n("Date"), true, false));
        columns.addColumn(LogViewColumn(i18n("Component"), true, false));
        columns.addColumn(LogViewColumn(i18n("Message"), true, false));

        return columns;
    }

protected:
    LogFileReader *createLogFileReader(const LogFile &logFile)
    {
        return new ProcessOutputLogFileReader(logFile);
    }

    Analyzer::LogFileSortMode logFileSortMode() { return Analyzer::AscendingSortedLogFile; }

    void startupTime()
    {
        QFile file(QLatin1String(UPTIME_FILE));

        file.open(QIODevice::ReadOnly | QIODevice::Text);

        QTextStream in(&file);
        QString line = in.readLine();

        // Format : 1618.72 1382.98 (uptime / something)
        QStringList times = line.split(QLatin1String(" "));

        QString secondsString = times.at(0);
        QString pureSecondsString = secondsString.left(secondsString.indexOf(QLatin1String(".")));
        long updateSeconds = pureSecondsString.toLong();

        startupDateTime = QDateTime::currentDateTime().addSecs(-updateSeconds);
        logDebug() << "Startup time : " << startupDateTime;
    }

    LogLine *parseMessage(const QString &logLine, const LogFile &originalLogFile)
    {
        QRegExp timeRegex(QLatin1String("\\[\\ *(\\d*)\\.(\\d*)\\]\\s+(.*)"));

        //			QRegExp componentRegexp(timeRegex + "([^\\s:]{,20})[:\\s\\t]+(.*)");
        //			QRegExp messageRegexp(timeRegex + "(.*)");

        QDateTime dateTime(startupDateTime);
        QStringList messages;

        int timeExists = timeRegex.indexIn(logLine);

        // If we have the date, we are able to update the start date
        if (timeExists != -1) {
            // logDebug() << componentRegexp.cap(1).toInt() << "and" << componentRegexp.cap(2).toInt();
            dateTime = dateTime.addSecs(timeRegex.cap(1).toInt());
            dateTime = dateTime.addMSecs(timeRegex.cap(2).toInt() / 1000);

            parseComponentMessage(timeRegex.cap(3), messages);

        }
        // Else, the date will never change
        else {
            parseComponentMessage(logLine, messages);
        }

        /*
  logDebug() << "--------------------------------";
  logDebug() << logLine;
  logDebug() << "Secs : " << dateTime.time().second();
  logDebug() << "MSec : " << dateTime.time().msec();
  logDebug() << "Comp : " << messages.at(0);
  logDebug() << "Msg  : " << messages.at(1);
  logDebug() << "--------------------------------";
        */

        LogLine *line
            = new LogLine(logLineInternalIdGenerator++, dateTime, messages, originalLogFile.url().path(),
                          Globals::instance().informationLogLevel(), logMode);

        return line;
    }

    inline void parseComponentMessage(const QString &logLine, QStringList &messages)
    {
        QString message(logLine);
        QString component;

        int doublePointPosition = message.indexOf(QLatin1String(":"));

        // Estimate the max size of a component
        if (doublePointPosition != -1 && doublePointPosition < 20) {
            component = message.left(doublePointPosition);
            // Remove component length + ": "
            message = message.remove(0, doublePointPosition + 2);
        }

        messages.append(component);
        messages.append(message.simplified());
    }

protected:
    QDateTime startupDateTime;
};

#endif
