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

#ifndef _DEFAULTS_H_
#define _DEFAULTS_H_

#define DEFAULT_LOG_FOLDER "/var/log"

/**
 * Icon name of KSystemLog
 */
#define KSYSTEMLOG_ICON "utilities-log-viewer"

enum LogLevelIds {
	NONE_LOG_LEVEL_ID=0,
	DEBUG_LOG_LEVEL_ID,
	INFORMATION_LOG_LEVEL_ID,
	NOTICE_LOG_LEVEL_ID,
	WARNING_LOG_LEVEL_ID,
	ERROR_LOG_LEVEL_ID,
	CRITICAL_LOG_LEVEL_ID,
	ALERT_LOG_LEVEL_ID,
	EMERGENCY_LOG_LEVEL_ID
};

//TODO Need a rewriting
/*
enum GroupByType {
	NO_GROUP_BY=0,
	GROUP_BY_LOG_LEVEL,
	GROUP_BY_DAY,
	GROUP_BY_HOUR,
	GROUP_BY_LOG_FILE,
	GROUP_BY_COLUMN
};

//This variable only counts GROUP_BY methods != than GROUP_BY_COLUMN
#define DEFAULT_GROUP_BY_COUNT 5

#define NO_GROUP_BY_ICON "process-stop"
#define GROUP_BY_LOG_LEVEL_ICON INFORMATION_LOG_LEVEL_ICON
#define GROUP_BY_DAY_ICON "go-jump-today"
#define GROUP_BY_HOUR_ICON "chronometer"
#define GROUP_BY_LOG_FILE_ICON "utilities-log-viewer"

#define GROUP_BY_COLUMN_ICON "view-list-text"


//Icon of the Group By action
#define GROUP_BY_ICON "view-list-tree"
*/

/**
 * Icon name of No Mode icon
 */
#define NO_MODE_ICON "text-x-generic"


#endif // _DEFAULTS_H_
