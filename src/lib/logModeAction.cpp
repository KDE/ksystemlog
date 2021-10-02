/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "logModeAction.h"

LogModeAction::LogModeAction(QObject *parent)
    : QObject(parent)
{
}

LogModeAction::~LogModeAction()
{
}

void LogModeAction::setCategory(LogModeAction::Category category)
{
    mCategory = category;
}

LogModeAction::Category LogModeAction::category() const
{
    return mCategory;
}
