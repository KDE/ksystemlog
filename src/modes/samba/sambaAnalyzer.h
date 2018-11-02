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

#ifndef _SAMBA_ANALYZER_H_
#define _SAMBA_ANALYZER_H_

#include <KLocalizedString>

#include "fileAnalyzer.h"

#include "logging.h"

#include "localLogFileReader.h"
#include "sambaLogMode.h"
#include "parsingHelper.h"

class SambaAnalyzer : public FileAnalyzer
{
    Q_OBJECT

public:
    explicit SambaAnalyzer(LogMode *logMode)
        : FileAnalyzer(logMode)
    {
        currentLogLine = NULL;
    }

    virtual ~SambaAnalyzer() {}

    LogViewColumns initColumns() override
    {
        LogViewColumns columns;

        columns.addColumn(LogViewColumn(i18n("Date"), true, false));
        columns.addColumn(LogViewColumn(i18n("Source File"), true, true));
        columns.addColumn(LogViewColumn(i18n("Function"), true, true));
        columns.addColumn(LogViewColumn(i18n("Line"), true, true));
        columns.addColumn(LogViewColumn(i18n("Message"), true, false));

        return columns;
    }

protected:
    LogFileReader *createLogFileReader(const LogFile &logFile) override { return new LocalLogFileReader(logFile); }

    Analyzer::LogFileSortMode logFileSortMode() override { return Analyzer::AscendingSortedLogFile; }

    /*
     * Log line examples :
     * [2005/06/27 21:06:01, 0] nmbd/nmbd.c:main(668)
     * Netbios nameserver version 3.0.14a started.
     * Copyright Andrew Tridgell and the Samba Team 1994-2004
     * [2005/06/27 21:11:46, 0] nmbd/nmbd_become_lmb.c:become_local_master_stage2(396)
     *  *****
     *  Samba name server STEAKHACHE is now a local master browser for workgroup MAISON on subnet 192.168.1.33
     *
     *  *****
     * [2005/06/28 06:41:03, 0] nmbd/nmbd.c:terminate(56)
     * Got SIGTERM: going down...
     * [2005/06/28 18:08:11, 0] nmbd/nmbd.c:main(668)
     * Netbios nameserver version 3.0.14a started.
     * Copyright Andrew Tridgell and the Samba Team 1994-2004
     *
     * Note:
     * This analyzer nevers return the last line of a log file because it's never sure
     * that the last file line is the last message of the current log line.
     * So the previous last line will be returned at the next file update,
     */
    LogLine *parseMessage(const QString &logLine, const LogFile &originalLogFile) override
    {
        QString line(logLine);

        // The Date
        int dateBegin = line.indexOf(QLatin1String("["));
        int dateEnd = line.indexOf(QLatin1String("]"));

        if (dateBegin != -1) {
            QString strDate = line.mid(dateBegin + 1, dateEnd - dateBegin - 1);

            QString year = strDate.mid(0, 4);
            QString month = strDate.mid(5, 2);
            QString day = strDate.mid(8, 2);

            QString hour = strDate.mid(11, 2);
            QString min = strDate.mid(14, 2);
            QString sec = strDate.mid(17, 2);

            QDate date = QDate(year.toInt(), month.toInt(), day.toInt());
            QTime time = QTime(hour.toInt(), min.toInt(), sec.toInt());

            line = line.remove(0, dateEnd + 2);

            // The source file
            int doubleDot;
            doubleDot = line.indexOf(QLatin1String(":"));
            QString file = line.left(doubleDot);
            line = line.remove(0, doubleDot + 1);

            // The function
            int bracket = line.indexOf(QLatin1String("("));
            QString function = line.left(bracket);
            line = line.remove(0, bracket + 1);

            // The line number
            bracket = line.indexOf(QLatin1String(")"));
            QString lineNumber = line.left(bracket);

            // Remove the first return character and the two useless space of the first message line
            line = line.remove(0, bracket + 4);

            QStringList list;
            list.append(file);
            list.append(function);
            list.append(lineNumber);

            logDebug() << "Creating new line " << endl;

            LogLine *returnedLogLine = currentLogLine;

            currentLogLine = new LogLine(logLineInternalIdGenerator++, QDateTime(date, time), list,
                                         originalLogFile.url().path(),
                                         Globals::instance().informationLogLevel(), logMode);

            return returnedLogLine;
        }

        if (line.indexOf(QLatin1String("  ")) != -1) {
            if (currentLogLine != NULL) {
                QStringList list = currentLogLine->logItems();

                // A line has already been added
                if (list.count() == 4) {
                    QString currentMessage = list.takeLast();
                    list.append(currentMessage + QLatin1String("\n") + line.simplified());
                }
                // First time we add a line for the current Log line
                else {
                    list.append(line.simplified());
                }

                currentLogLine->setLogItems(list);
            }
        }

        return NULL;
    }

    LogLine *currentLogLine;
};

#endif // _SAMBA_ANALYZER_H_
