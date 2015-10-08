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

#ifndef _LOG_LEVEL_H_
#define _LOG_LEVEL_H_

#include <QColor>
#include <QString>
#include <QPixmap>

class LogLevelPrivate;

class LogLevel : QObject
{
public:
    explicit LogLevel(int id, const QString &name, const QString &icon, const QColor &color, QObject *parent = nullptr);

    virtual ~LogLevel();

    int id();
    QString name();

    QString icon();

    QColor color();

    QPixmap pixmap();

private:
    LogLevelPrivate *const d;
};

#endif
