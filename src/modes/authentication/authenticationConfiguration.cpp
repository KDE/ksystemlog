/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "authenticationConfiguration.h"

AuthenticationConfiguration::AuthenticationConfiguration()
{
    mConfiguration->setCurrentGroup(QStringLiteral("AuthenticationLogMode"));

    const QString defaultAuthenticationPath{QStringLiteral("/var/log/auth.log")};
    mConfiguration->addItemString(QStringLiteral("LogFilePath"), mAuthenticationPath, defaultAuthenticationPath, QStringLiteral("LogFilePath"));

    const QStringList defaultWarningKeywords{QStringLiteral("failed")};
    mConfiguration->addItemStringList(QStringLiteral("WarningKeywords"), mWarningKeywords, defaultWarningKeywords, QStringLiteral("WarningKeywords"));

    const QStringList defaultErrorKeywords{QStringLiteral("error")};
    mConfiguration->addItemStringList(QStringLiteral("ErrorKeywords"), mErrorKeywords, defaultErrorKeywords, QStringLiteral("ErrorKeywords"));
}

AuthenticationConfiguration::~AuthenticationConfiguration()
{
}

QString AuthenticationConfiguration::authenticationPath() const
{
    return mAuthenticationPath;
}

void AuthenticationConfiguration::setAuthenticationPath(const QString &authenticationPath)
{
    mAuthenticationPath = authenticationPath;
}

QStringList AuthenticationConfiguration::warningKeywords() const
{
    return mWarningKeywords;
}

QStringList AuthenticationConfiguration::errorKeywords() const
{
    return mErrorKeywords;
}
