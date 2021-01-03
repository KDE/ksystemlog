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

#ifndef AUTHENTICATION_CONFIGURATION_H
#define AUTHENTICATION_CONFIGURATION_H

#include <QStringList>

#include "logModeConfiguration.h"

#include "logging.h"
#include "defaults.h"

#include "authenticationLogMode.h"

#include "ksystemlogConfig.h"

class AuthenticationConfigurationPrivate
{
public:
    QString authenticationPath;

    QStringList warningKeywords;
    QStringList errorKeywords;
};

class AuthenticationConfiguration : public LogModeConfiguration
{
    Q_OBJECT

public:
    AuthenticationConfiguration()
        : d(new AuthenticationConfigurationPrivate())
    {
        configuration->setCurrentGroup(QStringLiteral("AuthenticationLogMode"));

        QString defaultAuthenticationPath(QStringLiteral("/var/log/auth.log"));
        configuration->addItemString(QStringLiteral("LogFilePath"), d->authenticationPath,
                                     defaultAuthenticationPath, QStringLiteral("LogFilePath"));

        QStringList defaultWarningKeywords;
        defaultWarningKeywords.append(QStringLiteral("failed"));
        configuration->addItemStringList(QStringLiteral("WarningKeywords"), d->warningKeywords,
                                         defaultWarningKeywords, QStringLiteral("WarningKeywords"));

        QStringList defaultErrorKeywords;
        defaultErrorKeywords.append(QStringLiteral("error"));
        configuration->addItemStringList(QStringLiteral("ErrorKeywords"), d->errorKeywords,
                                         defaultErrorKeywords, QStringLiteral("ErrorKeywords"));
    }

    ~AuthenticationConfiguration() override { delete d; }

    QString authenticationPath() const { return d->authenticationPath; }

    void setAuthenticationPath(const QString &authenticationPath)
    {
        d->authenticationPath = authenticationPath;
    }

    QStringList warningKeywords() const { return d->warningKeywords; }

    QStringList errorKeywords() const { return d->errorKeywords; }

private:
    AuthenticationConfigurationPrivate *const d;
};

#endif // _AUTHENTICATION_CONFIGURATION_H
