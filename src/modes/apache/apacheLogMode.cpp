/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "apacheLogMode.h"

#include <QAction>

#include <KLocalizedString>

#include "logging.h"

#include "apacheAnalyzer.h"
#include "apacheConfiguration.h"
#include "apacheConfigurationWidget.h"
#include "apacheItemBuilder.h"

ApacheLogMode::ApacheLogMode(QSharedPointer<ApacheConfiguration> &apacheConfiguration, ApacheConfigurationWidget *apacheConfigurationWidget)
    : LogMode(QStringLiteral(APACHE_LOG_MODE_ID), i18n("Apache Log"), QStringLiteral(APACHE_MODE_ICON))
{
    d->logModeConfiguration = apacheConfiguration;
    d->logModeConfigurationWidget = apacheConfigurationWidget;

    d->itemBuilder = new ApacheItemBuilder();

    // Apache Log Action
    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the Apache log."));
    d->action->setWhatsThis(i18n("Displays the Apache log in the current tab. Apache is the main used Web server in the world."));

    checkLogFilesPresence(apacheConfiguration->apachePaths());
}

ApacheLogMode::~ApacheLogMode()
{
}

Analyzer *ApacheLogMode::createAnalyzer(const QVariant &options)
{
    Q_UNUSED(options)
    return new ApacheAnalyzer(this);
}

QVector<LogFile> ApacheLogMode::createLogFiles()
{
    auto *apacheConfiguration = logModeConfiguration<ApacheConfiguration *>();
    return apacheConfiguration->findNoModeLogFiles(apacheConfiguration->apachePaths());
}
