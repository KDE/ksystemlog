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

#ifndef _CRON_CONFIGURATION_H_
#define _CRON_CONFIGURATION_H_

#include <QStringList>

#include "logModeConfiguration.h"

#include "logging.h"
#include "defaults.h"

#include "cronLogMode.h"

#include "ksystemlogConfig.h"

class CronConfigurationPrivate {
public:
	QStringList cronPaths;
	
	QString processFilter;
};

class CronConfiguration : public LogModeConfiguration {
	
	Q_OBJECT
	
	public:
		CronConfiguration() :
			d(new CronConfigurationPrivate()) {

			configuration->setCurrentGroup("CronLogMode");
			
			QStringList defaultCronPaths;
			defaultCronPaths << "/var/log/syslog";
			configuration->addItemStringList("LogFilesPaths", d->cronPaths, defaultCronPaths, "LogFilesPaths");
			
			QString defaultProcessFilter("/usr/sbin/cron");
			configuration->addItemString("ProcessFilter", d->processFilter, defaultProcessFilter, "ProcessFilter");
						
				
		}

		virtual ~CronConfiguration() {
			delete d;
		}
		
		QString processFilter() const {
			return d->processFilter;
		}
		
		void setProcessFilter(const QString& processFilter) {
			d->processFilter = processFilter;
		}

		QStringList cronPaths() const {
			return d->cronPaths;
		}
		
		void setCronPaths(const QStringList& cronPaths) {
			d->cronPaths = cronPaths;
		}

	private:
		CronConfigurationPrivate* const d;

};

#endif // _CRON_CONFIGURATION_H_
