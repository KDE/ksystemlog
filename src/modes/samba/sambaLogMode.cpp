/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sambaLogMode.h"

#include <QAction>

#include <KLocalizedString>

#include "ksystemlog_debug.h"

#include "sambaAnalyzer.h"
#include "sambaConfiguration.h"
#include "sambaConfigurationWidget.h"
#include "sambaItemBuilder.h"

SambaLogMode::SambaLogMode(QSharedPointer<SambaConfiguration> &sambaConfiguration,
                           SambaConfigurationWidget *sambaConfigurationWidget,
                           SambaItemBuilder *itemBuilder)
    : LogMode(QStringLiteral(SAMBA_LOG_MODE_ID), i18n("Samba Log"), QStringLiteral(SAMBA_MODE_ICON))
{
    d->logModeConfiguration = sambaConfiguration;
    d->logModeConfigurationWidget = sambaConfigurationWidget;
    d->itemBuilder = itemBuilder;

    // Samba Log Action
    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the Samba log."));
    d->action->setWhatsThis(
        i18n("Displays the Samba log in the current tab. Samba is the file sharing server which interacts with "
             "Microsoft Windows network."));

    checkLogFilesPresence(sambaConfiguration->sambaPaths());
}

SambaLogMode::~SambaLogMode()
{
}

Analyzer *SambaLogMode::createAnalyzer(const QVariant &options)
{
    Q_UNUSED(options)
    return new SambaAnalyzer(this);
}

QVector<LogFile> SambaLogMode::createLogFiles()
{
    auto *sambaConfiguration = logModeConfiguration<SambaConfiguration *>();
    return sambaConfiguration->findNoModeLogFiles(sambaConfiguration->sambaPaths());
}

#include "moc_sambaLogMode.cpp"
