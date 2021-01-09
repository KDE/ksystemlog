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

#include "logLevel.h"

LogLevel::LogLevel(int id, const QString &nm, const QString &ic, const QColor &col, QObject *parent)
    : QObject(parent)
    , mId(id)
    , mName(nm)
    , mColor(col)
    , mIcon(QIcon::fromTheme(ic))
{
}

LogLevel::~LogLevel()
{
}

int LogLevel::id() const
{
    return mId;
}

QString LogLevel::name() const
{
    return mName;
}

QColor LogLevel::color() const
{
    return mColor;
}

QIcon LogLevel::icon() const
{
    return mIcon;
}
