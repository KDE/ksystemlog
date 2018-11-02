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

#ifndef POSTFIX_CONFIGURATION_WIDGET_H
#define POSTFIX_CONFIGURATION_WIDGET_H

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
    PostfixConfigurationWidget()
        : LogModeConfigurationWidget(i18n("Postfix Log"), QStringLiteral(POSTFIX_MODE_ICON),
                                     i18n("Postfix Log"))
    {
        QVBoxLayout *layout = new QVBoxLayout(this);

        QString description = i18n("<p>These files will be analyzed to show the <b>Postfix Logs</b>.</p>");

        fileList = new LogLevelFileList(this, description);

        connect(fileList, &FileList::fileListChanged, this, &LogModeConfigurationWidget::configurationChanged);

        layout->addWidget(fileList);
    }

    ~PostfixConfigurationWidget() override {}

    bool isValid() const override
    {
        if (fileList->isEmpty() == false) {
            logDebug() << "Postfix configuration valid";
            return true;
        }

        logDebug() << "Postfix configuration not valid";
        return false;
    }

    void saveConfig() override
    {
        logDebug() << "Saving config from Postfix Options...";

        PostfixConfiguration *configuration = Globals::instance()
                                                  .findLogMode(QStringLiteral(POSTFIX_LOG_MODE_ID))
                                                  ->logModeConfiguration<PostfixConfiguration *>();
        configuration->setLogFilesPaths(fileList->paths());
        configuration->setLogFilesLevels(fileList->levels());
    }

    void readConfig() override
    {
        PostfixConfiguration *configuration = Globals::instance()
                                                  .findLogMode(QStringLiteral(POSTFIX_LOG_MODE_ID))
                                                  ->logModeConfiguration<PostfixConfiguration *>();

        fileList->removeAllItems();

        fileList->addPaths(configuration->logFilesPaths(), configuration->logFilesLevels());
    }

    void defaultConfig() override
    {
        // TODO Find a way to read the configuration per default
        readConfig();
    }

private:
    LogLevelFileList *fileList;
};

#endif // _POSTFIX_CONFIGURATION_WIDGET_H
