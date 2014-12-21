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

#include "cupsPdfLogMode.h"

#include <QList>

#include <KLocalizedString>

#include "logging.h"
#include "logMode.h"

#include "cupsPdfAnalyzer.h"
#include "cupsPdfItemBuilder.h"
#include "cupsConfigurationWidget.h"
#include "cupsConfiguration.h"


CupsPdfLogMode::CupsPdfLogMode(CupsConfiguration* cupsConfiguration, CupsConfigurationWidget* cupsConfigurationWidget) :
	LogMode(QLatin1String( CUPS_PDF_LOG_MODE_ID ), i18n("Cups PDF Log"), QLatin1String( CUPS_PDF_MODE_ICON )) {

	d->logModeConfiguration = cupsConfiguration;
	d->logModeConfigurationWidget = cupsConfigurationWidget;

	d->itemBuilder = new CupsPdfItemBuilder();

	//Cups Log Action
	d->action = createDefaultAction();
	d->action->setToolTip(i18n("Display the CUPS PDF log."));
	d->action->setWhatsThis(i18n("Displays the CUPS PDF log in the current tab. CUPS is the program which manages printing on your computer. This log saves all requests performed to the CUPS embedded web server (default: <i>http://localhost:631</i>)."));

}



CupsPdfLogMode::~CupsPdfLogMode() {

}

Analyzer* CupsPdfLogMode::createAnalyzer() {
	return new CupsPdfAnalyzer(this);
}

QList<LogFile> CupsPdfLogMode::createLogFiles() {
	CupsConfiguration* cupsConfiguration = logModeConfiguration<CupsConfiguration*>();
	return cupsConfiguration->findNoModeLogFiles(cupsConfiguration->cupsPdfPaths());
}
