/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#pragma once

#include <QStringList>

#include "logModeConfiguration.h"

#include "defaults.h"
#include "logging.h"

#include "authenticationLogMode.h"

#include "ksystemlogConfig.h"

class AuthenticationConfiguration : public LogModeConfiguration
{
    Q_OBJECT

public:
    AuthenticationConfiguration();

    ~AuthenticationConfiguration() override;

    QString authenticationPath() const;

    void setAuthenticationPath(const QString &authenticationPath);

    QStringList warningKeywords() const;

    QStringList errorKeywords() const;

private:
    QString mAuthenticationPath;

    QStringList mWarningKeywords;
    QStringList mErrorKeywords;
};

