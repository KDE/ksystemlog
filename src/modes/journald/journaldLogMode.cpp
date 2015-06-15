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

#include "journaldLogMode.h"

#include <QList>

#include <KLocalizedString>

#include "logging.h"
#include "logMode.h"

#include "journaldAnalyzer.h"
#include "journaldConfigurationWidget.h"
#include "journaldConfiguration.h"

#include "logModeItemBuilder.h"

JournaldLogMode::JournaldLogMode()
    : LogMode(QLatin1String(JOURNALD_LOG_MODE_ID), i18n("Journald Log"), QLatin1String(JOURNALD_MODE_ICON))
{
    d->logModeConfiguration = new JournaldConfiguration();

    d->logModeConfigurationWidget = new JournaldConfigurationWidget();

    d->itemBuilder = new LogModeItemBuilder();

    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the Journald log."));
    d->action->setWhatsThis(i18n(
        "Displays the system log in the current tab. This log is generally used by non-specialized processes "
        "(like \"sudo\" or \"fsck\" commands)"));
}

JournaldLogMode::~JournaldLogMode()
{
}

Analyzer *JournaldLogMode::createAnalyzer()
{
    return new JournaldAnalyzer(this);
}

QList<LogFile> JournaldLogMode::createLogFiles()
{
    // No log file for journald.
    return QList<LogFile>();
}
