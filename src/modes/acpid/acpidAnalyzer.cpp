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

#include "acpidAnalyzer.h"

AcpidAnalyzer::AcpidAnalyzer(LogMode *logMode)
    : FileAnalyzer(logMode)
{
}

LogViewColumns AcpidAnalyzer::initColumns()
{
    LogViewColumns columns;
    columns.addColumn(LogViewColumn(i18n("Date"), true, false));
    columns.addColumn(LogViewColumn(i18n("Type"), true, true));
    columns.addColumn(LogViewColumn(i18n("Message"), true, false));

    return columns;
}

LogLine *AcpidAnalyzer::parseMessage(const QString &logLine, const LogFile &originalFile)
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
        const QString strDate = line.mid(dateBegin + 1, dateEnd - dateBegin - 1);

        const QString month = strDate.mid(4, 3);

        const QString day = strDate.mid(8, 2);

        const QString hour = strDate.mid(11, 2);
        const QString min = strDate.mid(14, 2);
        const QString sec = strDate.mid(17, 2);

        const QString year = strDate.mid(20, 4);

        date = QDate(year.toInt(), ParsingHelper::instance()->parseSyslogMonth(month), day.toInt());
        time = QTime(hour.toInt(), min.toInt(), sec.toInt());

        // logDebug() << "Date=" << date.toString();
        // logDebug() << "Time=" << time.toString();

        line.remove(0, dateEnd + 2);

        int endType = line.indexOf(QLatin1String("\""));

        // If the " character does not exist, it means that there is no Type category
        if (endType == -1) {
            type = QLatin1String(""); // No type
            message = line;
        } else {
            type = line.left(endType - 1);
            line.remove(0, endType + 1);

            message = line.left(line.length() - 2);
        }
    }

    QStringList list;

    list.append(type);
    list.append(message);

    return new LogLine(mLogLineInternalIdGenerator++, QDateTime(date, time), list,
                       originalFile.url().toLocalFile(), Globals::instance().informationLogLevel(), mLogMode);
}
