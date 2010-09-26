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

#include "configurationDialog.h"


#include <QList>

#include <klocale.h>
#include <kiconloader.h>


#include "generalConfigurationWidget.h"

#include "logModeConfigurationWidget.h"

#include "mainWindow.h"

#include "globals.h"
#include "ksystemlogConfig.h"

#include "logging.h"
#include "defaults.h"

class ConfigurationDialogPrivate {
public:

	GeneralConfigurationWidget* generalConfiguration;

	QList<LogModeConfigurationWidget*> logModeConfigurations;

	bool changed;
};

ConfigurationDialog::ConfigurationDialog(QWidget* parent) :
	KConfigDialog(parent, i18n("Settings"), KSystemLogConfig::self()),
	d(new ConfigurationDialogPrivate())
	{

	d->changed = false;

	setupGeneralConfiguration();

	setupLogModeConfigurations();

}

ConfigurationDialog::~ConfigurationDialog() {
	//All configuration pages are managed by KConfigDialog
	delete d;
}

void ConfigurationDialog::setupLogModeConfigurations() {
	logDebug() << "Setup Log Mode Configurations..." << endl;


	foreach(LogMode* logMode, Globals::instance()->logModes()) {
		//Some Log mode does not need a configuration widget
		if (logMode->logModeConfigurationWidget() == NULL) {
			continue;
		}

		//The configuration widget could be shared between Log Modes
		if (d->logModeConfigurations.contains(logMode->logModeConfigurationWidget()) == true) {
			continue;
		}

		d->logModeConfigurations.append(logMode->logModeConfigurationWidget());
	}


	foreach (LogModeConfigurationWidget* logModeConfigurationWidget, d->logModeConfigurations ) {
		logDebug() << "Adding " << logModeConfigurationWidget->itemName() << " configuration..." << endl;

		addPage(logModeConfigurationWidget, logModeConfigurationWidget->itemName(), logModeConfigurationWidget->iconName(), logModeConfigurationWidget->header(), false);

		connect(logModeConfigurationWidget, SIGNAL(configurationChanged()), this, SLOT(updateConfiguration()));
	}
}

void ConfigurationDialog::showConfiguration() {
	logDebug() << "Showing Configuration dialog..." << endl;

	show();
}

void ConfigurationDialog::setupGeneralConfiguration() {
	d->generalConfiguration = new GeneralConfigurationWidget();

	addPage(d->generalConfiguration, i18n("General"), QLatin1String( KSYSTEMLOG_ICON ), i18n("General"), false);

	connect(d->generalConfiguration, SIGNAL(configurationChanged()), this, SLOT(updateConfiguration()));
}

void ConfigurationDialog::updateSettings() {
	logDebug() << "Saving configuration..." << endl;

	d->changed = false;

	d->generalConfiguration->saveConfig();

	foreach (LogModeConfigurationWidget* logModeConfigurationWidget, d->logModeConfigurations ) {
		logModeConfigurationWidget->saveConfig();
	}

	KSystemLogConfig::self()->writeConfig();

	emit configurationSaved();

	logDebug() << "Configuration saved" << endl;
}

bool ConfigurationDialog::hasChanged() {
	logDebug() << "Current change status : " << d->changed << endl;
	return d->changed;
}

void ConfigurationDialog::updateConfiguration() {
	logDebug() << "Updating configuration..." << endl;

	bool valid = d->generalConfiguration->isValid();
	if (valid) {
		foreach (LogModeConfigurationWidget* logModeConfigurationWidget, d->logModeConfigurations ) {
			if (logModeConfigurationWidget->isValid() == false) {
				valid = false;
				break;
			}
		}
	}

	if (valid == true) {
		enableButtonOk(true);

		updateButtons();
	}
	else {
		enableButtonOk(false);
		enableButtonApply(false);
	}

}

void ConfigurationDialog::updateButtons() {
	logDebug() << "Updating configuration buttons..." << endl;

	d->changed = true;

}

void ConfigurationDialog::updateWidgets() {
	logDebug() << "Reading configuration..." << endl;

	d->generalConfiguration->readConfig();
	foreach (LogModeConfigurationWidget* logModeConfigurationWidget, d->logModeConfigurations ) {
		logModeConfigurationWidget->readConfig();
	}

	d->changed = false;
}

void ConfigurationDialog::updateWidgetsDefault() {
	logDebug() << "Loading default configuration..." << endl;

	d->generalConfiguration->defaultConfig();
	foreach (LogModeConfigurationWidget* logModeConfigurationWidget, d->logModeConfigurations ) {
		logModeConfigurationWidget->defaultConfig();
	}

	d->changed = false;
}

bool ConfigurationDialog::isDefault() {
	/**
	 * TODO Set this to true and find a way to retrieve defaults value
	 * of the configuration (see defaultConfig() methods of LogModeConfigurations)
	 */

	return true;
}

#include "configurationDialog.moc"

