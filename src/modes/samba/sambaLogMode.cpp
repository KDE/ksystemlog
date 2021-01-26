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

#include "sambaLogMode.h"

#include <QAction>

#include <KLocalizedString>

#include "logging.h"

#include "sambaAnalyzer.h"
#include "sambaConfiguration.h"
#include "sambaConfigurationWidget.h"
#include "sambaItemBuilder.h"

SambaLogMode::SambaLogMode(QSharedPointer<SambaConfiguration> &sambaConfiguration,
                           SambaConfigurationWidget *sambaConfigurationWidget,
                           SambaItemBuilder *itemBuilder)
    : LogMode(QStringLiteral(SAMBA_LOG_MODE_ID), i18n("Samba Log"), QStringLiteral(SAMBA_MODE_ICON))
{
    d->logModeConfiguration = sambaConfiguration;
    d->logModeConfigurationWidget = sambaConfigurationWidget;
    d->itemBuilder = itemBuilder;

    // Samba Log Action
    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the Samba log."));
    d->action->setWhatsThis(
        i18n("Displays the Samba log in the current tab. Samba is the file sharing server which interacts with "
             "Microsoft Windows network."));

    checkLogFilesPresence(sambaConfiguration->sambaPaths());
}

SambaLogMode::~SambaLogMode()
{
}

Analyzer *SambaLogMode::createAnalyzer(const QVariant &options)
{
    Q_UNUSED(options)
    return new SambaAnalyzer(this);
}

QList<LogFile> SambaLogMode::createLogFiles()
{
    auto *sambaConfiguration = logModeConfiguration<SambaConfiguration *>();
    return sambaConfiguration->findNoModeLogFiles(sambaConfiguration->sambaPaths());
}
