/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "ui_generalConfigurationWidgetBase.h"
class KMessageWidget;
class GeneralConfigurationWidget : public QWidget, public Ui::GeneralConfigurationWidgetBase
{
    Q_OBJECT

public:
    GeneralConfigurationWidget();

    ~GeneralConfigurationWidget() override;

    bool isValid() const;

public Q_SLOTS:
    void saveConfig() const;

    void defaultConfig();

    void readConfig();

Q_SIGNALS:
    void configurationChanged();

private:
    void addDateFormatExample();
    QButtonGroup *mDateFormatGroup = nullptr;
    KMessageWidget *mWarningBox = nullptr;
};
