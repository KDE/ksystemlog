/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "cupsAccessLogMode.h"

#include <QAction>

#include <KLocalizedString>

#include "logging.h"

#include "cupsAccessAnalyzer.h"
#include "cupsAccessItemBuilder.h"
#include "cupsConfiguration.h"
#include "cupsConfigurationWidget.h"

CupsAccessLogMode::CupsAccessLogMode(QSharedPointer<CupsConfiguration> &cupsConfiguration, CupsConfigurationWidget *cupsConfigurationWidget)
    : LogMode(QStringLiteral(CUPS_ACCESS_LOG_MODE_ID), i18n("Cups Web Log"), QStringLiteral(CUPS_ACCESS_MODE_ICON))
{
    d->logModeConfiguration = cupsConfiguration;
    d->logModeConfigurationWidget = cupsConfigurationWidget;

    d->itemBuilder = new CupsAccessItemBuilder();

    // Cups Log Action
    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the CUPS Web Server Access log."));
    d->action->setWhatsThis(
        i18n("Displays the CUPS Web Server Access log in the current tab. CUPS is the program which manages "
             "printing on your computer. This log saves all requests performed to the CUPS embedded web server "
             "(default: <i>http://localhost:631</i>)."));

    checkLogFilesPresence(cupsConfiguration->cupsAccessPaths());
}

CupsAccessLogMode::~CupsAccessLogMode()
{
}

Analyzer *CupsAccessLogMode::createAnalyzer(const QVariant &options)
{
    Q_UNUSED(options)
    return new CupsAccessAnalyzer(this);
}

QVector<LogFile> CupsAccessLogMode::createLogFiles()
{
    auto *cupsConfiguration = logModeConfiguration<CupsConfiguration *>();
    return cupsConfiguration->findNoModeLogFiles(cupsConfiguration->cupsAccessPaths());
}
