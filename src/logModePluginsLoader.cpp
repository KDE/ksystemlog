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

#include "logModePluginsLoader.h"

#include "globals.h"

#include "logging.h"
#include "defaults.h"

// Includes existing log modes
#include "openFactory.h"

#include "systemFactory.h"

#include "kernelFactory.h"
#include "xorgFactory.h"
#include "cronFactory.h"
#include "daemonFactory.h"
#include "authenticationFactory.h"
#include "apacheFactory.h"
#include "cupsFactory.h"
#include "postfixFactory.h"
#include "acpidFactory.h"
#include "xsessionFactory.h"
#include "sambaFactory.h"
#include "journaldFactory.h"

LogModePluginsLoader::LogModePluginsLoader(QWidget *parent)
    : parent(parent)
{
}

LogModePluginsLoader::~LogModePluginsLoader()
{
}

void LogModePluginsLoader::loadPlugins()
{
    // Open Log mode needs the current window to display error messages and open dialogs
    Globals::instance().registerLogModeFactory(new OpenLogModeFactory(parent));

    // System
    Globals::instance().registerLogModeFactory(new SystemLogModeFactory());

    // Kernel
    Globals::instance().registerLogModeFactory(new KernelLogModeFactory());

    // Authentication
    Globals::instance().registerLogModeFactory(new AuthenticationLogModeFactory());

    // Daemon
    Globals::instance().registerLogModeFactory(new DaemonLogModeFactory());

    // Xorg
    Globals::instance().registerLogModeFactory(new XorgLogModeFactory());

    // Cron
    Globals::instance().registerLogModeFactory(new CronLogModeFactory());

    // Apache
    Globals::instance().registerLogModeFactory(new ApacheLogModeFactory());

    // Postfix
    Globals::instance().registerLogModeFactory(new PostfixLogModeFactory());

    // Cups
    Globals::instance().registerLogModeFactory(new CupsLogModeFactory());

    // Samba
    Globals::instance().registerLogModeFactory(new SambaLogModeFactory());

    // Acpid
    Globals::instance().registerLogModeFactory(new AcpidLogModeFactory());

    // XSession
    Globals::instance().registerLogModeFactory(new XSessionLogModeFactory());

    // Journald
#ifdef HAVE_JOURNALD
    Globals::instance().registerLogModeFactory(new JournaldModeFactory());
#endif
}
