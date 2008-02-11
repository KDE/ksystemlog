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

#include "apacheFactory.h"


#include <klocale.h>

#include "multipleActions.h"
#include "logMode.h"
#include "logging.h"

#include "apacheLogMode.h"
#include "apacheAccessLogMode.h"

#include "apacheConfigurationWidget.h"
#include "apacheConfiguration.h"

QList<LogMode*> ApacheLogModeFactory::createLogModes() const {

	//Create the shared configuration and configuration widget between the logModes
	
	ApacheConfiguration* logModeConfiguration = new ApacheConfiguration();
	ApacheConfigurationWidget* logModeConfigurationWidget = new ApacheConfigurationWidget();
	
	QList<LogMode*> logModes;
	logModes.append(new ApacheLogMode(logModeConfiguration, logModeConfigurationWidget));
	logModes.append(new ApacheAccessLogMode(logModeConfiguration, logModeConfigurationWidget));
	
	return logModes;
}

LogModeAction* ApacheLogModeFactory::createLogModeAction() const {
	LogMode* apacheLogMode = Globals::instance()->findLogMode(APACHE_LOG_MODE_ID);
	
	MultipleActions* multipleActions = new MultipleActions(KIcon(APACHE_MODE_ICON), i18n("Apache"), apacheLogMode);
	multipleActions->addInnerAction(apacheLogMode->action());
	multipleActions->addInnerAction(Globals::instance()->findLogMode(APACHE_ACCESS_LOG_MODE_ID)->action());
	
	multipleActions->setInToolBar(false);
	multipleActions->setCategory(LogModeAction::ServicesCategory);
	
	return multipleActions;
}
