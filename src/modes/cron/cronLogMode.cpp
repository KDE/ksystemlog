/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "cronLogMode.h"

#include <KLocalizedString>

#include "ksystemlog_debug.h"

#include "cronAnalyzer.h"
#include "cronConfiguration.h"
#include "cronConfigurationWidget.h"
#include "cronItemBuilder.h"

CronLogMode::CronLogMode()
    : LogMode(QStringLiteral(CRON_LOG_MODE_ID), i18n("Cron Log"), QStringLiteral(CRON_MODE_ICON))
{
    d->logModeConfiguration = QSharedPointer<CronConfiguration>(new CronConfiguration());

    d->logModeConfigurationWidget = new CronConfigurationWidget();

    d->itemBuilder = new CronItemBuilder();

    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the planned tasks log (Cron log)."));
    d->action->setWhatsThis(
        i18n("Displays the planned tasks log in the current tab. Cron process is a program in charge of launching "
             "planned tasks on your system, like security checks, or auto-restarting of some services. Use this "
             "menu to see the recently launched processes."));

    auto *cronConfiguration = logModeConfiguration<CronConfiguration *>();
    checkLogFilesPresence(cronConfiguration->cronPaths());
}

CronLogMode::~CronLogMode()
{
}

Analyzer *CronLogMode::createAnalyzer(const QVariant &options)
{
    Q_UNUSED(options)
    return new CronAnalyzer(this);
}

QVector<LogFile> CronLogMode::createLogFiles()
{
    auto *cronConfiguration = logModeConfiguration<CronConfiguration *>();
    return cronConfiguration->findNoModeLogFiles(cronConfiguration->cronPaths());
}
