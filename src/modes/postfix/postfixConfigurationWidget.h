/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#pragma once

#include "logModeConfigurationWidget.h"

#include <QVBoxLayout>

#include <KLocalizedString>

#include "globals.h"
#include "logging.h"

#include "logLevelFileList.h"

#include "logLevel.h"

#include "postfixConfiguration.h"

#include "postfixLogMode.h"

class PostfixConfigurationWidget : public LogModeConfigurationWidget
{
    Q_OBJECT

public:
    PostfixConfigurationWidget();

    ~PostfixConfigurationWidget() override
    {
    }

    bool isValid() const override;

    void saveConfig() override;

    void readConfig() override;

    void defaultConfig() override;

private:
    LogLevelFileList *mLileList = nullptr;
};

