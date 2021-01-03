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

#ifndef POSTFIX_ANALYZER_H
#define POSTFIX_ANALYZER_H

#include "syslogAnalyzer.h"

#include "logMode.h"
#include "logging.h"

class PostfixAnalyzer : public SyslogAnalyzer
{
    Q_OBJECT

public:
    explicit PostfixAnalyzer(LogMode *logMode)
        : SyslogAnalyzer(logMode)
    {
    }

    ~PostfixAnalyzer() override {}

    /*
     * Just a test of multilines log lines (and it works well !)
     */
    /*
    LogLine* parseMessage(const QString& logLine, const LogFile& originalFile) {
        LogLine* syslogLine = SyslogAnalyzer::parseMessage(logLine, originalFile);

        QStringList items = syslogLine->logItems();
        QString message = items.takeLast();

        items.append(message + "\n" + message);

  logDebug() << "Coucou" << items.at(items.count()-1);

        syslogLine->setLogItems(items);
        return syslogLine;
    }
    */
};

#endif
