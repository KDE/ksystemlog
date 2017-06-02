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

#ifndef _ACPID_ANALYZER_H_
#define _ACPID_ANALYZER_H_

#include <KLocalizedString>

#include "fileAnalyzer.h"

#include "localLogFileReader.h"
#include "parsingHelper.h"
#include "acpidLogMode.h"

class AcpidAnalyzer : public FileAnalyzer
{
    Q_OBJECT

public:
    AcpidAnalyzer(LogMode *logMode)
        : FileAnalyzer(logMode)
    {
    }

    virtual ~AcpidAnalyzer() {}

    LogViewColumns initColumns() Q_DECL_OVERRIDE
    {
        LogViewColumns columns;
        columns.addColumn(LogViewColumn(i18n("Date"), true, false));
        columns.addColumn(LogViewColumn(i18n("Type"), true, true));
        columns.addColumn(LogViewColumn(i18n("Message"), true, false));

        return columns;
    }

protected:
    LogFileReader *createLogFileReader(const LogFile &logFile) Q_DECL_OVERRIDE { return new LocalLogFileReader(logFile); }

    Analyzer::LogFileSortMode logFileSortMode() Q_DECL_OVERRIDE { return Analyzer::AscendingSortedLogFile; }

    LogLine *parseMessage(const QString &logLine, const LogFile &originalFile) Q_DECL_OVERRIDE
    {
        QString line(logLine);

        int dateBegin = line.indexOf(QLatin1String("["));
        int dateEnd = line.indexOf(QLatin1String("]"));

        QString type;
        QString message;
        QDate date;
        QTime time;

        // If there is a format problem in the line
        if (dateBegin == -1 || dateEnd == -1) {
            type = QLatin1String(""); // No type
            message = line;
            date = QDate::currentDate();
            time = QTime::currentTime();
        } else {
            QString strDate = line.mid(dateBegin + 1, dateEnd - dateBegin - 1);

            QString month = strDate.mid(4, 3);

            QString day = strDate.mid(8, 2);

            QString hour = strDate.mid(11, 2);
            QString min = strDate.mid(14, 2);
            QString sec = strDate.mid(17, 2);

            QString year = strDate.mid(20, 4);

            date = QDate(year.toInt(), ParsingHelper::instance()->parseSyslogMonth(month), day.toInt());
            time = QTime(hour.toInt(), min.toInt(), sec.toInt());

            // logDebug() << "Date=" << date.toString();
            // logDebug() << "Time=" << time.toString();

            line = line.remove(0, dateEnd + 2);

            int endType = line.indexOf(QLatin1String("\""));

            // If the " character does not exist, it means that there is no Type category
            if (endType == -1) {
                type = QLatin1String(""); // No type
                message = line;
            } else {
                type = line.left(endType - 1);
                line = line.remove(0, endType + 1);

                message = line.left(line.length() - 2);
            }
        }

        QStringList list;

        list.append(type);
        list.append(message);

        return new LogLine(logLineInternalIdGenerator++, QDateTime(date, time), list,
                           originalFile.url().path(), Globals::instance().informationLogLevel(), logMode);
    }
};

#endif // _ACPID_ANALYZER_H_
