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

#include "sambaConfigurationWidget.h"

SambaConfigurationWidget::SambaConfigurationWidget()
    : LogModeConfigurationWidget(i18n("Samba Log"), QStringLiteral(SAMBA_MODE_ICON), i18n("Samba Log"))
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    mSambaFileList
        = new MultipleFileList(this, i18n(
                                   "<p>These files will be analyzed to show <b>Samba log</b>, "
                                   "<b>Samba Access log</b> and <b>Netbios log</b>.</p>"));

    mSambaPathsId = mSambaFileList->addCategory(i18n("Samba Log Files"), i18n("Add Samba File..."));
    mSambaAccessPathsId
        = mSambaFileList->addCategory(i18n("Samba Access Log Files"), i18n("Add Samba Access File..."));
    mNetbiosPathsId = mSambaFileList->addCategory(i18n("Netbios Log Files"), i18n("Add Netbios File..."));

    connect(mSambaFileList, &MultipleFileList::fileListChanged, this, &LogModeConfigurationWidget::configurationChanged);

    layout->addWidget(mSambaFileList);
}

void SambaConfigurationWidget::saveConfig()
{
    logDebug() << "Saving config from Samba Options...";

    SambaConfiguration *sambaConfiguration = Globals::instance()
                                             .findLogMode(QStringLiteral(SAMBA_LOG_MODE_ID))
                                             ->logModeConfiguration<SambaConfiguration *>();
    sambaConfiguration->setSambaPaths(mSambaFileList->paths(mSambaPathsId));
    sambaConfiguration->setSambaAccessPaths(mSambaFileList->paths(mSambaAccessPathsId));
    sambaConfiguration->setNetbiosPaths(mSambaFileList->paths(mNetbiosPathsId));
}

void SambaConfigurationWidget::defaultConfig()
{
    // TODO Find a way to read the configuration per default
    readConfig();
}

void SambaConfigurationWidget::readConfig()
{
    SambaConfiguration *sambaConfiguration = Globals::instance()
                                             .findLogMode(QStringLiteral(SAMBA_LOG_MODE_ID))
                                             ->logModeConfiguration<SambaConfiguration *>();

    mSambaFileList->removeAllItems();

    mSambaFileList->addPaths(mSambaPathsId, sambaConfiguration->sambaPaths());
    mSambaFileList->addPaths(mSambaAccessPathsId, sambaConfiguration->sambaAccessPaths());
    mSambaFileList->addPaths(mNetbiosPathsId, sambaConfiguration->netbiosPaths());
}

bool SambaConfigurationWidget::isValid() const
{
    if (mSambaFileList->isOneOfCategoryEmpty()) {
        logDebug() << "Samba configuration not valid";
        return false;
    }

    logDebug() << "Samba configuration valid";
    return true;
}
