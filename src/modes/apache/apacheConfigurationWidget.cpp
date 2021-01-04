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

#include "apacheConfigurationWidget.h"

ApacheConfigurationWidget::ApacheConfigurationWidget()
    : LogModeConfigurationWidget(i18n("Apache Log"), QStringLiteral(APACHE_MODE_ICON), i18n("Apache Log"))
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    apacheFileList
            = new MultipleFileList(this, i18n(
                                       "<p>These files will be analyzed to show the <b>Apache log</b> "
                                             "and the <b>Apache Access log</b>.</p>"));

    apachePathsId = apacheFileList->addCategory(i18n("Apache Log Files"), i18n("Add Apache File..."));
    apacheAccessPathsId
            = apacheFileList->addCategory(i18n("Apache Access Log Files"), i18n("Add Apache Access File..."));

    connect(apacheFileList, &MultipleFileList::fileListChanged, this, &LogModeConfigurationWidget::configurationChanged);

    layout->addWidget(apacheFileList);
}

void ApacheConfigurationWidget::saveConfig()
{
    logDebug() << "Saving config from Apache Options...";

    ApacheConfiguration *apacheConfiguration = Globals::instance()
            .findLogMode(QStringLiteral(APACHE_LOG_MODE_ID))
            ->logModeConfiguration<ApacheConfiguration *>();
    apacheConfiguration->setApachePaths(apacheFileList->paths(apachePathsId));
    apacheConfiguration->setApacheAccessPaths(apacheFileList->paths(apacheAccessPathsId));
}

void ApacheConfigurationWidget::defaultConfig()
{
    // TODO Find a way to read the configuration per default
    readConfig();
}

void ApacheConfigurationWidget::readConfig()
{
    ApacheConfiguration *apacheConfiguration = Globals::instance()
            .findLogMode(QStringLiteral(APACHE_LOG_MODE_ID))
            ->logModeConfiguration<ApacheConfiguration *>();

    apacheFileList->removeAllItems();

    apacheFileList->addPaths(apachePathsId, apacheConfiguration->apachePaths());
    apacheFileList->addPaths(apacheAccessPathsId, apacheConfiguration->apacheAccessPaths());
}

bool ApacheConfigurationWidget::isValid() const
{
    if (apacheFileList->isOneOfCategoryEmpty() == true) {
        logDebug() << "Apache configuration not valid";
        return false;
    }

    logDebug() << "Apache configuration valid";
    return true;
}
