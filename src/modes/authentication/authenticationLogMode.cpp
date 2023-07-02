/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "authenticationLogMode.h"

#include <KLocalizedString>

#include "ksystemlog_debug.h"

#include "authenticationAnalyzer.h"
#include "authenticationConfiguration.h"
#include "authenticationConfigurationWidget.h"
#include "logModeItemBuilder.h"

AuthenticationLogMode::AuthenticationLogMode()
    : LogMode(QStringLiteral(AUTHENTICATION_LOG_MODE_ID), i18n("Authentication Log"), QStringLiteral(AUTHENTICATION_MODE_ICON))
{
    d->logModeConfiguration = QSharedPointer<AuthenticationConfiguration>(new AuthenticationConfiguration());

    d->logModeConfigurationWidget = new AuthenticationConfigurationWidget();

    d->itemBuilder = new LogModeItemBuilder();

    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the authentication log."));
    d->action->setWhatsThis(
        i18n("Displays the authentication log in the current tab. This log displays all logins made by each user "
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

QVector<LogFile> AuthenticationLogMode::createLogFiles()
{
    auto *configuration = logModeConfiguration<AuthenticationConfiguration *>();

    const QVector<LogFile> logFiles{configuration->findGenericLogFile(configuration->authenticationPath())};
    return logFiles;
}

#include "moc_authenticationLogMode.cpp"
