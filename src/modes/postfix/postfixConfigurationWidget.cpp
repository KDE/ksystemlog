/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
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

#include "postfixConfigurationWidget.h"

PostfixConfigurationWidget::PostfixConfigurationWidget()
    : LogModeConfigurationWidget(i18n("Postfix Log"), QStringLiteral(POSTFIX_MODE_ICON), i18n("Postfix Log"))
{
    auto *layout = new QVBoxLayout(this);

    const QString description = i18n("<p>These files will be analyzed to show the <b>Postfix Logs</b>.</p>");

    mLileList = new LogLevelFileList(this, description);

    connect(mLileList, &FileList::fileListChanged, this, &LogModeConfigurationWidget::configurationChanged);

    layout->addWidget(mLileList);
}

bool PostfixConfigurationWidget::isValid() const
{
    if (!mLileList->isEmpty()) {
        logDebug() << "Postfix configuration valid";
        return true;
    }

    logDebug() << "Postfix configuration not valid";
    return false;
}

void PostfixConfigurationWidget::saveConfig()
{
    logDebug() << "Saving config from Postfix Options...";

    auto *configuration = Globals::instance().findLogMode(QStringLiteral(POSTFIX_LOG_MODE_ID))->logModeConfiguration<PostfixConfiguration *>();
    configuration->setLogFilesPaths(mLileList->paths());
    configuration->setLogFilesLevels(mLileList->levels());
}

void PostfixConfigurationWidget::readConfig()
{
    auto *configuration = Globals::instance().findLogMode(QStringLiteral(POSTFIX_LOG_MODE_ID))->logModeConfiguration<PostfixConfiguration *>();

    mLileList->removeAllItems();

    mLileList->addPaths(configuration->logFilesPaths(), configuration->logFilesLevels());
}

void PostfixConfigurationWidget::defaultConfig()
{
    // TODO Find a way to read the configuration per default
    readConfig();
}
