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

#ifndef _ACPID_CONFIGURATION_H_
#define _ACPID_CONFIGURATION_H_

#include <QStringList>

#include "logModeConfiguration.h"

#include "logging.h"
#include "defaults.h"

#include "acpidLogMode.h"

#include "ksystemlogConfig.h"

class AcpidConfigurationPrivate {
public:
	QStringList acpidPaths;
};

class AcpidConfiguration : public LogModeConfiguration {
	
	Q_OBJECT
	
	public:
		AcpidConfiguration() :
			d(new AcpidConfigurationPrivate()) {

			configuration->setCurrentGroup("AcpidLogMode");
			
			QStringList defaultAcpidPaths;
			defaultAcpidPaths << "/var/log/acpid";
			configuration->addItemStringList("LogFilesPaths", d->acpidPaths, defaultAcpidPaths, "LogFilesPaths");
				
		}

		virtual ~AcpidConfiguration() {
			delete d;
		}
		
		QStringList acpidPaths() const {
			return d->acpidPaths;
		}
		
		void setAcpidPaths(const QStringList& acpidPaths) {
			d->acpidPaths = acpidPaths;
		}

	private:
		AcpidConfigurationPrivate* const d;

};

#endif // _ACPID_CONFIGURATION_H_
