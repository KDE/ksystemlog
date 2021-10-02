/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "daemonFactory.h"

#include "logMode.h"
#include "logging.h"

#include "daemonLogMode.h"
#include "simpleAction.h"

QList<LogMode *> DaemonLogModeFactory::createLogModes() const
{
    const QList<LogMode *> logModes{new DaemonLogMode()};
    return logModes;
}

LogModeAction *DaemonLogModeFactory::createLogModeAction() const
{
    LogMode *logMode = Globals::instance().findLogMode(QStringLiteral(DAEMON_LOG_MODE_ID));

    if (!logMode->filesExist()) {
        return nullptr;
    }

    auto logModeAction = new SimpleAction(logMode->action(), logMode);

    return logModeAction;
}
