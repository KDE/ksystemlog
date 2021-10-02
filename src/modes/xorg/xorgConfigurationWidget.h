/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KLocalizedString>

#include "fileList.h"
#include "globals.h"
#include "ksystemlog_debug.h"

#include "logLevel.h"
#include "logModeConfigurationWidget.h"

#include "xorgConfiguration.h"
#include "xorgLogMode.h"

class FileList;

class XorgConfigurationWidget : public LogModeConfigurationWidget
{
    Q_OBJECT

public:
    XorgConfigurationWidget();

    ~XorgConfigurationWidget() override
    {
    }

public Q_SLOTS:

    void saveConfig() override;

    void readConfig() override;

    void defaultConfig() override;

protected:
    bool isValid() const override;

private:
    FileList *mFileList = nullptr;
};

