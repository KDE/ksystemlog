/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "acpidFactory.h"

#include "logMode.h"
#include "logging.h"

#include "acpidLogMode.h"
#include "simpleAction.h"

QList<LogMode *> AcpidLogModeFactory::createLogModes() const
{
    const QList<LogMode *> logModes{new AcpidLogMode()};
    return logModes;
}

LogModeAction *AcpidLogModeFactory::createLogModeAction() const
{
    LogMode *logMode = Globals::instance().findLogMode(QStringLiteral(ACPID_LOG_MODE_ID));

    if (!logMode->filesExist()) {
        return nullptr;
    }

    auto logModeAction = new SimpleAction(logMode->action(), logMode);
    logModeAction->setCategory(LogModeAction::OthersCategory);

    return logModeAction;
}
