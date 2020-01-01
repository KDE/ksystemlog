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
#include <QIcon>
class LogLevelPrivate
{
public:
    int id;
    QString name;

    QColor color;

    QIcon icon;
};

LogLevel::LogLevel(int id, const QString &nm, const QString &ic, const QColor &col, QObject *parent)
    : QObject(parent), d(new LogLevelPrivate())
{
    d->id = id;
    d->name = nm;
    d->color = col;
    d->icon = QIcon::fromTheme(ic);
}

LogLevel::~LogLevel()
{
    delete d;
}

int LogLevel::id() const
{
    return d->id;
}

QString LogLevel::name() const
{
    return d->name;
}

QColor LogLevel::color() const
{
    return d->color;
}

QIcon LogLevel::icon() const
{
    return d->icon;
}
