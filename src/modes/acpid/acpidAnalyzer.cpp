/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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

        // qCDebug(KSYSTEMLOG) << "Date=" << date.toString();
        // qCDebug(KSYSTEMLOG) << "Time=" << time.toString();

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

    return new LogLine(mLogLineInternalIdGenerator++,
                       QDateTime(date, time),
                       list,
                       originalFile.url().toLocalFile(),
                       Globals::instance().informationLogLevel(),
                       mLogMode);
}

#include "moc_acpidAnalyzer.cpp"
