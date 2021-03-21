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

#pragma once

#include <QDateTime>

#include <QMap>
#include <QString>

/**
 * TODO Fork this class in SyslogParsingHelper and HttpParsingHelper
 */
class ParsingHelper
{
public:
    static ParsingHelper *instance();

    ~ParsingHelper();

    /**
     * Returns the months number represented by the 3 first letters in the QString parameter
     */
    int parseSyslogMonth(const QString &month);

    QDateTime parseHttpDateTime(const QString &dateTime);

    /*
     * TODO Use this method in SyslogAnalyzer, and add a parameter to use the current year instead
     * of trying to search it in string
     */
    QDateTime parseSyslogDateTime(const QString &dateTime);
    QString syslogDateTimeRegexp() const;

    QString parseSize(const QString &size);

    QString parseHttpResponse(const QString &response);

    QString parseAgent(const QString &agent);

private:
    explicit ParsingHelper();

    static ParsingHelper *self;

    QMap<QString, int> mMapMonths;

    QMap<QString, QString> mMapHTTPResponse;
};

