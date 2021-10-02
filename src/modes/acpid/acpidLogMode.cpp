/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "acpidLogMode.h"

#include <KLocalizedString>

#include "logging.h"

#include "acpidAnalyzer.h"
#include "acpidConfiguration.h"
#include "acpidConfigurationWidget.h"
#include "acpidItemBuilder.h"

AcpidLogMode::AcpidLogMode()
    : LogMode(QStringLiteral(ACPID_LOG_MODE_ID), i18n("ACPI Log"), QStringLiteral(ACPID_MODE_ICON))
{
    d->logModeConfiguration = QSharedPointer<AcpidConfiguration>(new AcpidConfiguration());

    d->logModeConfigurationWidget = new AcpidConfigurationWidget();

    d->itemBuilder = new AcpidItemBuilder();

    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the ACPI log."));
    d->action->setWhatsThis(
        i18n("Displays the ACPI log in the current tab. ACPI is used to manage the hardware components of your "
             "computer, like notebook batteries, reset buttons..."));

    auto *configuration = logModeConfiguration<AcpidConfiguration *>();
    checkLogFilesPresence(configuration->acpidPaths());
}

AcpidLogMode::~AcpidLogMode()
{
}

Analyzer *AcpidLogMode::createAnalyzer(const QVariant &options)
{
    Q_UNUSED(options)
    return new AcpidAnalyzer(this);
}

QVector<LogFile> AcpidLogMode::createLogFiles()
{
    auto *configuration = logModeConfiguration<AcpidConfiguration *>();
    return configuration->findNoModeLogFiles(configuration->acpidPaths());
}
