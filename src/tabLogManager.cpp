/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   Copyright (C) 2007 by Nicolas Ternisien                               *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#include "tabLogManager.h"

#include <KLocalizedString>

#include "logging.h"

#include "view.h"

#include "logMode.h"
#include "defaults.h"
#include "logManager.h"

class TabLogManagerPrivate
{
public:
    LogManager *logManager;
    int newLinesCount;
};

TabLogManager::TabLogManager(LogManager *logManager)
    : d(new TabLogManagerPrivate())
{
    d->logManager = logManager;

    d->newLinesCount = 0;
}

TabLogManager::~TabLogManager()
{
    delete d->logManager->usedView();

    delete d->logManager;

    delete d;
}

LogManager *TabLogManager::logManager()
{
    return d->logManager;
}

void TabLogManager::addNewLinesCount(int newLines)
{
    d->newLinesCount += newLines;
}

void TabLogManager::initNewLinesCount()
{
    d->newLinesCount = 0;
}

QString TabLogManager::title()
{
    if (d->newLinesCount == 0)
        return logModeName();
    else
        return i18nc("Log mode name (added lines count)", "%1 (%2)", d->logManager->title(),
                     d->newLinesCount);
}

QString TabLogManager::logModeName()
{
    if (d->logManager->logMode() == NULL)
        return i18nc("Newly created tab", "Empty Log");
    else
        return d->logManager->title();
}
