/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
