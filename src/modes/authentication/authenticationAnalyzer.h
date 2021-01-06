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

#ifndef AUTHENTICATION_ANALYZER_H
#define AUTHENTICATION_ANALYZER_H

#include "syslogAnalyzer.h"

#include "authenticationLogMode.h"
#include "authenticationConfiguration.h"

class AuthenticationAnalyzer : public SyslogAnalyzer
{
    Q_OBJECT

public:
    explicit AuthenticationAnalyzer(LogMode *logMode);

    ~AuthenticationAnalyzer() override
    {
    }

    LogLine *parseMessage(const QString &logLine, const LogFile &originalLogFile) override;

private:
    bool hasWarningKeywords(const QString &message);

    bool hasErrorKeywords(const QString &message);

    bool hasKeywords(const QString &message, const QStringList &keywords);
};

#endif
