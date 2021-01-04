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

#include "cupsConfigurationWidget.h"

CupsConfigurationWidget::CupsConfigurationWidget()
    : LogModeConfigurationWidget(i18n("Cups Log"), QStringLiteral(CUPS_MODE_ICON),
                                 i18n("Cups &amp; Cups Web Server Log"))
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    cupsFileList = new MultipleFileList(this, i18n(
                                            "<p>These files will be analyzed to show the <b>Cups "
                                                      "log</b> and the <b>Cups Web Access log</b>.</p>"));

    cupsPathsId = cupsFileList->addCategory(i18n("Cups Log Files"), i18n("Add Cups File..."));
    cupsAccessPathsId
            = cupsFileList->addCategory(i18n("Cups Access Log Files"), i18n("Add Cups Access File..."));
    cupsPagePathsId
            = cupsFileList->addCategory(i18n("Cups Page Log Files"), i18n("Add Cups Page File..."));
    cupsPdfPathsId = cupsFileList->addCategory(i18n("Cups PDF Log Files"), i18n("Add Cups PDF File..."));

    connect(cupsFileList, &MultipleFileList::fileListChanged, this, &LogModeConfigurationWidget::configurationChanged);

    layout->addWidget(cupsFileList);
}

void CupsConfigurationWidget::saveConfig()
{
    logDebug() << "Saving config from Cups Options...";

    CupsConfiguration *cupsConfiguration = Globals::instance()
            .findLogMode(QStringLiteral(CUPS_LOG_MODE_ID))
            ->logModeConfiguration<CupsConfiguration *>();
    cupsConfiguration->setCupsPaths(cupsFileList->paths(cupsPathsId));
    cupsConfiguration->setCupsAccessPaths(cupsFileList->paths(cupsAccessPathsId));
    cupsConfiguration->setCupsPagePaths(cupsFileList->paths(cupsPagePathsId));
    cupsConfiguration->setCupsPdfPaths(cupsFileList->paths(cupsPdfPathsId));
}

void CupsConfigurationWidget::defaultConfig()
{
    // TODO Find a way to read the configuration per default
    readConfig();
}

void CupsConfigurationWidget::readConfig()
{
    CupsConfiguration *cupsConfiguration = Globals::instance()
            .findLogMode(QStringLiteral(CUPS_LOG_MODE_ID))
            ->logModeConfiguration<CupsConfiguration *>();

    cupsFileList->removeAllItems();

    cupsFileList->addPaths(cupsPathsId, cupsConfiguration->cupsPaths());
    cupsFileList->addPaths(cupsAccessPathsId, cupsConfiguration->cupsAccessPaths());
    cupsFileList->addPaths(cupsPagePathsId, cupsConfiguration->cupsPagePaths());
    cupsFileList->addPaths(cupsPdfPathsId, cupsConfiguration->cupsPdfPaths());
}

bool CupsConfigurationWidget::isValid() const
{
    if (cupsFileList->isOneOfCategoryEmpty() == true) {
        logDebug() << "Cups configuration not valid";
        return false;
    }

    logDebug() << "Cups configuration valid";
    return true;
}
