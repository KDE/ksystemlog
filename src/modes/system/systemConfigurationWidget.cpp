/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "systemConfigurationWidget.h"

SystemConfigurationWidget::SystemConfigurationWidget()
    : LogModeConfigurationWidget(i18n("System Log"), QStringLiteral(SYSTEM_MODE_ICON), i18n("System Log"))
{
    auto layout = new QVBoxLayout(this);

    const QString description = i18n("<p>These files will be analyzed to show the <b>System logs</b>.</p>");

    mFileList = new LogLevelFileList(this, description);

    connect(mFileList, &FileList::fileListChanged, this, &LogModeConfigurationWidget::configurationChanged);

    layout->addWidget(mFileList);
}

bool SystemConfigurationWidget::isValid() const
{
    if (!mFileList->isEmpty()) {
        qCDebug(KSYSTEMLOG) << "System configuration valid";
        return true;
    }

    qCDebug(KSYSTEMLOG) << "System configuration not valid";
    return false;
}

void SystemConfigurationWidget::saveConfig()
{
    qCDebug(KSYSTEMLOG) << "Saving config from System Options...";

    auto *systemConfiguration = Globals::instance().findLogMode(QStringLiteral(SYSTEM_LOG_MODE_ID))->logModeConfiguration<SystemConfiguration *>();
    systemConfiguration->setLogFilesPaths(mFileList->paths());
    systemConfiguration->setLogFilesLevels(mFileList->levels());
}

void SystemConfigurationWidget::readConfig()
{
    auto *systemConfiguration = Globals::instance().findLogMode(QStringLiteral(SYSTEM_LOG_MODE_ID))->logModeConfiguration<SystemConfiguration *>();

    mFileList->removeAllItems();

    mFileList->addPaths(systemConfiguration->logFilesPaths(), systemConfiguration->logFilesLevels());
}

void SystemConfigurationWidget::defaultConfig()
{
    // TODO Find a way to read the configuration per default
    readConfig();
}
