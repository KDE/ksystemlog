/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "cupsPageLogMode.h"

#include <QAction>

#include <KLocalizedString>

#include "logging.h"

#include "cupsConfiguration.h"
#include "cupsConfigurationWidget.h"
#include "cupsPageAnalyzer.h"
#include "cupsPageItemBuilder.h"

CupsPageLogMode::CupsPageLogMode(QSharedPointer<CupsConfiguration> &cupsConfiguration, CupsConfigurationWidget *cupsConfigurationWidget)
    : LogMode(QStringLiteral(CUPS_PAGE_LOG_MODE_ID), i18n("Cups Page Log"), QStringLiteral(CUPS_PAGE_MODE_ICON))
{
    d->logModeConfiguration = cupsConfiguration;
    d->logModeConfigurationWidget = cupsConfigurationWidget;

    d->itemBuilder = new CupsPageItemBuilder();

    // Cups Log Action
    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the CUPS Page log."));
    d->action->setWhatsThis(
        i18n("Displays the CUPS Page log in the current tab. CUPS is the program which manages printing on your "
             "computer. This log saves all requests performed to the CUPS embedded web server (default: "
             "<i>http://localhost:631</i>)."));

    checkLogFilesPresence(cupsConfiguration->cupsPagePaths());
}

CupsPageLogMode::~CupsPageLogMode()
{
}

Analyzer *CupsPageLogMode::createAnalyzer(const QVariant &options)
{
    Q_UNUSED(options)
    return new CupsPageAnalyzer(this);
}

QVector<LogFile> CupsPageLogMode::createLogFiles()
{
    auto *cupsConfiguration = logModeConfiguration<CupsConfiguration *>();
    return cupsConfiguration->findNoModeLogFiles(cupsConfiguration->cupsPagePaths());
}
