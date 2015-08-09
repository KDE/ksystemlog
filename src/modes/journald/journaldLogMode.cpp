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

#include <KLocalizedString>

#include "logging.h"
#include "logMode.h"

#include "journaldLocalAnalyzer.h"
#include "journaldConfigurationWidget.h"
#include "journaldConfiguration.h"
#include "journaldItemBuilder.h"

JournaldLogMode::JournaldLogMode()
    : LogMode(QLatin1String(JOURNALD_LOG_MODE_ID), i18n("Journald Log"), QLatin1String(JOURNALD_MODE_ICON))
{
    d->logModeConfiguration = QSharedPointer<JournaldConfiguration>(new JournaldConfiguration());

    d->logModeConfigurationWidget = new JournaldConfigurationWidget();

    d->itemBuilder = new JournaldItemBuilder();

    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the Journald log."));
    d->action->setWhatsThis(i18n("Displays the journald log in the current tab."));
}

JournaldLogMode::~JournaldLogMode()
{
}

Analyzer *JournaldLogMode::createAnalyzer(const QVariant &options)
{
    JournaldAnalyzerOptions analyzerOptions = options.value<JournaldAnalyzerOptions>();
    switch (analyzerOptions.analyzerType) {
    case JournaldAnalyzerType::Local:
        return new JournaldLocalAnalyzer(this, analyzerOptions.filter);
        break;
    case JournaldAnalyzerType::Network:
        break;
    default:
        break;
    }

    return new JournaldLocalAnalyzer(this);
}

QList<LogFile> JournaldLogMode::createLogFiles()
{
    // No log file for journald.
    return QList<LogFile>();
}
