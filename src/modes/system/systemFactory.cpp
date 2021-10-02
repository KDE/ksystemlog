/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#include "systemFactory.h"

#include "logMode.h"
#include "logging.h"

#include "simpleAction.h"
#include "systemLogMode.h"

QList<LogMode *> SystemLogModeFactory::createLogModes() const
{
    const QList<LogMode *> logModes{new SystemLogMode()};
    return logModes;
}

LogModeAction *SystemLogModeFactory::createLogModeAction() const
{
    LogMode *logMode = Globals::instance().findLogMode(QStringLiteral(SYSTEM_LOG_MODE_ID));

    if (!logMode->filesExist()) {
        return nullptr;
    }

    auto logModeAction = new SimpleAction(logMode->action(), logMode);

    return logModeAction;
}
