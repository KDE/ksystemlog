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

#include "xsessionLogMode.h"

#include <QList>

#include <KLocalizedString>

#include "logging.h"
#include "logMode.h"

#include "xsessionAnalyzer.h"
#include "xsessionItemBuilder.h"
#include "xsessionConfigurationWidget.h"
#include "xsessionConfiguration.h"

XSessionLogMode::XSessionLogMode()
    : LogMode(QLatin1String(X_SESSION_LOG_MODE_ID), i18n("X Session Log"), QLatin1String(X_SESSION_MODE_ICON))
{
    d->logModeConfiguration = new XSessionConfiguration();

    d->logModeConfigurationWidget = new XSessionConfigurationWidget();

    d->itemBuilder = new XSessionItemBuilder();

    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the X Session log."));
    d->action->setWhatsThis(i18n(
        "Displays the X Session log in the current tab. X Session log is the place where graphical programs "
        "write their output. See this log if you want to know why a program has crashed, or why your display "
        "manager (KDE, Gnome,...) has not started."));
}

XSessionLogMode::~XSessionLogMode()
{
}

Analyzer *XSessionLogMode::createAnalyzer()
{
    return new XSessionAnalyzer(this);
}

QList<LogFile> XSessionLogMode::createLogFiles()
{
    XSessionConfiguration *configuration = Globals::instance()
                                               .findLogMode(QLatin1String(X_SESSION_LOG_MODE_ID))
                                               ->logModeConfiguration<XSessionConfiguration *>();

    QList<LogFile> logFiles;
    logFiles.append(configuration->findGenericLogFile(configuration->xsessionPath()));
    return logFiles;
}
