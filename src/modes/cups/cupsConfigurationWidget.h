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

#ifndef _CUPS_CONFIGURATION_WIDGET_H_
#define _CUPS_CONFIGURATION_WIDGET_H_

#include "logModeConfigurationWidget.h"

#include <KLocalizedString>

#include "globals.h"
#include "logging.h"
#include "multipleFileList.h"

#include "logLevel.h"

#include "cupsConfiguration.h"
#include "cupsLogMode.h"

class CupsConfigurationWidget : public LogModeConfigurationWidget
{
    Q_OBJECT

public:
    CupsConfigurationWidget()
        : LogModeConfigurationWidget(i18n("Cups Log"), QLatin1String(CUPS_MODE_ICON),
                                     i18n("Cups &amp; Cups Web Server Log"))
    {
        QHBoxLayout *layout = new QHBoxLayout();
        this->setLayout(layout);

        cupsFileList = new MultipleFileList(this, i18n(
                                                      "<p>These files will be analyzed to show the <b>Cups "
                                                      "log</b> and the <b>Cups Web Access log</b>.</p>"));

        cupsPathsId = cupsFileList->addCategory(i18n("Cups Log Files"), i18n("Add Cups File..."));
        cupsAccessPathsId
            = cupsFileList->addCategory(i18n("Cups Access Log Files"), i18n("Add Cups Access File..."));
        cupsPagePathsId
            = cupsFileList->addCategory(i18n("Cups Page Log Files"), i18n("Add Cups Page File..."));
        cupsPdfPathsId = cupsFileList->addCategory(i18n("Cups PDF Log Files"), i18n("Add Cups PDF File..."));

        connect(cupsFileList, SIGNAL(fileListChanged()), this, SIGNAL(configurationChanged()));

        layout->addWidget(cupsFileList);
    }

    ~CupsConfigurationWidget() {}

public slots:

    void saveConfig()
    {
        logDebug() << "Saving config from Cups Options...";

        CupsConfiguration *cupsConfiguration = Globals::instance()
                                                   .findLogMode(QLatin1String(CUPS_LOG_MODE_ID))
                                                   ->logModeConfiguration<CupsConfiguration *>();
        cupsConfiguration->setCupsPaths(cupsFileList->paths(cupsPathsId));
        cupsConfiguration->setCupsAccessPaths(cupsFileList->paths(cupsAccessPathsId));
        cupsConfiguration->setCupsPagePaths(cupsFileList->paths(cupsPagePathsId));
        cupsConfiguration->setCupsPdfPaths(cupsFileList->paths(cupsPdfPathsId));
    }

    void defaultConfig()
    {
        // TODO Find a way to read the configuration per default
        readConfig();
    }

    void readConfig()
    {
        CupsConfiguration *cupsConfiguration = Globals::instance()
                                                   .findLogMode(QLatin1String(CUPS_LOG_MODE_ID))
                                                   ->logModeConfiguration<CupsConfiguration *>();

        cupsFileList->removeAllItems();

        cupsFileList->addPaths(cupsPathsId, cupsConfiguration->cupsPaths());
        cupsFileList->addPaths(cupsAccessPathsId, cupsConfiguration->cupsAccessPaths());
        cupsFileList->addPaths(cupsPagePathsId, cupsConfiguration->cupsPagePaths());
        cupsFileList->addPaths(cupsPdfPathsId, cupsConfiguration->cupsPdfPaths());
    }

protected:
    bool isValid() const
    {
        if (cupsFileList->isOneOfCategoryEmpty() == true) {
            logDebug() << "Cups configuration not valid";
            return false;
        }

        logDebug() << "Cups configuration valid";
        return true;
    }

private:
    MultipleFileList *cupsFileList;

    int cupsPathsId;
    int cupsAccessPathsId;
    int cupsPagePathsId;
    int cupsPdfPathsId;
};

#endif // _CUPS_CONFIGURATION_WIDGET_H_
