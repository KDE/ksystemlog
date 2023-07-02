/*
    KApacheLog, a apache log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "xorgConfigurationWidget.h"

XorgConfigurationWidget::XorgConfigurationWidget()
    : LogModeConfigurationWidget(i18n("X.org Log"), QStringLiteral(XORG_MODE_ICON), i18n("X.org Log"))
{
    auto layout = new QHBoxLayout(this);

    mFileList = new FileList(this, i18n("<p>These files will be analyzed to show the <b>X.org log</b>.</p>"));
    connect(mFileList, &FileList::fileListChanged, this, &LogModeConfigurationWidget::configurationChanged);
    layout->addWidget(mFileList);
}

void XorgConfigurationWidget::saveConfig()
{
    auto *xorgConfiguration = Globals::instance().findLogMode(QStringLiteral(XORG_LOG_MODE_ID))->logModeConfiguration<XorgConfiguration *>();

    xorgConfiguration->setXorgPaths(mFileList->paths());
}

void XorgConfigurationWidget::readConfig()
{
    auto *xorgConfiguration = Globals::instance().findLogMode(QStringLiteral(XORG_LOG_MODE_ID))->logModeConfiguration<XorgConfiguration *>();

    mFileList->removeAllItems();

    mFileList->addPaths(xorgConfiguration->xorgPaths());
}

void XorgConfigurationWidget::defaultConfig()
{
    // TODO Find a way to read the configuration per default
    readConfig();
}

bool XorgConfigurationWidget::isValid() const
{
    if (!mFileList->isEmpty()) {
        return true;
    }

    return false;
}

#include "moc_xorgConfigurationWidget.cpp"
