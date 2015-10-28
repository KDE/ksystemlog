/***************************************************************************
 *   KApacheLog, a apache log viewer tool                                  *
 *   Copyright (C) 2007 by Nicolas Ternisien                               *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#include "apacheFactory.h"

#include <KLocalizedString>

#include "multipleActions.h"
#include "logMode.h"
#include "logging.h"

#include "apacheLogMode.h"
#include "apacheAccessLogMode.h"

#include "apacheConfigurationWidget.h"
#include "apacheConfiguration.h"

QList<LogMode *> ApacheLogModeFactory::createLogModes() const
{
    // Create the shared configuration and configuration widget between the logModes

    QSharedPointer<ApacheConfiguration> logModeConfiguration = QSharedPointer<ApacheConfiguration>(new ApacheConfiguration());
    ApacheConfigurationWidget *logModeConfigurationWidget = new ApacheConfigurationWidget();

    QList<LogMode *> logModes;
    logModes.append(new ApacheLogMode(logModeConfiguration, logModeConfigurationWidget));
    logModes.append(new ApacheAccessLogMode(logModeConfiguration, logModeConfigurationWidget));

    return logModes;
}

LogModeAction *ApacheLogModeFactory::createLogModeAction() const
{
    LogMode *apacheLogMode = Globals::instance().findLogMode(QStringLiteral(APACHE_LOG_MODE_ID));
    LogMode *apacheAccessLogMode = Globals::instance().findLogMode(QStringLiteral(APACHE_ACCESS_LOG_MODE_ID));

    bool apacheLogsExist = apacheLogMode->filesExist();
    bool apacheAccessLogsExist = apacheAccessLogMode->filesExist();

    if (!apacheLogsExist && !apacheAccessLogsExist)
        return nullptr;

    MultipleActions *multipleActions = new MultipleActions(QIcon::fromTheme(QStringLiteral(APACHE_MODE_ICON)),
                                                           i18n("Apache"), apacheLogMode);
    if (apacheLogsExist)
        multipleActions->addInnerAction(apacheLogMode->action());

    if (apacheAccessLogsExist)
        multipleActions->addInnerAction(apacheAccessLogMode->action());

    multipleActions->setCategory(LogModeAction::ServicesCategory);

    return multipleActions;
}
