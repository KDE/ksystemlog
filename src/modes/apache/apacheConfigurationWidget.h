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

#ifndef _APACHE_CONFIGURATION_WIDGET_H_
#define _APACHE_CONFIGURATION_WIDGET_H_

#include "logModeConfigurationWidget.h"


#include <klocale.h>

#include "globals.h"
#include "logging.h"
#include "multipleFileList.h"

#include "logLevel.h"

#include "apacheConfiguration.h"
#include "apacheLogMode.h"

class ApacheConfigurationWidget : public LogModeConfigurationWidget {

	Q_OBJECT

	public:
		ApacheConfigurationWidget() :
			LogModeConfigurationWidget(i18n("Apache Log"),QLatin1String( APACHE_MODE_ICON ), i18n("Apache Log"))
			{

			QHBoxLayout* layout = new QHBoxLayout();
			this->setLayout(layout);

			apacheFileList=new MultipleFileList(this, i18n("<p>These files will be analyzed to show the <b>Apache log</b> and the <b>Apache Access log</b>.</p>"));

			apachePathsId = apacheFileList->addCategory(i18n("Apache Log Files"), i18n("Add Apache File..."));
			apacheAccessPathsId = apacheFileList->addCategory(i18n("Apache Access Log Files"), i18n("Add Apache Access File..."));

			connect(apacheFileList, SIGNAL(fileListChanged()), this, SIGNAL(configurationChanged()));

			layout->addWidget(apacheFileList);
		}

		~ApacheConfigurationWidget() {

		}


	public slots:

		void saveConfig() {
			logDebug() << "Saving config from Apache Options..." << endl;

			ApacheConfiguration* apacheConfiguration = Globals::instance()->findLogMode(QLatin1String( APACHE_LOG_MODE_ID ))->logModeConfiguration<ApacheConfiguration*>();
			apacheConfiguration->setApachePaths(apacheFileList->paths(apachePathsId));
			apacheConfiguration->setApacheAccessPaths(apacheFileList->paths(apacheAccessPathsId));
		}

		void defaultConfig() {
			//TODO Find a way to read the configuration per default
			readConfig();
		}

		void readConfig() {
			ApacheConfiguration* apacheConfiguration = Globals::instance()->findLogMode(QLatin1String( APACHE_LOG_MODE_ID ))->logModeConfiguration<ApacheConfiguration*>();

			apacheFileList->removeAllItems();

			apacheFileList->addPaths(apachePathsId, apacheConfiguration->apachePaths());
			apacheFileList->addPaths(apacheAccessPathsId, apacheConfiguration->apacheAccessPaths());
		}

	protected:
		bool isValid() const {
			if (apacheFileList->isOneOfCategoryEmpty()==true) {
				logDebug() << "Apache configuration not valid" << endl;
				return false;
			}

			logDebug() << "Apache configuration valid" << endl;
			return true;

		}

	private:

		MultipleFileList* apacheFileList;

		int apachePathsId;
		int apacheAccessPathsId;

};

#endif // _APACHE_CONFIGURATION_WIDGET_H_
