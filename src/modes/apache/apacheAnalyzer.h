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

#ifndef _APACHE_ANALYZER_H_
#define _APACHE_ANALYZER_H_

#include <KLocalizedString>

#include "fileAnalyzer.h"

#include "logging.h"

#include "localLogFileReader.h"
#include "apacheLogMode.h"
#include "parsingHelper.h"

class ApacheAnalyzer : public FileAnalyzer
{
    Q_OBJECT

public:
    explicit ApacheAnalyzer(LogMode *logMode)
        : FileAnalyzer(logMode)
    {
        initializeTypeLevels();
    }

    virtual ~ApacheAnalyzer() {}

    LogViewColumns initColumns() override
    {
        LogViewColumns columns;
        columns.addColumn(LogViewColumn(i18n("Date"), true, false));
        columns.addColumn(LogViewColumn(i18n("Client"), true, false));
        columns.addColumn(LogViewColumn(i18n("Message"), true, false));

        return columns;
    }

protected:
    LogFileReader *createLogFileReader(const LogFile &logFile) override { return new LocalLogFileReader(logFile); }

    Analyzer::LogFileSortMode logFileSortMode() override { return Analyzer::AscendingSortedLogFile; }

    /*
     * Log line examples :
     * [Wed May 18 22:16:02 2005] [error] [client 127.0.0.1] File does not exist:
     * /var/www/html/ksystemlog/screenshots/small/kernel-view.png, referer:
     * http://localhost.localdomain/ksystemlog/screenshots.php
     * [Wed May 18 22:16:02 2005] [error] [client 127.0.0.1] File does not exist:
     * /var/www/html/ksystemlog/screenshots/small/system-filter.png, referer:
     * http://localhost.localdomain/ksystemlog/screenshots.php
     * [Thu May 19 18:00:19 2005] [notice] mod_jk2.post_config() first invocation
     * [Thu May 19 18:00:19 2005] [notice] Digest: generating secret for digest authentication ...
     * [client 127.0.0.1] PHP Parse error:  parse error, unexpected T_PRIVATE, expecting T_STRING in
     * /mnt/boulot/web/annivernet/src/fonctions/formulaire.inc.php on line 25
     */
    LogLine *parseMessage(const QString &logLine, const LogFile &originalLogFile) override
    {
        QString line(logLine);

        QDate date;
        QTime time;

        QString level;

        // Temporary variable
        int squareBracket;

        // Special case which sometimes happens
        if (line.indexOf(QLatin1String("[client")) == 0) {
            date = QDate::currentDate();
            time = QTime::currentTime();
            level = QStringLiteral("notice");
        } else {
            // The Date
            int dateBegin = line.indexOf(QLatin1String("["));
            int dateEnd = line.indexOf(QLatin1String("]"));

            QString type;
            QString message;

            QString strDate = line.mid(dateBegin + 1, dateEnd - dateBegin - 1);

            QString month = strDate.mid(4, 3);

            QString day = strDate.mid(8, 2);

            QString hour = strDate.mid(11, 2);
            QString min = strDate.mid(14, 2);
            QString sec = strDate.mid(17, 2);

            QString year = strDate.mid(20, 4);

            date = QDate(year.toInt(), ParsingHelper::instance()->parseSyslogMonth(month), day.toInt());
            time = QTime(hour.toInt(), min.toInt(), sec.toInt());

            line = line.remove(0, dateEnd + 3);

            // The log level
            squareBracket = line.indexOf(QLatin1String("]"));
            level = line.left(squareBracket);
            line = line.remove(0, squareBracket + 2);
        }

        // The client
        int beginSquareBracket = line.indexOf(QLatin1String("[client"));
        squareBracket = line.indexOf(QLatin1String("]"));
        QString client;
        if (beginSquareBracket == -1 || squareBracket == -1) {
            client = QLatin1String("");
        } else {
            client = line.mid(8, squareBracket - 8); // 8=strlen("[client ")
            line = line.remove(0, squareBracket + 2);
        }

        QStringList list;
        list.append(client);
        list.append(line);

        return new LogLine(logLineInternalIdGenerator++, QDateTime(date, time), list,
                           originalLogFile.url().path(), findLogLevel(level), logMode);
    }

private:
    QMap<QString, LogLevel *> mapTypeLevels;

    void initializeTypeLevels()
    {
        mapTypeLevels[QStringLiteral("notice")] = Globals::instance().informationLogLevel();
        mapTypeLevels[QStringLiteral("warn")] = Globals::instance().warningLogLevel();
        mapTypeLevels[QStringLiteral("error")] = Globals::instance().errorLogLevel();
    }

    LogLevel *findLogLevel(const QString &type)
    {
        QMap<QString, LogLevel *>::iterator it;

        it = mapTypeLevels.find(type);
        if (it != mapTypeLevels.end()) {
            return (*it);
        } else {
            logCritical()
                << "New Log Level detected: Please send this log file to the KSystemLog developer to add it ("
                << type << ")";
            return Globals::instance().noLogLevel();
        }
    }
};

#endif // _APACHE_ANALYZER_H_
