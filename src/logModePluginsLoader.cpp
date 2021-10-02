/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "logModePluginsLoader.h"

#include "globals.h"

#include "defaults.h"
#include "logging.h"

// Includes existing log modes
#include "openFactory.h"

#include "systemFactory.h"

#include "acpidFactory.h"
#include "apacheFactory.h"
#include "authenticationFactory.h"
#include "cronFactory.h"
#include "cupsFactory.h"
#include "daemonFactory.h"
#include "kernelFactory.h"
#include "postfixFactory.h"
#include "sambaFactory.h"
#include "xorgFactory.h"
#include "xsessionFactory.h"
#ifdef HAVE_JOURNALD
#include "journaldFactory.h"
#endif
#ifdef HAVE_AUDIT
#include "auditFactory.h"
#endif

LogModePluginsLoader::LogModePluginsLoader(QWidget *parent)
    : mParent(parent)
{
}

LogModePluginsLoader::~LogModePluginsLoader()
{
}

void LogModePluginsLoader::loadPlugins()
{
    // Open Log mode needs the current window to display error messages and open dialogs
    Globals::instance().registerLogModeFactory(new OpenLogModeFactory(mParent));

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

    // Audit
#ifdef HAVE_AUDIT
    Globals::instance().registerLogModeFactory(new AuditLogModeFactory());
#endif

    // Journald
#ifdef HAVE_JOURNALD
    Globals::instance().registerLogModeFactory(new JournaldModeFactory());
#endif
}
