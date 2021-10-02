/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "logViewColumns.h"

#include <QStringList>

#include "logging.h"

#include "globals.h"
#include "logViewColumn.h"

LogViewColumns::LogViewColumns()
{
}

LogViewColumns::LogViewColumns(const LogViewColumns &columns)
{
    mColumns = columns.columns();
    mGroupByLogLevel = columns.isGroupByLogLevel();
    mGroupByDay = columns.isGroupByDay();
    mGroupByHour = columns.isGroupByHour();
    mGroupByLogFile = columns.isGroupByLogFile();
}

LogViewColumns::~LogViewColumns()
{
}

void LogViewColumns::setGroupByLogLevel(bool value)
{
    mGroupByLogLevel = value;
}

void LogViewColumns::setGroupByDay(bool value)
{
    mGroupByDay = value;
}

void LogViewColumns::setGroupByHour(bool value)
{
    mGroupByHour = value;
}

void LogViewColumns::setGroupByLogFile(bool value)
{
    mGroupByLogFile = value;
}

bool LogViewColumns::isGroupByLogLevel() const
{
    return mGroupByLogLevel;
}

bool LogViewColumns::isGroupByDay() const
{
    return mGroupByDay;
}

bool LogViewColumns::isGroupByHour() const
{
    return mGroupByHour;
}

bool LogViewColumns::isGroupByLogFile() const
{
    return mGroupByLogFile;
}

QStringList LogViewColumns::toStringList() const
{
    QStringList columnNames;
    columnNames.reserve(mColumns.count());

    for (const LogViewColumn &column : std::as_const(mColumns)) {
        columnNames.append(column.columnName());
    }

    return columnNames;
}

void LogViewColumns::addColumn(const LogViewColumn &column)
{
    mColumns.append(column);
}

QVector<LogViewColumn> LogViewColumns::columns() const
{
    return mColumns;
}

LogViewColumns &LogViewColumns::operator=(const LogViewColumns &columns)
{
    mColumns = columns.columns();
    mGroupByLogLevel = columns.isGroupByLogLevel();
    mGroupByDay = columns.isGroupByDay();
    mGroupByHour = columns.isGroupByHour();
    mGroupByLogFile = columns.isGroupByLogFile();

    return *this;
}

QDataStream &operator<<(QDataStream &out, const LogViewColumns &columns)
{
    out << columns.columns();
    return out;
}

QDebug &operator<<(QDebug &out, const LogViewColumns &columns)
{
    out << columns.columns();
    return out;
}
