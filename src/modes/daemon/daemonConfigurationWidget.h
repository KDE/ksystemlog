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

#ifndef _DAEMON_CONFIGURATION_WIDGET_H_
#define _DAEMON_CONFIGURATION_WIDGET_H_


#include <klocale.h>

#include "globals.h"
#include "logging.h"
#include "fileList.h"

#include "logLevel.h"

#include "daemonConfiguration.h"
#include "daemonLogMode.h"

#include "logModeConfigurationWidget.h"

class FileList;

class DaemonConfigurationWidget : public LogModeConfigurationWidget {
	
	Q_OBJECT
	
	public:
		DaemonConfigurationWidget() : 
			LogModeConfigurationWidget(i18n("Daemons' Logs"), DAEMON_MODE_ICON, i18n("Daemons' Logs"))
			{
			
			QHBoxLayout* layout = new QHBoxLayout();
			this->setLayout(layout);

			fileList=new FileList(this, i18n("<p>These files will be analyzed to show the <b>Daemons' Logs</b>.</p>"));
			connect(fileList, SIGNAL(fileListChanged()), this, SIGNAL(configurationChanged()));
			layout->addWidget(fileList);

		}

		~DaemonConfigurationWidget() {
			
		}
		
	public slots:
	
		void saveConfig() {
			DaemonConfiguration* daemonConfiguration = Globals::instance()->findLogMode(DAEMON_LOG_MODE_ID)->logModeConfiguration<DaemonConfiguration*>();
			
			daemonConfiguration->setDaemonPaths(fileList->paths());
		}

		void readConfig() {
			DaemonConfiguration* daemonConfiguration = Globals::instance()->findLogMode(DAEMON_LOG_MODE_ID)->logModeConfiguration<DaemonConfiguration*>();

			fileList->removeAllItems();
			
			fileList->addPaths(daemonConfiguration->daemonPaths());
		}

		void defaultConfig() {
			//TODO Find a way to read the configuration per default
			readConfig();
		}

	protected:
		bool isValid() const {
			if (fileList->isEmpty()==false) {
				return true;
			}

			return false;

		}

	private:
		FileList* fileList;

};

#endif // _DAEMON_CONFIGURATION_WIDGET_H_
