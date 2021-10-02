/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
