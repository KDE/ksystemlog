/*
    KCupsLog, a cups log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "cupsFactory.h"

#include <KLocalizedString>

#include "logMode.h"
#include "logging.h"
#include "multipleActions.h"

#include "cupsAccessLogMode.h"
#include "cupsLogMode.h"
#include "cupsPageLogMode.h"
#include "cupsPdfLogMode.h"

#include "cupsConfiguration.h"
#include "cupsConfigurationWidget.h"

QList<LogMode *> CupsLogModeFactory::createLogModes() const
{
    // Create the shared configuration and configuration widget between the logModes
    QSharedPointer<CupsConfiguration> logModeConfiguration = QSharedPointer<CupsConfiguration>(new CupsConfiguration());
    auto logModeConfigurationWidget = new CupsConfigurationWidget();

    QList<LogMode *> logModes;
    logModes.append(new CupsLogMode(logModeConfiguration, logModeConfigurationWidget));
    logModes.append(new CupsAccessLogMode(logModeConfiguration, logModeConfigurationWidget));
    logModes.append(new CupsPageLogMode(logModeConfiguration, logModeConfigurationWidget));
    logModes.append(new CupsPdfLogMode(logModeConfiguration, logModeConfigurationWidget));

    return logModes;
}

LogModeAction *CupsLogModeFactory::createLogModeAction() const
{
    LogMode *cupsLogMode = Globals::instance().findLogMode(QStringLiteral(CUPS_LOG_MODE_ID));
    LogMode *cupsAccessLogMode = Globals::instance().findLogMode(QStringLiteral(CUPS_ACCESS_LOG_MODE_ID));
    LogMode *cupsPageLogMode = Globals::instance().findLogMode(QStringLiteral(CUPS_PAGE_LOG_MODE_ID));
    LogMode *cupsPdfLogMode = Globals::instance().findLogMode(QStringLiteral(CUPS_PDF_LOG_MODE_ID));

    bool cupsLogsExist = cupsLogMode->filesExist();
    bool cupsAccessLogsExist = cupsAccessLogMode->filesExist();
    bool cupsPageLogsExist = cupsPageLogMode->filesExist();
    bool cupsPdfLogsExist = cupsPdfLogMode->filesExist();

    if (!cupsLogsExist && !cupsAccessLogsExist && !cupsPageLogsExist && !cupsPdfLogsExist) {
        return nullptr;
    }

    auto multipleActions = new MultipleActions(QIcon::fromTheme(QStringLiteral(CUPS_MODE_ICON)), i18n("Cups"), cupsLogMode);

    if (cupsLogsExist) {
        multipleActions->addInnerAction(cupsLogMode->action());
    }

    if (cupsAccessLogsExist) {
        multipleActions->addInnerAction(cupsAccessLogMode->action());
    }

    if (cupsPageLogsExist) {
        multipleActions->addInnerAction(cupsPageLogMode->action());
    }

    if (cupsPdfLogsExist) {
        multipleActions->addInnerAction(cupsPdfLogMode->action());
    }

    multipleActions->setCategory(LogModeAction::ServicesCategory);

    return multipleActions;
}
