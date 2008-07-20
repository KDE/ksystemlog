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

#include "apacheLogMode.h"

#include <QList>

#include <kicon.h>
#include <klocale.h>

#include "logging.h"
#include "logMode.h"

#include "apacheAnalyzer.h"
#include "apacheItemBuilder.h"
#include "apacheConfigurationWidget.h"
#include "apacheConfiguration.h"


ApacheLogMode::ApacheLogMode(ApacheConfiguration* apacheConfiguration, ApacheConfigurationWidget* apacheConfigurationWidget) : 
	LogMode(APACHE_LOG_MODE_ID, i18n("Apache Log"), APACHE_MODE_ICON) {
	
	d->logModeConfiguration = apacheConfiguration;
	d->logModeConfigurationWidget = apacheConfigurationWidget;
	
	d->itemBuilder = new ApacheItemBuilder();
	
	//Apache Log Action
	d->action = createDefaultAction();
	d->action->setToolTip(i18n("Display the Apache log."));
	d->action->setWhatsThis(i18n("Displays the Apache log in the current tab. Apache is the main used Web server in the world."));
	
}

ApacheLogMode::~ApacheLogMode() {
	
}

Analyzer* ApacheLogMode::createAnalyzer() {
	return new ApacheAnalyzer(this);
}

QList<LogFile> ApacheLogMode::createLogFiles() {
	ApacheConfiguration* apacheConfiguration = logModeConfiguration<ApacheConfiguration*>();
	return apacheConfiguration->findNoModeLogFiles(apacheConfiguration->apachePaths());
}
