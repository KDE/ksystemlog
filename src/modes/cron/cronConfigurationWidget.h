/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "logModeConfigurationWidget.h"

#include <QLabel>
#include <QLineEdit>

#include <KLocalizedString>

#include "globals.h"
#include "ksystemlog_debug.h"

#include "fileList.h"

#include "logLevel.h"

#include "cronConfiguration.h"
#include "cronLogMode.h"

class CronConfigurationWidget : public LogModeConfigurationWidget
{
    Q_OBJECT

public:
    CronConfigurationWidget();

    ~CronConfigurationWidget() override
    {
    }

    bool isValid() const override;

    void saveConfig() override;

    void readConfig() override;

    void defaultConfig() override;

private Q_SLOTS:
    void toggleProcessFilterEnabling(bool enabled);

private:
    FileList *mFileList = nullptr;

    QGroupBox *mProcessFilterGroup = nullptr;

    QLineEdit *mProcessFilter = nullptr;
    QLabel *mProcessFilterLabel = nullptr;
};
