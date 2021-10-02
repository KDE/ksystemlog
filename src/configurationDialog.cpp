/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "configurationDialog.h"

#include <QList>
#include <QPushButton>

#include <KLocalizedString>

#include "generalConfigurationWidget.h"

#include "logModeConfigurationWidget.h"

#include "mainWindow.h"

#include "globals.h"
#include "ksystemlogConfig.h"

#include "defaults.h"
#include "logging.h"

ConfigurationDialog::ConfigurationDialog(QWidget *parent)
    : KConfigDialog(parent, i18n("Settings"), KSystemLogConfig::self())
{
    setupGeneralConfiguration();

    setupLogModeConfigurations();
}

ConfigurationDialog::~ConfigurationDialog()
{
}

void ConfigurationDialog::setupLogModeConfigurations()
{
    logDebug() << "Setup Log Mode Configurations...";

    const auto logModes = Globals::instance().logModes();
    for (LogMode *logMode : logModes) {
        // Some Log mode does not need a configuration widget
        if (!logMode->logModeConfigurationWidget()) {
            continue;
        }

        // The configuration widget could be shared between Log Modes
        if (mLogModeConfigurations.contains(logMode->logModeConfigurationWidget())) {
            continue;
        }

        mLogModeConfigurations.append(logMode->logModeConfigurationWidget());
    }

    for (LogModeConfigurationWidget *logModeConfigurationWidget : std::as_const(mLogModeConfigurations)) {
        logDebug() << "Adding " << logModeConfigurationWidget->itemName() << " configuration...";

        addPage(logModeConfigurationWidget,
                logModeConfigurationWidget->itemName(),
                logModeConfigurationWidget->iconName(),
                logModeConfigurationWidget->header(),
                false);

        connect(logModeConfigurationWidget, &LogModeConfigurationWidget::configurationChanged, this, &ConfigurationDialog::updateConfiguration);
    }
}

void ConfigurationDialog::showConfiguration()
{
    logDebug() << "Showing Configuration dialog...";

    show();
}

void ConfigurationDialog::setupGeneralConfiguration()
{
    mGeneralConfiguration = new GeneralConfigurationWidget();

    addPage(mGeneralConfiguration, i18n("General"), QStringLiteral("utilities-log-viewer"), i18n("General"), false);

    connect(mGeneralConfiguration, &GeneralConfigurationWidget::configurationChanged, this, &ConfigurationDialog::updateConfiguration);
}

void ConfigurationDialog::updateSettings()
{
    logDebug() << "Saving configuration...";

    mChanged = false;

    mGeneralConfiguration->saveConfig();

    for (LogModeConfigurationWidget *logModeConfigurationWidget : std::as_const(mLogModeConfigurations)) {
        logModeConfigurationWidget->saveConfig();
    }

    KSystemLogConfig::self()->save();

    Q_EMIT configurationSaved();

    logDebug() << "Configuration saved";
}

bool ConfigurationDialog::hasChanged()
{
    logDebug() << "Current change status : " << mChanged;
    return mChanged;
}

void ConfigurationDialog::updateConfiguration()
{
    logDebug() << "Updating configuration...";

    bool valid = mGeneralConfiguration->isValid();
    if (valid) {
        for (LogModeConfigurationWidget *logModeConfigurationWidget : std::as_const(mLogModeConfigurations)) {
            if (!logModeConfigurationWidget->isValid()) {
                valid = false;
                break;
            }
        }
    }

    if (valid) {
        buttonBox()->button(QDialogButtonBox::Ok)->setEnabled(true);

        updateButtons();
    } else {
        buttonBox()->button(QDialogButtonBox::Ok)->setEnabled(false);
        buttonBox()->button(QDialogButtonBox::Apply)->setEnabled(false);
    }
}

void ConfigurationDialog::updateButtons()
{
    logDebug() << "Updating configuration buttons...";

    mChanged = true;
}

void ConfigurationDialog::updateWidgets()
{
    logDebug() << "Reading configuration...";

    mGeneralConfiguration->readConfig();
    for (LogModeConfigurationWidget *logModeConfigurationWidget : std::as_const(mLogModeConfigurations)) {
        logModeConfigurationWidget->readConfig();
    }

    mChanged = false;
}

void ConfigurationDialog::updateWidgetsDefault()
{
    logDebug() << "Loading default configuration...";

    mGeneralConfiguration->defaultConfig();
    for (LogModeConfigurationWidget *logModeConfigurationWidget : std::as_const(mLogModeConfigurations)) {
        logModeConfigurationWidget->defaultConfig();
    }

    mChanged = false;
}

bool ConfigurationDialog::isDefault()
{
    /**
     * TODO Set this to true and find a way to retrieve defaults value
     * of the configuration (see defaultConfig() methods of LogModeConfigurations)
     */

    return true;
}
