/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#pragma once

#include <KLocalizedString>

#include "fileList.h"
#include "globals.h"
#include "logging.h"

#include "logLevel.h"

#include "acpidConfiguration.h"
#include "acpidLogMode.h"

#include "logModeConfigurationWidget.h"

class FileList;

class AcpidConfigurationWidget : public LogModeConfigurationWidget
{
    Q_OBJECT

public:
    AcpidConfigurationWidget();

    ~AcpidConfigurationWidget() override
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

