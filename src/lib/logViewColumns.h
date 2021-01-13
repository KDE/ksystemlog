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

#ifndef LOG_VIEW_COLUMNS_H
#define LOG_VIEW_COLUMNS_H

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

#endif
