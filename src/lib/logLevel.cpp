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

#include <kiconloader.h>

class LogLevelPrivate
{
public:
    int id;
    QString name;

    QString icon;

    QColor color;

    QPixmap pixmap;
};

LogLevel::LogLevel(int id, const QString &nm, const QString &ic, const QColor &col, QObject *parent)
    : QObject(parent), d(new LogLevelPrivate())
{
    d->id = id;
    d->name = nm;
    d->icon = ic;
    d->color = col;
    d->pixmap = SmallIcon(ic);
}

LogLevel::~LogLevel()
{
    delete d;
}

int LogLevel::id()
{
    return d->id;
}

QString LogLevel::name()
{
    return d->name;
}

QString LogLevel::icon()
{
    return d->icon;
}

QColor LogLevel::color()
{
    return d->color;
}

QPixmap LogLevel::pixmap()
{
    return d->pixmap;
}
