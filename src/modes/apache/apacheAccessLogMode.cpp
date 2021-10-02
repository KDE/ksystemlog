/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "apacheAccessLogMode.h"

#include <QAction>

#include <KLocalizedString>

#include "logging.h"

#include "apacheAccessAnalyzer.h"
#include "apacheAccessItemBuilder.h"
#include "apacheConfiguration.h"
#include "apacheConfigurationWidget.h"

ApacheAccessLogMode::ApacheAccessLogMode(QSharedPointer<ApacheConfiguration> &apacheConfiguration, ApacheConfigurationWidget *apacheConfigurationWidget)
    : LogMode(QStringLiteral(APACHE_ACCESS_LOG_MODE_ID), i18n("Apache Access Log"), QStringLiteral(APACHE_ACCESS_MODE_ICON))
{
    d->logModeConfiguration = apacheConfiguration;
    d->logModeConfigurationWidget = apacheConfigurationWidget;

    d->itemBuilder = new ApacheAccessItemBuilder();

    // Apache Log Action
    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the Apache Access log."));
    d->action->setWhatsThis(
        i18n("Displays the Apache Access log in the current tab. Apache is the main used Web server in the world. "
             "This log saves all requests performed by the Apache web server."));

    checkLogFilesPresence(apacheConfiguration->apacheAccessPaths());
}

ApacheAccessLogMode::~ApacheAccessLogMode()
{
}

Analyzer *ApacheAccessLogMode::createAnalyzer(const QVariant &options)
{
    Q_UNUSED(options)
    return new ApacheAccessAnalyzer(this);
}

QVector<LogFile> ApacheAccessLogMode::createLogFiles()
{
    auto *apacheConfiguration = logModeConfiguration<ApacheConfiguration *>();
    return apacheConfiguration->findNoModeLogFiles(apacheConfiguration->apacheAccessPaths());
}
