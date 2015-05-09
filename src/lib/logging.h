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

#include <QLoggingCategory>

/**
 * Existing logging functions are :
 * - logDebug()
 * - logWarning()
 * - logCritical()
 * - logFatal()
 * 
 * Use it like kDebug() function :
 * logDebug() << "Debug message" << list.size();
 */

Q_DECLARE_LOGGING_CATEGORY(KSYSTEMLOG)

/**
 * Colored logging
 */

/*

#define LOG_DEFAULT_COLOR   "\033[m"

#define LOG_DEBUG_COLOR     "\033[37m"
#define LOG_CRITICAL_COLOR  "\033[31m"
#define LOG_FATAL_COLOR     "\033[31m"
#define LOG_WARNING_COLOR   "\033[33m"

#define LOG_PREFIX(logColor) logColor << "[" <<  __LINE__ << "]" << LOG_DEFAULT_COLOR

*/

#define LOG_DEFAULT_COLOR   ""

#define LOG_DEBUG_COLOR     ""
#define LOG_CRITICAL_COLOR  ""
#define LOG_FATAL_COLOR     ""
#define LOG_WARNING_COLOR   ""

#define LOG_PREFIX(logColor) ""

#if !defined(KDE_NO_DEBUG_OUTPUT)
  #define logDebug() qCDebug(KSYSTEMLOG) << LOG_PREFIX(LOG_DEBUG_COLOR)
#else  // KDE_NO_DEBUG_OUTPUT
  #define logDebug() qCDebug(KSYSTEMLOG)
#endif

#if !defined(KDE_NO_WARNING_OUTPUT)
  #define logWarning() qCWarning(KSYSTEMLOG) << LOG_PREFIX(LOG_WARNING_COLOR)
#else  // KDE_NO_WARNING_OUTPUT
	#define logWarning() qCWarning(KSYSTEMLOG)
#endif

#define logCritical() qCCritical(KSYSTEMLOG) << LOG_PREFIX(LOG_CRITICAL_COLOR)

#define logFatal() qCFatal(KSYSTEMLOG) << LOG_PREFIX(LOG_FATAL_COLOR)

#endif // _KSYSTEMLOG_LOGGING_H_
