/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
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

#include "acpidLogMode.h"

#include <QList>

#include <KLocalizedString>

#include "logging.h"
#include "logMode.h"

#include "acpidAnalyzer.h"
#include "acpidItemBuilder.h"
#include "acpidConfigurationWidget.h"
#include "acpidConfiguration.h"

AcpidLogMode::AcpidLogMode()
    : LogMode(QLatin1String(ACPID_LOG_MODE_ID), i18n("ACPI Log"), QLatin1String(ACPID_MODE_ICON))
{
    d->logModeConfiguration = new AcpidConfiguration();

    d->logModeConfigurationWidget = new AcpidConfigurationWidget();

    d->itemBuilder = new AcpidItemBuilder();

    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the ACPI log."));
    d->action->setWhatsThis(i18n(
        "Displays the ACPI log in the current tab. ACPI is used to manage the hardware components of your "
        "computer, like notebook batteries, reset buttons..."));

    AcpidConfiguration *configuration = logModeConfiguration<AcpidConfiguration *>();
    checkLogFilesPresence(configuration->acpidPaths());
}

AcpidLogMode::~AcpidLogMode()
{
}

Analyzer *AcpidLogMode::createAnalyzer()
{
    return new AcpidAnalyzer(this);
}

QList<LogFile> AcpidLogMode::createLogFiles()
{
    AcpidConfiguration *configuration = logModeConfiguration<AcpidConfiguration *>();
    return configuration->findNoModeLogFiles(configuration->acpidPaths());
}
