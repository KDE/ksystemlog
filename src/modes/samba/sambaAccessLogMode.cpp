/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sambaAccessLogMode.h"

#include <QAction>

#include <KLocalizedString>

#include "logging.h"

#include "sambaAnalyzer.h"
#include "sambaConfiguration.h"
#include "sambaConfigurationWidget.h"
#include "sambaItemBuilder.h"

SambaAccessLogMode::SambaAccessLogMode(QSharedPointer<SambaConfiguration> &sambaConfiguration,
                                       SambaConfigurationWidget *sambaConfigurationWidget,
                                       SambaItemBuilder *itemBuilder)
    : LogMode(QStringLiteral(SAMBA_ACCESS_LOG_MODE_ID), i18n("Samba Access Log"), QStringLiteral(SAMBA_ACCESS_MODE_ICON))
{
    d->logModeConfiguration = sambaConfiguration;
    d->logModeConfigurationWidget = sambaConfigurationWidget;
    d->itemBuilder = itemBuilder;

    // Samba Log Action
    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the Samba Access log."));
    d->action->setWhatsThis(
        i18n("Displays the Samba Access log in the current tab. This log mode allows you to see connections "
             "between your shares and remote hosts."));

    checkLogFilesPresence(sambaConfiguration->sambaAccessPaths());
}

SambaAccessLogMode::~SambaAccessLogMode()
{
}

Analyzer *SambaAccessLogMode::createAnalyzer(const QVariant &options)
{
    Q_UNUSED(options)
    return new SambaAnalyzer(this);
}

QVector<LogFile> SambaAccessLogMode::createLogFiles()
{
    auto *sambaConfiguration = logModeConfiguration<SambaConfiguration *>();
    return sambaConfiguration->findNoModeLogFiles(sambaConfiguration->sambaAccessPaths());
}
