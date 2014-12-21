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

#include "sambaAccessLogMode.h"

#include <QList>

#include <QIcon>
#include <KLocalizedString>

#include "logging.h"
#include "logMode.h"

#include "sambaAnalyzer.h"
#include "sambaItemBuilder.h"
#include "sambaConfigurationWidget.h"
#include "sambaConfiguration.h"


SambaAccessLogMode::SambaAccessLogMode(SambaConfiguration* sambaConfiguration, SambaConfigurationWidget* sambaConfigurationWidget, SambaItemBuilder* itemBuilder) :
	LogMode(QLatin1String( SAMBA_ACCESS_LOG_MODE_ID ), i18n("Samba Access Log"),QLatin1String( SAMBA_ACCESS_MODE_ICON )) {

	d->logModeConfiguration = sambaConfiguration;
	d->logModeConfigurationWidget = sambaConfigurationWidget;
	d->itemBuilder = itemBuilder;

	//Samba Log Action
	d->action = createDefaultAction();
	d->action->setToolTip(i18n("Display the Samba Access log."));
	d->action->setWhatsThis(i18n("Displays the Samba Access log in the current tab. This log mode allows you to see connections between your shares and remote hosts."));

}

SambaAccessLogMode::~SambaAccessLogMode() {

}

Analyzer* SambaAccessLogMode::createAnalyzer() {
	return new SambaAnalyzer(this);
}

QList<LogFile> SambaAccessLogMode::createLogFiles() {
	SambaConfiguration* sambaConfiguration = logModeConfiguration<SambaConfiguration*>();
	return sambaConfiguration->findNoModeLogFiles(sambaConfiguration->sambaAccessPaths());
}
