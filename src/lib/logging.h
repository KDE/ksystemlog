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


#ifndef _KSYSTEMLOG_LOGGING_H_
#define _KSYSTEMLOG_LOGGING_H_


#include <kdebug.h>

/**
 * Existing logging functions are :
 * - logDebug()
 * - logWarning()
 * - logError()
 * - logFatal()
 * 
 * Use it like kDebug() function :
 * logDebug() << "Debug message" << list.size() << endl;
 */

/*
 * Log prefix of any log message
 * The regexp removes the parameters and return type of the method prototype 
 * 
 * Example : 
 * [LogManager::synchronizeLogView(547)] Synchronizing the model...
 * 
 */

#define KSYSTEM_LOG_KDEBUG_ID 87400

/**
 * Colored logging

#define LOG_DEFAULT_COLOR "\033[m"

#define LOG_DEBUG_COLOR 			"\033[37m"
#define LOG_ERROR_COLOR            "\033[31m"
#define LOG_FATAL_COLOR            "\033[31m"
#define LOG_WARNING_COLOR         "\033[33m" 

#define LOG_PREFIX(logColor) logColor << "[" <<  __LINE__ << "]" << LOG_DEFAULT_COLOR

 */

#define LOG_DEFAULT_COLOR		""

#define LOG_DEBUG_COLOR			""
#define LOG_ERROR_COLOR			""
#define LOG_FATAL_COLOR			""
#define LOG_WARNING_COLOR		""

#define LOG_PREFIX(logColor)	"" 

#if !defined(KDE_NO_DEBUG_OUTPUT)
	#define logDebug() kDebug(KSYSTEM_LOG_KDEBUG_ID) << LOG_PREFIX(LOG_DEBUG_COLOR)  
#else  // KDE_NO_DEBUG_OUTPUT
	#define logDebug() kDebug(KSYSTEM_LOG_KDEBUG_ID)
#endif

#if !defined(KDE_NO_WARNING_OUTPUT)
	#define logWarning() kWarning(KSYSTEM_LOG_KDEBUG_ID) << LOG_PREFIX(LOG_WARNING_COLOR)
#else  // KDE_NO_WARNING_OUTPUT
	#define logWarning() kWarning(KSYSTEM_LOG_KDEBUG_ID)
#endif

#define logError() kError(KSYSTEM_LOG_KDEBUG_ID) << LOG_PREFIX(LOG_ERROR_COLOR)
#define logFatal() kFatal(KSYSTEM_LOG_KDEBUG_ID) << LOG_PREFIX(LOG_FATAL_COLOR)

#endif // _KSYSTEMLOG_LOGGING_H_
