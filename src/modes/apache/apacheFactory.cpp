/*
    KApacheLog, a apache log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "apacheFactory.h"

#include <KLocalizedString>

#include "ksystemlog_debug.h"
#include "logMode.h"
#include "multipleActions.h"

#include "apacheAccessLogMode.h"
#include "apacheLogMode.h"

#include "apacheConfiguration.h"
#include "apacheConfigurationWidget.h"

QList<LogMode *> ApacheLogModeFactory::createLogModes() const
{
    // Create the shared configuration and configuration widget between the logModes

    QSharedPointer<ApacheConfiguration> logModeConfiguration = QSharedPointer<ApacheConfiguration>(new ApacheConfiguration());
    auto logModeConfigurationWidget = new ApacheConfigurationWidget();

    QList<LogMode *> logModes;
    logModes.append(new ApacheLogMode(logModeConfiguration, logModeConfigurationWidget));
    logModes.append(new ApacheAccessLogMode(logModeConfiguration, logModeConfigurationWidget));

    return logModes;
}

LogModeAction *ApacheLogModeFactory::createLogModeAction() const
{
    LogMode *apacheLogMode = Globals::instance().findLogMode(QStringLiteral(APACHE_LOG_MODE_ID));
    LogMode *apacheAccessLogMode = Globals::instance().findLogMode(QStringLiteral(APACHE_ACCESS_LOG_MODE_ID));

    const bool apacheLogsExist = apacheLogMode->filesExist();
    const bool apacheAccessLogsExist = apacheAccessLogMode->filesExist();

    if (!apacheLogsExist && !apacheAccessLogsExist) {
        return nullptr;
    }

    auto multipleActions = new MultipleActions(QIcon::fromTheme(QStringLiteral(APACHE_MODE_ICON)), i18n("Apache"), apacheLogMode);
    if (apacheLogsExist) {
        multipleActions->addInnerAction(apacheLogMode->action());
    }

    if (apacheAccessLogsExist) {
        multipleActions->addInnerAction(apacheAccessLogMode->action());
    }

    multipleActions->setCategory(LogModeAction::ServicesCategory);

    return multipleActions;
}
