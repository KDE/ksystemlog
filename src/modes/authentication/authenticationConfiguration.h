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

#ifndef _AUTHENTICATION_CONFIGURATION_H_
#define _AUTHENTICATION_CONFIGURATION_H_

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
        configuration->setCurrentGroup(QLatin1String("AuthenticationLogMode"));

        QString defaultAuthenticationPath(QLatin1String("/var/log/auth.log"));
        configuration->addItemString(QLatin1String("LogFilePath"), d->authenticationPath,
                                     defaultAuthenticationPath, QLatin1String("LogFilePath"));

        QStringList defaultWarningKeywords;
        defaultWarningKeywords.append(QLatin1String("failed"));
        configuration->addItemStringList(QLatin1String("WarningKeywords"), d->warningKeywords,
                                         defaultWarningKeywords, QLatin1String("WarningKeywords"));

        QStringList defaultErrorKeywords;
        defaultErrorKeywords.append(QLatin1String("error"));
        configuration->addItemStringList(QLatin1String("ErrorKeywords"), d->errorKeywords,
                                         defaultErrorKeywords, QLatin1String("ErrorKeywords"));
    }

    virtual ~AuthenticationConfiguration() { delete d; }

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

#endif // _AUTHENTICATION_CONFIGURATION_H_
