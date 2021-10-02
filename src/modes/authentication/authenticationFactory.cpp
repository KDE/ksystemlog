/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "authenticationFactory.h"

#include "logMode.h"
#include "logging.h"

#include "authenticationLogMode.h"
#include "simpleAction.h"

QList<LogMode *> AuthenticationLogModeFactory::createLogModes() const
{
    const QList<LogMode *> logModes{new AuthenticationLogMode()};
    return logModes;
}

LogModeAction *AuthenticationLogModeFactory::createLogModeAction() const
{
    LogMode *logMode = Globals::instance().findLogMode(QStringLiteral(AUTHENTICATION_LOG_MODE_ID));

    if (!logMode->filesExist()) {
        return nullptr;
    }

    auto logModeAction = new SimpleAction(logMode->action(), logMode);

    return logModeAction;
}
