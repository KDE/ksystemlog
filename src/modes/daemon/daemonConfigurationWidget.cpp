/*
    KApacheLog, a apache log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "daemonConfigurationWidget.h"

DaemonConfigurationWidget::DaemonConfigurationWidget()
    : LogModeConfigurationWidget(i18n("Daemons' Logs"), QStringLiteral(DAEMON_MODE_ICON), i18n("Daemons' Logs"))
{
    auto layout = new QHBoxLayout(this);

    mFileList = new FileList(this, i18n("<p>These files will be analyzed to show the <b>Daemons' Logs</b>.</p>"));
    connect(mFileList, &FileList::fileListChanged, this, &LogModeConfigurationWidget::configurationChanged);
    layout->addWidget(mFileList);
}

void DaemonConfigurationWidget::saveConfig()
{
    auto *daemonConfiguration = Globals::instance().findLogMode(QStringLiteral(DAEMON_LOG_MODE_ID))->logModeConfiguration<DaemonConfiguration *>();

    daemonConfiguration->setDaemonPaths(mFileList->paths());
}

void DaemonConfigurationWidget::readConfig()
{
    auto *daemonConfiguration = Globals::instance().findLogMode(QStringLiteral(DAEMON_LOG_MODE_ID))->logModeConfiguration<DaemonConfiguration *>();

    mFileList->removeAllItems();

    mFileList->addPaths(daemonConfiguration->daemonPaths());
}

void DaemonConfigurationWidget::defaultConfig()
{
    // TODO Find a way to read the configuration per default
    readConfig();
}

bool DaemonConfigurationWidget::isValid() const
{
    if (!mFileList->isEmpty()) {
        return true;
    }

    return false;
}

#include "moc_daemonConfigurationWidget.cpp"
