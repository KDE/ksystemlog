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

#include "xorgLogMode.h"

#include <QList>

#include <KLocalizedString>

#include "logging.h"
#include "logMode.h"

#include "xorgAnalyzer.h"
#include "xorgItemBuilder.h"
#include "xorgConfigurationWidget.h"
#include "xorgConfiguration.h"

XorgLogMode::XorgLogMode()
    : LogMode(QStringLiteral(XORG_LOG_MODE_ID), i18n("X.org Log"), QStringLiteral(XORG_MODE_ICON))
{
    d->logModeConfiguration = QSharedPointer<XorgConfiguration>(new XorgConfiguration());

    d->logModeConfigurationWidget = new XorgConfigurationWidget();

    d->itemBuilder = new XorgItemBuilder();

    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the X.org log."));
    d->action->setWhatsThis(i18n(
        "Displays the X.org log in the current tab. X.org is the service which displays on screen your "
        "desktop and manage your graphical hardware. See this log if you want to know why you do not have 3D "
        "accelerations or why your input device is not recognized."));

    XorgConfiguration *configuration = logModeConfiguration<XorgConfiguration *>();
    checkLogFilesPresence(configuration->xorgPaths());
}

XorgLogMode::~XorgLogMode()
{
}

Analyzer *XorgLogMode::createAnalyzer(const QVariant &options)
{
    Q_UNUSED(options)
    return new XorgAnalyzer(this);
}

QList<LogFile> XorgLogMode::createLogFiles()
{
    XorgConfiguration *configuration = logModeConfiguration<XorgConfiguration *>();
    return configuration->findNoModeLogFiles(configuration->xorgPaths());
}
