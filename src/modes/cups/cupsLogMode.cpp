/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#include "cupsLogMode.h"

#include <QAction>

#include <KLocalizedString>

#include "logging.h"

#include "cupsAnalyzer.h"
#include "cupsConfiguration.h"
#include "cupsConfigurationWidget.h"
#include "cupsItemBuilder.h"

CupsLogMode::CupsLogMode(QSharedPointer<CupsConfiguration> &cupsConfiguration, CupsConfigurationWidget *cupsConfigurationWidget)
    : LogMode(QStringLiteral(CUPS_LOG_MODE_ID), i18n("Cups Log"), QStringLiteral(CUPS_MODE_ICON))
{
    d->logModeConfiguration = cupsConfiguration;
    d->logModeConfigurationWidget = cupsConfigurationWidget;

    d->itemBuilder = new CupsItemBuilder();

    // Cups Log Action
    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the Cups log."));
    d->action->setWhatsThis(
        i18n("Displays the CUPS log in the current tab. CUPS is the program which manages printing on your "
             "computer."));

    checkLogFilesPresence(cupsConfiguration->cupsPaths());
}

CupsLogMode::~CupsLogMode()
{
}

Analyzer *CupsLogMode::createAnalyzer(const QVariant &options)
{
    Q_UNUSED(options)
    return new CupsAnalyzer(this);
}

QVector<LogFile> CupsLogMode::createLogFiles()
{
    auto *cupsConfiguration = logModeConfiguration<CupsConfiguration *>();
    return cupsConfiguration->findNoModeLogFiles(cupsConfiguration->cupsPaths());
}
