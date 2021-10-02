/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "cupsPdfLogMode.h"

#include <QAction>

#include <KLocalizedString>

#include "ksystemlog_debug.h"

#include "cupsConfiguration.h"
#include "cupsConfigurationWidget.h"
#include "cupsPdfAnalyzer.h"
#include "cupsPdfItemBuilder.h"

CupsPdfLogMode::CupsPdfLogMode(QSharedPointer<CupsConfiguration> &cupsConfiguration, CupsConfigurationWidget *cupsConfigurationWidget)
    : LogMode(QStringLiteral(CUPS_PDF_LOG_MODE_ID), i18n("Cups PDF Log"), QStringLiteral(CUPS_PDF_MODE_ICON))
{
    d->logModeConfiguration = cupsConfiguration;
    d->logModeConfigurationWidget = cupsConfigurationWidget;

    d->itemBuilder = new CupsPdfItemBuilder();

    // Cups Log Action
    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the CUPS PDF log."));
    d->action->setWhatsThis(
        i18n("Displays the CUPS PDF log in the current tab. CUPS is the program which manages printing on your "
             "computer. This log saves all requests performed to the CUPS embedded web server (default: "
             "<i>http://localhost:631</i>)."));

    checkLogFilesPresence(cupsConfiguration->cupsPdfPaths());
}

CupsPdfLogMode::~CupsPdfLogMode()
{
}

Analyzer *CupsPdfLogMode::createAnalyzer(const QVariant &options)
{
    Q_UNUSED(options)
    return new CupsPdfAnalyzer(this);
}

QVector<LogFile> CupsPdfLogMode::createLogFiles()
{
    auto *cupsConfiguration = logModeConfiguration<CupsConfiguration *>();
    return cupsConfiguration->findNoModeLogFiles(cupsConfiguration->cupsPdfPaths());
}
