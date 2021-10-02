/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#include "postfixConfigurationWidget.h"

PostfixConfigurationWidget::PostfixConfigurationWidget()
    : LogModeConfigurationWidget(i18n("Postfix Log"), QStringLiteral(POSTFIX_MODE_ICON), i18n("Postfix Log"))
{
    auto layout = new QVBoxLayout(this);

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
