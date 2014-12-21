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

#include "multipleActions.h"
#include "logMode.h"
#include "logging.h"

#include "sambaLogMode.h"
#include "sambaAccessLogMode.h"
#include "netbiosLogMode.h"

#include "sambaConfigurationWidget.h"
#include "sambaConfiguration.h"
#include "sambaItemBuilder.h"

QList<LogMode*> SambaLogModeFactory::createLogModes() const {

	//Create the shared configuration and configuration widget between the logModes

	SambaConfiguration* logModeConfiguration = new SambaConfiguration();
	SambaConfigurationWidget* logModeConfigurationWidget = new SambaConfigurationWidget();
	SambaItemBuilder* itemBuilder = new SambaItemBuilder();

	QList<LogMode*> logModes;
	logModes.append(new SambaLogMode(logModeConfiguration, logModeConfigurationWidget, itemBuilder));
	logModes.append(new SambaAccessLogMode(logModeConfiguration, logModeConfigurationWidget, itemBuilder));
	logModes.append(new NetbiosLogMode(logModeConfiguration, logModeConfigurationWidget, itemBuilder));

	return logModes;
}

LogModeAction* SambaLogModeFactory::createLogModeAction() const {
	LogMode* sambaLogMode = Globals::instance()->findLogMode(QLatin1String( SAMBA_LOG_MODE_ID ));

	MultipleActions* multipleActions = new MultipleActions(QIcon::fromTheme( QLatin1String( SAMBA_MODE_ICON) ), i18n("Samba"), sambaLogMode);
	multipleActions->addInnerAction(sambaLogMode->action());
	multipleActions->addInnerAction(Globals::instance()->findLogMode(QLatin1String( SAMBA_ACCESS_LOG_MODE_ID ))->action());
	multipleActions->addInnerAction(Globals::instance()->findLogMode(QLatin1String( NETBIOS_LOG_MODE_ID ))->action());

	multipleActions->setInToolBar(false);
	multipleActions->setCategory(LogModeAction::ServicesCategory);

	return multipleActions;
}
