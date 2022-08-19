/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "logModeConfigurationWidget.h"

#include <KLocalizedString>

#include "globals.h"
#include "ksystemlog_debug.h"
#include "multipleFileList.h"

#include "logLevel.h"

#include "apacheConfiguration.h"
#include "apacheLogMode.h"

class ApacheConfigurationWidget : public LogModeConfigurationWidget
{
    Q_OBJECT

public:
    ApacheConfigurationWidget();

    ~ApacheConfigurationWidget() override
    {
    }

public Q_SLOTS:

    void saveConfig() override;

    void defaultConfig() override;

    void readConfig() override;

protected:
    bool isValid() const override;

private:
    MultipleFileList *mApacheFileList = nullptr;

    int mApachePathsId = -1;
    int mApacheAccessPathsId = -1;
};
