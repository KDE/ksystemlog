/***************************************************************************
 *   KSambaLog, a samba log viewer tool                                  *
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

#include "sambaFactory.h"

#include <KLocalizedString>

#include "logMode.h"
#include "logging.h"
#include "multipleActions.h"

#include "netbiosLogMode.h"
#include "sambaAccessLogMode.h"
#include "sambaLogMode.h"

#include "sambaConfiguration.h"
#include "sambaConfigurationWidget.h"
#include "sambaItemBuilder.h"

QList<LogMode *> SambaLogModeFactory::createLogModes() const
{
    // Create the shared configuration and configuration widget between the logModes
    QSharedPointer<SambaConfiguration> configuration = QSharedPointer<SambaConfiguration>(new SambaConfiguration());
    auto configurationWidget = new SambaConfigurationWidget();

    QList<LogMode *> logModes;
    logModes.append(new SambaLogMode(configuration, configurationWidget, new SambaItemBuilder()));
    logModes.append(new SambaAccessLogMode(configuration, configurationWidget, new SambaItemBuilder()));
    logModes.append(new NetbiosLogMode(configuration, configurationWidget, new SambaItemBuilder()));

    return logModes;
}

LogModeAction *SambaLogModeFactory::createLogModeAction() const
{
    LogMode *sambaLogMode = Globals::instance().findLogMode(QStringLiteral(SAMBA_LOG_MODE_ID));
    LogMode *sambaAccessLogMode = Globals::instance().findLogMode(QStringLiteral(SAMBA_ACCESS_LOG_MODE_ID));
    LogMode *sambaNetbiosLogMode = Globals::instance().findLogMode(QStringLiteral(NETBIOS_LOG_MODE_ID));

    const bool sambaLogsExist = sambaLogMode->filesExist();
    const bool sambaAccessLogsExist = sambaAccessLogMode->filesExist();
    const bool sambaNetbiosLogsExist = sambaNetbiosLogMode->filesExist();

    if (!sambaLogsExist && !sambaAccessLogsExist && !sambaNetbiosLogsExist) {
        return nullptr;
    }

    auto multipleActions = new MultipleActions(QIcon::fromTheme(QStringLiteral(SAMBA_MODE_ICON)), i18n("Samba"), sambaLogMode);

    if (sambaLogsExist) {
        multipleActions->addInnerAction(sambaLogMode->action());
    }

    if (sambaAccessLogsExist) {
        multipleActions->addInnerAction(sambaAccessLogMode->action());
    }

    if (sambaNetbiosLogsExist) {
        multipleActions->addInnerAction(sambaNetbiosLogMode->action());
    }

    multipleActions->setCategory(LogModeAction::ServicesCategory);

    return multipleActions;
}
