/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "ksystemlog_debug.h"

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

#define LOG_DEFAULT_COLOR ""

#define LOG_DEBUG_COLOR ""
#define LOG_CRITICAL_COLOR ""
#define LOG_FATAL_COLOR ""
#define LOG_WARNING_COLOR ""

#define LOG_PREFIX(logColor) ""

#if !defined(KDE_NO_DEBUG_OUTPUT)
#define logDebug() qCDebug(KSYSTEMLOG) << LOG_PREFIX(LOG_DEBUG_COLOR)
#else // KDE_NO_DEBUG_OUTPUT
#define logDebug() qCDebug(KSYSTEMLOG)
#endif

#if !defined(KDE_NO_WARNING_OUTPUT)
#define logWarning() qCWarning(KSYSTEMLOG) << LOG_PREFIX(LOG_WARNING_COLOR)
#else // KDE_NO_WARNING_OUTPUT
#define logWarning() qCWarning(KSYSTEMLOG)
#endif

#define logCritical() qCCritical(KSYSTEMLOG) << LOG_PREFIX(LOG_CRITICAL_COLOR)

#define logFatal() qCFatal(KSYSTEMLOG) << LOG_PREFIX(LOG_FATAL_COLOR)

