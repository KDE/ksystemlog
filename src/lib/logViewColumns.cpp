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
