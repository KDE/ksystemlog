/*
    KApacheLog, a apache log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sambaConfigurationWidget.h"

SambaConfigurationWidget::SambaConfigurationWidget()
    : LogModeConfigurationWidget(i18n("Samba Log"), QStringLiteral(SAMBA_MODE_ICON), i18n("Samba Log"))
{
    auto layout = new QHBoxLayout(this);

    mSambaFileList = new MultipleFileList(this,
                                          i18n("<p>These files will be analyzed to show <b>Samba log</b>, "
                                               "<b>Samba Access log</b> and <b>Netbios log</b>.</p>"));

    mSambaPathsId = mSambaFileList->addCategory(i18n("Samba Log Files"), i18n("Add Samba File..."));
    mSambaAccessPathsId = mSambaFileList->addCategory(i18n("Samba Access Log Files"), i18n("Add Samba Access File..."));
    mNetbiosPathsId = mSambaFileList->addCategory(i18n("Netbios Log Files"), i18n("Add Netbios File..."));

    connect(mSambaFileList, &MultipleFileList::fileListChanged, this, &LogModeConfigurationWidget::configurationChanged);

    layout->addWidget(mSambaFileList);
}

void SambaConfigurationWidget::saveConfig()
{
    logDebug() << "Saving config from Samba Options...";

    auto *sambaConfiguration = Globals::instance().findLogMode(QStringLiteral(SAMBA_LOG_MODE_ID))->logModeConfiguration<SambaConfiguration *>();
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
    auto *sambaConfiguration = Globals::instance().findLogMode(QStringLiteral(SAMBA_LOG_MODE_ID))->logModeConfiguration<SambaConfiguration *>();

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
