/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "logModeConfigurationWidget.h"

#include <KLocalizedString>

#include "globals.h"
#include "logging.h"
#include "multipleFileList.h"

#include "logLevel.h"

#include "sambaConfiguration.h"
#include "sambaLogMode.h"

class SambaConfigurationWidget : public LogModeConfigurationWidget
{
    Q_OBJECT

public:
    SambaConfigurationWidget();

    ~SambaConfigurationWidget() override
    {
    }

public Q_SLOTS:

    void saveConfig() override;

    void defaultConfig() override;

    void readConfig() override;

protected:
    bool isValid() const override;

private:
    MultipleFileList *mSambaFileList = nullptr;

    int mSambaPathsId;
    int mSambaAccessPathsId;
    int mNetbiosPathsId;
};

