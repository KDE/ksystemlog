/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#include "postfixFactory.h"

#include "logMode.h"
#include "logging.h"

#include "postfixLogMode.h"
#include "simpleAction.h"

QList<LogMode *> PostfixLogModeFactory::createLogModes() const
{
    const QList<LogMode *> logModes{new PostfixLogMode()};
    return logModes;
}

LogModeAction *PostfixLogModeFactory::createLogModeAction() const
{
    LogMode *logMode = Globals::instance().findLogMode(QStringLiteral(POSTFIX_LOG_MODE_ID));

    if (!logMode->filesExist()) {
        return nullptr;
    }

    auto logModeAction = new SimpleAction(logMode->action(), logMode);

    logModeAction->setCategory(LogModeAction::ServicesCategory);

    return logModeAction;
}
