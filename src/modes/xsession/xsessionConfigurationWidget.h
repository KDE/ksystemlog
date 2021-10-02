/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QCheckBox>

#include <KLocalizedString>
#include <KUrlRequester>

#include "fileList.h"
#include "globals.h"
#include "ksystemlog_debug.h"

#include "logLevel.h"

#include "xsessionConfiguration.h"
#include "xsessionLogMode.h"

#include "logModeConfigurationWidget.h"

#include "ui_xsessionConfigurationWidgetBase.h"

class XSessionConfigurationWidget : public LogModeConfigurationWidget, public Ui::XSessionConfigurationWidgetBase
{
    Q_OBJECT

public:
    XSessionConfigurationWidget();

    ~XSessionConfigurationWidget() override
    {
    }

public Q_SLOTS:

    void saveConfig() override;

    void readConfig() override;

    void defaultConfig() override;

protected:
    bool isValid() const override;

private:
    void prepareXorgErrorsDescription();

    bool mXorgErrorsDescriptionDefined;

    KMessageWidget *mWarningBox = nullptr;
};

