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

#include "daemonLogMode.h"

#include <KLocalizedString>

DaemonLogMode::DaemonLogMode()
    : LogMode(QStringLiteral(DAEMON_LOG_MODE_ID), i18n("Daemons' Logs"), QStringLiteral(DAEMON_MODE_ICON))
{
    d->logModeConfiguration = QSharedPointer<DaemonConfiguration>(new DaemonConfiguration());

    d->logModeConfigurationWidget = new DaemonConfigurationWidget();

    d->itemBuilder = new LogModeItemBuilder();

    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the daemons' logs."));
    d->action->setWhatsThis(
        i18n("Displays the daemons' logs in the current tab. The daemons are all processes launched in the "
             "background of the system. See this log if you want to know what occurs in the background of your "
             "system."));

    auto *configuration = logModeConfiguration<DaemonConfiguration *>();
    checkLogFilesPresence(configuration->daemonPaths());
}

DaemonLogMode::~DaemonLogMode()
{
}

Analyzer *DaemonLogMode::createAnalyzer(const QVariant &options)
{
    Q_UNUSED(options)
    return new SyslogAnalyzer(this);
}

QVector<LogFile> DaemonLogMode::createLogFiles()
{
    auto *configuration = logModeConfiguration<DaemonConfiguration *>();
    return configuration->findGenericLogFiles(configuration->daemonPaths());
}
