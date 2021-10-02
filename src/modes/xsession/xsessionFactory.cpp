/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "xsessionFactory.h"

#include "logMode.h"
#include "logging.h"

#include "simpleAction.h"
#include "xsessionLogMode.h"

QList<LogMode *> XSessionLogModeFactory::createLogModes() const
{
    const QList<LogMode *> logModes{new XSessionLogMode()};
    return logModes;
}

LogModeAction *XSessionLogModeFactory::createLogModeAction() const
{
    LogMode *logMode = Globals::instance().findLogMode(QStringLiteral(X_SESSION_LOG_MODE_ID));

    if (!logMode->filesExist()) {
        return nullptr;
    }

    auto logModeAction = new SimpleAction(logMode->action(), logMode);

    logModeAction->setCategory(LogModeAction::OthersCategory);

    return logModeAction;
}
