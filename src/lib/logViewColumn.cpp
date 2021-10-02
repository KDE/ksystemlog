/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#include "logViewColumn.h"

LogViewColumn::LogViewColumn(const QString &name, bool filtered, bool grouped)
    : mColumnName(name)
    , mFiltered(filtered)
    , mGrouped(grouped)
{
}

LogViewColumn::LogViewColumn(const LogViewColumn &column)
    : mColumnName(column.columnName())
    , mFiltered(column.isFiltered())
    , mGrouped(column.isGrouped())
{
}

LogViewColumn::~LogViewColumn()
{
}

QString LogViewColumn::columnName() const
{
    return mColumnName;
}

bool LogViewColumn::isGrouped() const
{
    return mGrouped;
}

bool LogViewColumn::isFiltered() const
{
    return mFiltered;
}

LogViewColumn &LogViewColumn::operator=(const LogViewColumn &column)
{
    mColumnName = column.columnName();
    mGrouped = column.isGrouped();
    mFiltered = column.isFiltered();

    return *this;
}

QDataStream &operator<<(QDataStream &out, const LogViewColumn &column)
{
    out << column.columnName();
    return out;
}

QDebug &operator<<(QDebug &out, const LogViewColumn &column)
{
    out << column.columnName();
    return out;
}
