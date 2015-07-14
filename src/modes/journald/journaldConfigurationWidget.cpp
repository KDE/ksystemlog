/***************************************************************************
 *   KApacheLog, a apache log viewer tool                                  *
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

#include "journaldConfigurationWidget.h"
#include "journaldConfiguration.h"
#include "globals.h"

#include <KLocalizedString>

JournaldConfigurationWidget::JournaldConfigurationWidget()
    : LogModeConfigurationWidget(i18n("Journald Log"), QLatin1String(JOURNALD_MODE_ICON),
                                 i18n("Journald Log"))
{
    setupUi(this);

    connect(lastBootOnly, SIGNAL(stateChanged(int)), SIGNAL(configurationChanged()));
    connect(currentUserEntries, SIGNAL(stateChanged(int)), SIGNAL(configurationChanged()));
    connect(systemEntries, SIGNAL(stateChanged(int)), SIGNAL(configurationChanged()));
}

void JournaldConfigurationWidget::saveConfig()
{
    JournaldConfiguration *configuration = Globals::instance()
                                               .findLogMode(QLatin1String(JOURNALD_LOG_MODE_ID))
                                               ->logModeConfiguration<JournaldConfiguration *>();

    configuration->setDisplayCurrentBootOnly(lastBootOnly->isChecked());
    configuration->setDisplayCurrentUserProcesses(currentUserEntries->isChecked());
    configuration->setDisplaySystemServices(systemEntries->isChecked());
}

void JournaldConfigurationWidget::readConfig()
{
    JournaldConfiguration *configuration = Globals::instance()
                                               .findLogMode(QLatin1String(JOURNALD_LOG_MODE_ID))
                                               ->logModeConfiguration<JournaldConfiguration *>();

    lastBootOnly->setChecked(configuration->displayCurrentBootOnly());
    currentUserEntries->setChecked(configuration->displayCurrentUserProcesses());
    systemEntries->setChecked(configuration->displaySystemServices());
}

void JournaldConfigurationWidget::defaultConfig()
{
    readConfig();
}
