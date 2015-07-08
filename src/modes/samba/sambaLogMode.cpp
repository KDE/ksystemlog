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

#include "sambaLogMode.h"

#include <QList>

#include <KLocalizedString>

#include "logging.h"
#include "logMode.h"

#include "sambaAnalyzer.h"
#include "sambaItemBuilder.h"
#include "sambaConfigurationWidget.h"
#include "sambaConfiguration.h"

SambaLogMode::SambaLogMode(SambaConfiguration *sambaConfiguration,
                           SambaConfigurationWidget *sambaConfigurationWidget, SambaItemBuilder *itemBuilder)
    : LogMode(QLatin1String(SAMBA_LOG_MODE_ID), i18n("Samba Log"), QLatin1String(SAMBA_MODE_ICON))
{
    d->logModeConfiguration = sambaConfiguration;
    d->logModeConfigurationWidget = sambaConfigurationWidget;
    d->itemBuilder = itemBuilder;

    // Samba Log Action
    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the Samba log."));
    d->action->setWhatsThis(i18n(
        "Displays the Samba log in the current tab. Samba is the file sharing server which interacts with "
        "Microsoft Windows network."));

    checkLogFilesPresence(sambaConfiguration->sambaPaths());
}

SambaLogMode::~SambaLogMode()
{
}

Analyzer *SambaLogMode::createAnalyzer()
{
    return new SambaAnalyzer(this);
}

QList<LogFile> SambaLogMode::createLogFiles()
{
    SambaConfiguration *sambaConfiguration = logModeConfiguration<SambaConfiguration *>();
    return sambaConfiguration->findNoModeLogFiles(sambaConfiguration->sambaPaths());
}
