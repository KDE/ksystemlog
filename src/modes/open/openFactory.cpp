/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "openFactory.h"

#include "defaults.h"
#include "ksystemlogConfig.h"
#include "ksystemlog_debug.h"
#include "logFile.h"
#include "logMode.h"

#include "logModeItemBuilder.h"
#include "openLogMode.h"

OpenLogModeFactory::OpenLogModeFactory(QWidget *parent)
    : mParent(parent)
{
}

OpenLogModeFactory::~OpenLogModeFactory()
{
}

QList<LogMode *> OpenLogModeFactory::createLogModes() const
{
    const QList<LogMode *> logModes{new OpenLogMode(mParent)};
    return logModes;
}

LogModeAction *OpenLogModeFactory::createLogModeAction() const
{
    return nullptr;
}

#include "moc_openFactory.cpp"
