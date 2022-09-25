/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KLocalizedString>
#include <KUrlRequester>

#include "fileList.h"
#include "globals.h"
#include "ksystemlog_debug.h"

#include "logLevel.h"

#include "authenticationConfiguration.h"
#include "authenticationLogMode.h"

#include "logModeConfigurationWidget.h"

class AuthenticationConfigurationWidget : public LogModeConfigurationWidget
{
    Q_OBJECT

public:
    AuthenticationConfigurationWidget();

    ~AuthenticationConfigurationWidget() override
    {
    }

public Q_SLOTS:

    void saveConfig() override;

    void readConfig() override;

    void defaultConfig() override;

protected:
    bool isValid() const override;

private:
    KUrlRequester *mAuthenticationUrlRequester = nullptr;
    KMessageWidget *mWarningBox = nullptr;
};
