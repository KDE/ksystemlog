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

#include "xorgLogMode.h"

#include <QList>

#include <QIcon>
#include <klocale.h>

#include "logging.h"
#include "logMode.h"

#include "xorgAnalyzer.h"
#include "xorgItemBuilder.h"
#include "xorgConfigurationWidget.h"
#include "xorgConfiguration.h"

XorgLogMode::XorgLogMode() :
	LogMode(QLatin1String( XORG_LOG_MODE_ID ), i18n("X.org Log"),QLatin1String( XORG_MODE_ICON )) {

	d->logModeConfiguration = new XorgConfiguration();

	d->logModeConfigurationWidget = new XorgConfigurationWidget();

	d->itemBuilder = new XorgItemBuilder();

	d->action = createDefaultAction();
	d->action->setToolTip(i18n("Display the X.org log."));
	d->action->setWhatsThis(i18n("Displays the X.org log in the current tab. X.org is the service which displays on screen your desktop and manage your graphical hardware. See this log if you want to know why you do not have 3D accelerations or why your input device is not recognized."));

}

XorgLogMode::~XorgLogMode() {

}

Analyzer* XorgLogMode::createAnalyzer() {
	return new XorgAnalyzer(this);
}

QList<LogFile> XorgLogMode::createLogFiles() {
	XorgConfiguration* configuration = logModeConfiguration<XorgConfiguration*>();
	return configuration->findNoModeLogFiles(configuration->xorgPaths());
}
