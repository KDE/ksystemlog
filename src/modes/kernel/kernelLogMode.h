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

#ifndef _KERNEL_LOG_MODE_H_
#define _KERNEL_LOG_MODE_H_

/**
 * Kernel Log Mode Identifier
 */
#define KERNEL_LOG_MODE_ID "kernelLogMode"

/**
 * Kernel Log Icon
 */
#define KERNEL_MODE_ICON "utilities-system-monitor"

#define UPTIME_FILE "/proc/uptime"

#include <QList>

#include "logFile.h"
#include "logMode.h"

class KernelLogMode : public LogMode
{
    Q_OBJECT

public:
    explicit KernelLogMode();

    ~KernelLogMode();

    Analyzer *createAnalyzer(const QVariant &options = QVariant()) Q_DECL_OVERRIDE;

    QList<LogFile> createLogFiles() Q_DECL_OVERRIDE;
};

#endif // _KERNEL_LOG_MODE_H_
