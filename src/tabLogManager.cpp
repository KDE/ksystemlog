/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "tabLogManager.h"

#include <KLocalizedString>

#include "logging.h"

#include "view.h"

#include "defaults.h"
#include "logManager.h"
#include "logMode.h"

TabLogManager::TabLogManager(LogManager *logManager)
    : mLogManager(logManager)
{
}

TabLogManager::~TabLogManager()
{
    // delete mLogManager->usedView();
}

LogManager *TabLogManager::logManager() const
{
    return mLogManager;
}

void TabLogManager::addNewLinesCount(int newLines)
{
    mNewLinesCount += newLines;
}

void TabLogManager::initNewLinesCount()
{
    mNewLinesCount = 0;
}

QString TabLogManager::title() const
{
    if (mNewLinesCount == 0) {
        return logModeName();
    } else {
        return i18nc("Log mode name (added lines count)", "%1 (%2)", mLogManager->title(), mNewLinesCount);
    }
}

QString TabLogManager::logModeName() const
{
    if (!mLogManager->logMode()) {
        return i18nc("Newly created tab", "Empty Log");
    } else {
        return mLogManager->title();
    }
}
