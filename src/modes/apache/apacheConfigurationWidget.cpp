/*
    KApacheLog, a apache log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "apacheConfigurationWidget.h"

ApacheConfigurationWidget::ApacheConfigurationWidget()
    : LogModeConfigurationWidget(i18n("Apache Log"), QStringLiteral(APACHE_MODE_ICON), i18n("Apache Log"))
{
    auto layout = new QHBoxLayout(this);

    mApacheFileList = new MultipleFileList(this,
                                           i18n("<p>These files will be analyzed to show the <b>Apache log</b> "
                                                "and the <b>Apache Access log</b>.</p>"));

    mApachePathsId = mApacheFileList->addCategory(i18n("Apache Log Files"), i18n("Add Apache File..."));
    mApacheAccessPathsId = mApacheFileList->addCategory(i18n("Apache Access Log Files"), i18n("Add Apache Access File..."));

    connect(mApacheFileList, &MultipleFileList::fileListChanged, this, &LogModeConfigurationWidget::configurationChanged);

    layout->addWidget(mApacheFileList);
}

void ApacheConfigurationWidget::saveConfig()
{
    logDebug() << "Saving config from Apache Options...";

    auto *apacheConfiguration = Globals::instance().findLogMode(QStringLiteral(APACHE_LOG_MODE_ID))->logModeConfiguration<ApacheConfiguration *>();
    apacheConfiguration->setApachePaths(mApacheFileList->paths(mApachePathsId));
    apacheConfiguration->setApacheAccessPaths(mApacheFileList->paths(mApacheAccessPathsId));
}

void ApacheConfigurationWidget::defaultConfig()
{
    // TODO Find a way to read the configuration per default
    readConfig();
}

void ApacheConfigurationWidget::readConfig()
{
    auto *apacheConfiguration = Globals::instance().findLogMode(QStringLiteral(APACHE_LOG_MODE_ID))->logModeConfiguration<ApacheConfiguration *>();

    mApacheFileList->removeAllItems();

    mApacheFileList->addPaths(mApachePathsId, apacheConfiguration->apachePaths());
    mApacheFileList->addPaths(mApacheAccessPathsId, apacheConfiguration->apacheAccessPaths());
}

bool ApacheConfigurationWidget::isValid() const
{
    if (mApacheFileList->isOneOfCategoryEmpty()) {
        logDebug() << "Apache configuration not valid";
        return false;
    }

    logDebug() << "Apache configuration valid";
    return true;
}
