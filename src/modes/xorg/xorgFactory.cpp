/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "xorgFactory.h"

#include "logMode.h"
#include "logging.h"

#include "simpleAction.h"
#include "xorgLogMode.h"

QList<LogMode *> XorgLogModeFactory::createLogModes() const
{
    const QList<LogMode *> logModes{new XorgLogMode()};
    return logModes;
}

LogModeAction *XorgLogModeFactory::createLogModeAction() const
{
    LogMode *logMode = Globals::instance().findLogMode(QStringLiteral(XORG_LOG_MODE_ID));

    if (!logMode->filesExist()) {
        return nullptr;
    }

    auto logModeAction = new SimpleAction(logMode->action(), logMode);

    logModeAction->setCategory(LogModeAction::ServicesCategory);

    return logModeAction;
}
