/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kernelFactory.h"

#include "ksystemlog_debug.h"
#include "logMode.h"

#include "kernelLogMode.h"
#include "simpleAction.h"

QList<LogMode *> KernelLogModeFactory::createLogModes() const
{
    const QList<LogMode *> logModes{new KernelLogMode()};
    return logModes;
}

LogModeAction *KernelLogModeFactory::createLogModeAction() const
{
    LogMode *logMode = Globals::instance().findLogMode(QStringLiteral(KERNEL_LOG_MODE_ID));

    if (!logMode->filesExist()) {
        return nullptr;
    }

    auto logModeAction = new SimpleAction(logMode->action(), logMode);

    return logModeAction;
}

#include "moc_kernelFactory.cpp"
