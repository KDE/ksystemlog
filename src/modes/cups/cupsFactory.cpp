/***************************************************************************
 *   KCupsLog, a cups log viewer tool                                  *
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

#include "cupsFactory.h"

#include <KLocalizedString>

#include "multipleActions.h"
#include "logMode.h"
#include "logging.h"

#include "cupsLogMode.h"
#include "cupsAccessLogMode.h"
#include "cupsPageLogMode.h"
#include "cupsPdfLogMode.h"

#include "cupsConfigurationWidget.h"
#include "cupsConfiguration.h"

QList<LogMode *> CupsLogModeFactory::createLogModes() const
{
    // Create the shared configuration and configuration widget between the logModes

    CupsConfiguration *logModeConfiguration = new CupsConfiguration();
    CupsConfigurationWidget *logModeConfigurationWidget = new CupsConfigurationWidget();

    QList<LogMode *> logModes;
    logModes.append(new CupsLogMode(logModeConfiguration, logModeConfigurationWidget));
    logModes.append(new CupsAccessLogMode(logModeConfiguration, logModeConfigurationWidget));
    logModes.append(new CupsPageLogMode(logModeConfiguration, logModeConfigurationWidget));
    logModes.append(new CupsPdfLogMode(logModeConfiguration, logModeConfigurationWidget));

    return logModes;
}

LogModeAction *CupsLogModeFactory::createLogModeAction() const
{
    LogMode *cupsLogMode = Globals::instance().findLogMode(QLatin1String(CUPS_LOG_MODE_ID));

    MultipleActions *multipleActions
        = new MultipleActions(QIcon::fromTheme(QLatin1String(CUPS_MODE_ICON)), i18n("Cups"), cupsLogMode);
    multipleActions->addInnerAction(cupsLogMode->action());
    multipleActions->addInnerAction(
        Globals::instance().findLogMode(QLatin1String(CUPS_ACCESS_LOG_MODE_ID))->action());
    multipleActions->addInnerAction(
        Globals::instance().findLogMode(QLatin1String(CUPS_PAGE_LOG_MODE_ID))->action());
    multipleActions->addInnerAction(
        Globals::instance().findLogMode(QLatin1String(CUPS_PDF_LOG_MODE_ID))->action());

    multipleActions->setCategory(LogModeAction::ServicesCategory);

    return multipleActions;
}
