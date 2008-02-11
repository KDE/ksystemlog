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

#ifndef _CUPS_ACCESS_LOG_MODE_H_
#define _CUPS_ACCESS_LOG_MODE_H_

/**
 * Cups Access Log Mode Identifier
 */
#define CUPS_ACCESS_LOG_MODE_ID "cupsAccessLogMode"

/**
 * Cups Access Log Icon
 */
#define CUPS_ACCESS_MODE_ICON "printer"


#include <QList>

#include "logFile.h"

#include "logMode.h"

class CupsConfiguration;
class CupsConfigurationWidget;

class CupsAccessLogMode : public LogMode {
	
	Q_OBJECT
	
public:
	explicit CupsAccessLogMode(CupsConfiguration* cupsConfiguration, CupsConfigurationWidget* cupsConfigurationWidget);

	~CupsAccessLogMode();

	Analyzer* createAnalyzer();

	QList<LogFile> createLogFiles();

};

#endif // _CUPS_ACCESS_LOG_MODE_H_

