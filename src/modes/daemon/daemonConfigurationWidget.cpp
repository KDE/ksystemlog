/***************************************************************************
 *   KApacheLog, a apache log viewer tool                                  *
 *   Copyright (C) 2007 by Nicolas Ternisien                               *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

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
