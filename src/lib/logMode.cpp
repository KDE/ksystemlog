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

#include "logMode.h"

#include <QAction>
#include <QFileInfo>

#include <kiconloader.h>

#include "multipleActions.h"
#include "logModeItemBuilder.h"
#include "analyzer.h"

LogMode::LogMode(const QString &id, const QString &name, const QString &iconName)
    : d(new LogModePrivate())
{
    d->id = id;
    d->name = name;
    d->icon = SmallIcon(iconName);
    d->logFilesExist = true;
}

LogMode::~LogMode()
{
    if (d->action)
        delete d->action;

    if (d->itemBuilder)
        delete d->itemBuilder;

    delete d;
}

QString LogMode::id() const
{
    return d->id;
}

QString LogMode::name() const
{
    return d->name;
}

QPixmap LogMode::icon() const
{
    return d->icon;
}

QAction *LogMode::action() const
{
    return d->action;
}

LogModeItemBuilder *LogMode::itemBuilder() const
{
    return d->itemBuilder;
}

Analyzer *LogMode::analyzer()
{
    return d->analyzer;
}

bool LogMode::filesExist() const
{
    return d->logFilesExist;
}

LogModeConfigurationWidget *LogMode::logModeConfigurationWidget() const
{
    return d->logModeConfigurationWidget;
}

LogModeConfiguration *LogMode::innerConfiguration() const
{
    return d->logModeConfiguration;
}

QAction *LogMode::createDefaultAction()
{
    QAction *action = new QAction(d->icon, d->name, this);
    action->setData(QVariant(d->id));

    return action;
}

void LogMode::checkLogFilesPresence(const QStringList &paths)
{
    d->logFilesExist = false;
    for (const QString &path : paths) {
        QFileInfo fileInfo(path);
        if (fileInfo.exists())
            d->logFilesExist = true;
    }
}
