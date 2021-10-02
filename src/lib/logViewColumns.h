/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QList>

class LogViewColumn;
class QStringList;

class LogViewColumns
{
public:
    LogViewColumns();
    LogViewColumns(const LogViewColumns &columns);

    ~LogViewColumns();

    bool isGroupByLogLevel() const;
    bool isGroupByDay() const;
    bool isGroupByHour() const;
    bool isGroupByLogFile() const;

    void setGroupByLogLevel(bool value);
    void setGroupByDay(bool value);
    void setGroupByHour(bool value);
    void setGroupByLogFile(bool value);

    QVector<LogViewColumn> columns() const;

    void addColumn(const LogViewColumn &column);
    QStringList toStringList() const;

    LogViewColumns &operator=(const LogViewColumns &columns);

private:
    QVector<LogViewColumn> mColumns;
    bool mGroupByLogLevel = true;
    bool mGroupByDay = true;
    bool mGroupByHour = true;
    bool mGroupByLogFile = true;
};

QDataStream &operator<<(QDataStream &out, const LogViewColumns &columns);
QDebug &operator<<(QDebug &out, const LogViewColumns &columns);

