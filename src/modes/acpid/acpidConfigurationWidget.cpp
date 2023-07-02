/*
    KApacheLog, a apache log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "acpidConfigurationWidget.h"

AcpidConfigurationWidget::AcpidConfigurationWidget()
    : LogModeConfigurationWidget(i18n("Acpid Log"), QStringLiteral(ACPID_MODE_ICON), i18n("Acpid Log"))
{
    auto layout = new QHBoxLayout(this);

    mFileList = new FileList(this, i18n("<p>These files will be analyzed to show the <b>Acpid log</b>.</p>"));
    connect(mFileList, &FileList::fileListChanged, this, &LogModeConfigurationWidget::configurationChanged);
    layout->addWidget(mFileList);
}

void AcpidConfigurationWidget::saveConfig()
{
    auto *acpidConfiguration = Globals::instance().findLogMode(QStringLiteral(ACPID_LOG_MODE_ID))->logModeConfiguration<AcpidConfiguration *>();

    acpidConfiguration->setAcpidPaths(mFileList->paths());
}

void AcpidConfigurationWidget::readConfig()
{
    auto *acpidConfiguration = Globals::instance().findLogMode(QStringLiteral(ACPID_LOG_MODE_ID))->logModeConfiguration<AcpidConfiguration *>();

    mFileList->removeAllItems();

    mFileList->addPaths(acpidConfiguration->acpidPaths());
}

void AcpidConfigurationWidget::defaultConfig()
{
    // TODO Find a way to read the configuration per default
    readConfig();
}

bool AcpidConfigurationWidget::isValid() const
{
    if (!mFileList->isEmpty()) {
        return true;
    }

    return false;
}

#include "moc_acpidConfigurationWidget.cpp"
