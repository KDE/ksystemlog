/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KConfigDialog>

class GeneralConfigurationWidget;
class LogModeConfigurationWidget;
class ConfigurationDialog : public KConfigDialog
{
    Q_OBJECT

public:
    explicit ConfigurationDialog(QWidget *parent);

    ~ConfigurationDialog() override;

    void showConfiguration();

Q_SIGNALS:
    void configurationSaved();

protected Q_SLOTS:

    void updateWidgets() override;
    void updateWidgetsDefault() override;
    void updateSettings() override;
    void updateButtons();

private Q_SLOTS:
    void updateConfiguration();

private:
    bool hasChanged() override;
    bool isDefault() override;

    void setupGeneralConfiguration();

    void setupLogModeConfigurations();
    GeneralConfigurationWidget *mGeneralConfiguration = nullptr;

    QList<LogModeConfigurationWidget *> mLogModeConfigurations;

    bool mChanged = false;
};
