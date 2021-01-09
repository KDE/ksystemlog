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

#include "authenticationLogMode.h"

#include <KLocalizedString>

#include "logging.h"

#include "authenticationAnalyzer.h"
#include "logModeItemBuilder.h"
#include "authenticationConfigurationWidget.h"
#include "authenticationConfiguration.h"

AuthenticationLogMode::AuthenticationLogMode()
    : LogMode(QStringLiteral(AUTHENTICATION_LOG_MODE_ID), i18n("Authentication Log"),
              QStringLiteral(AUTHENTICATION_MODE_ICON))
{
    d->logModeConfiguration = QSharedPointer<AuthenticationConfiguration>(new AuthenticationConfiguration());

    d->logModeConfigurationWidget = new AuthenticationConfigurationWidget();

    d->itemBuilder = new LogModeItemBuilder();

    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the authentication log."));
    d->action->setWhatsThis(i18n(
                                "Displays the authentication log in the current tab. This log displays all logins made by each user "
                                "of the system, and can help you to know if someone tried to crack your system."));

    auto *configuration = logModeConfiguration<AuthenticationConfiguration *>();
    checkLogFilesPresence(QStringList() << configuration->authenticationPath());
}

AuthenticationLogMode::~AuthenticationLogMode()
{
}

Analyzer *AuthenticationLogMode::createAnalyzer(const QVariant &options)
{
    Q_UNUSED(options)
    return new AuthenticationAnalyzer(this);
}

QList<LogFile> AuthenticationLogMode::createLogFiles()
{
    auto *configuration = logModeConfiguration<AuthenticationConfiguration *>();

    const QList<LogFile> logFiles {configuration->findGenericLogFile(configuration->authenticationPath())};
    return logFiles;
}
