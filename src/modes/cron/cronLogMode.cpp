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

#include "cronLogMode.h"

#include <QList>

#include <KLocalizedString>

#include "logging.h"
#include "logMode.h"

#include "cronAnalyzer.h"
#include "cronConfigurationWidget.h"
#include "cronConfiguration.h"
#include "cronItemBuilder.h"

CronLogMode::CronLogMode()
    : LogMode(QLatin1String(CRON_LOG_MODE_ID), i18n("Cron Log"), QLatin1String(CRON_MODE_ICON))
{
    d->logModeConfiguration = new CronConfiguration();

    d->logModeConfigurationWidget = new CronConfigurationWidget();

    d->itemBuilder = new CronItemBuilder();

    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the planned tasks log (Cron log)."));
    d->action->setWhatsThis(i18n(
        "Displays the planned tasks log in the current tab. Cron process is a program in charge of launching "
        "planned tasks on your system, like security checks, or auto-restarting of some services. Use this "
        "menu to see the recently launched processes."));

    CronConfiguration *cronConfiguration = logModeConfiguration<CronConfiguration *>();
    checkLogFilesPresence(cronConfiguration->cronPaths());
}

CronLogMode::~CronLogMode()
{
}

Analyzer *CronLogMode::createAnalyzer()
{
    return new CronAnalyzer(this);
}

QList<LogFile> CronLogMode::createLogFiles()
{
    CronConfiguration *cronConfiguration = logModeConfiguration<CronConfiguration *>();
    return cronConfiguration->findNoModeLogFiles(cronConfiguration->cronPaths());
}
