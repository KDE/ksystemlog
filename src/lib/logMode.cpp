/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "logMode.h"

#include <QAction>
#include <QFileInfo>

#include "logModeItemBuilder.h"
#include "multipleActions.h"

LogMode::LogMode(const QString &id, const QString &name, const QString &iconName)
    : d(new LogModePrivate())
{
    d->id = id;
    d->name = name;
    d->icon = QIcon::fromTheme(iconName);
    d->logFilesExist = true;
}

LogMode::~LogMode()
{
    delete d->action;

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

QIcon LogMode::icon() const
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
    return d->logModeConfiguration.data();
}

QAction *LogMode::createDefaultAction()
{
    auto action = new QAction(d->icon, d->name, this);
    ActionData data;
    data.id = d->id;
    action->setData(QVariant::fromValue(data));

    return action;
}

void LogMode::checkLogFilesPresence(const QStringList &paths)
{
    d->logFilesExist = false;
    for (const QString &path : paths) {
        QFileInfo const fileInfo(path);
        if (fileInfo.exists()) {
            d->logFilesExist = true;
        }
    }
}

#include "moc_logMode.cpp"
