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
