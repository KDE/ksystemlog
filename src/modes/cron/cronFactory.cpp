/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "cronFactory.h"

#include "ksystemlog_debug.h"
#include "logMode.h"

#include "cronLogMode.h"
#include "simpleAction.h"

QList<LogMode *> CronLogModeFactory::createLogModes() const
{
    QList<LogMode *> logModes{new CronLogMode()};
    return logModes;
}

LogModeAction *CronLogModeFactory::createLogModeAction() const
{
    LogMode *logMode = Globals::instance().findLogMode(QStringLiteral(CRON_LOG_MODE_ID));

    if (!logMode->filesExist()) {
        return nullptr;
    }

    auto logModeAction = new SimpleAction(logMode->action(), logMode);
    logModeAction->setCategory(LogModeAction::ServicesCategory);

    return logModeAction;
}

#include "moc_cronFactory.cpp"
