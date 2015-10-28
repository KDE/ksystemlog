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

#ifndef _SAMBA_CONFIGURATION_WIDGET_H_
#define _SAMBA_CONFIGURATION_WIDGET_H_

#include "logModeConfigurationWidget.h"

#include <KLocalizedString>

#include "globals.h"
#include "logging.h"
#include "multipleFileList.h"

#include "logLevel.h"

#include "sambaConfiguration.h"
#include "sambaLogMode.h"

class SambaConfigurationWidget : public LogModeConfigurationWidget
{
    Q_OBJECT

public:
    SambaConfigurationWidget()
        : LogModeConfigurationWidget(i18n("Samba Log"), QStringLiteral(SAMBA_MODE_ICON), i18n("Samba Log"))
    {
        QHBoxLayout *layout = new QHBoxLayout();
        this->setLayout(layout);

        sambaFileList
            = new MultipleFileList(this, i18n(
                                             "<p>These files will be analyzed to show <b>Samba log</b>, "
                                             "<b>Samba Access log</b> and <b>Netbios log</b>.</p>"));

        sambaPathsId = sambaFileList->addCategory(i18n("Samba Log Files"), i18n("Add Samba File..."));
        sambaAccessPathsId
            = sambaFileList->addCategory(i18n("Samba Access Log Files"), i18n("Add Samba Access File..."));
        netbiosPathsId = sambaFileList->addCategory(i18n("Netbios Log Files"), i18n("Add Netbios File..."));

        connect(sambaFileList, &MultipleFileList::fileListChanged, this, &LogModeConfigurationWidget::configurationChanged);

        layout->addWidget(sambaFileList);
    }

    ~SambaConfigurationWidget() {}

public slots:

    void saveConfig()
    {
        logDebug() << "Saving config from Samba Options...";

        SambaConfiguration *sambaConfiguration = Globals::instance()
                                                     .findLogMode(QStringLiteral(SAMBA_LOG_MODE_ID))
                                                     ->logModeConfiguration<SambaConfiguration *>();
        sambaConfiguration->setSambaPaths(sambaFileList->paths(sambaPathsId));
        sambaConfiguration->setSambaAccessPaths(sambaFileList->paths(sambaAccessPathsId));
        sambaConfiguration->setNetbiosPaths(sambaFileList->paths(netbiosPathsId));
    }

    void defaultConfig()
    {
        // TODO Find a way to read the configuration per default
        readConfig();
    }

    void readConfig()
    {
        SambaConfiguration *sambaConfiguration = Globals::instance()
                                                     .findLogMode(QStringLiteral(SAMBA_LOG_MODE_ID))
                                                     ->logModeConfiguration<SambaConfiguration *>();

        sambaFileList->removeAllItems();

        sambaFileList->addPaths(sambaPathsId, sambaConfiguration->sambaPaths());
        sambaFileList->addPaths(sambaAccessPathsId, sambaConfiguration->sambaAccessPaths());
        sambaFileList->addPaths(netbiosPathsId, sambaConfiguration->netbiosPaths());
    }

protected:
    bool isValid() const
    {
        if (sambaFileList->isOneOfCategoryEmpty() == true) {
            logDebug() << "Samba configuration not valid";
            return false;
        }

        logDebug() << "Samba configuration valid";
        return true;
    }

private:
    MultipleFileList *sambaFileList;

    int sambaPathsId;
    int sambaAccessPathsId;
    int netbiosPathsId;
};

#endif // _SAMBA_CONFIGURATION_WIDGET_H_
