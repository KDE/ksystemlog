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

#include "cronConfigurationWidget.h"

CronConfigurationWidget::CronConfigurationWidget()
    : LogModeConfigurationWidget(i18n("Cron Log"), QStringLiteral(CRON_MODE_ICON), i18n("Cron Log"))
{
    auto *layout = new QVBoxLayout(this);

    const QString description = i18n(
        "<p>These files will be analyzed to show the <b>Cron Logs</b> (i.e. planned tasks logs). <a "
        "href='man:/cron'>More information...</a></p>");

    mFileList = new FileList(this, description);

    connect(mFileList, &FileList::fileListChanged, this, &LogModeConfigurationWidget::configurationChanged);

    layout->addWidget(mFileList);

    mProcessFilterGroup = new QGroupBox(i18n("Enable Process Filtering"));
    mProcessFilterGroup->setCheckable(true);

    connect(mProcessFilterGroup, &QGroupBox::clicked, this, &CronConfigurationWidget::toggleProcessFilterEnabling);
    connect(mProcessFilterGroup, &QGroupBox::clicked, this, &LogModeConfigurationWidget::configurationChanged);

    layout->addWidget(mProcessFilterGroup);

    auto *processFilterLayout = new QHBoxLayout();

    mProcessFilterGroup->setLayout(processFilterLayout);

    mProcessFilterLabel = new QLabel(i18n("Only keeps lines which matches this process :"));
    mProcessFilter = new QLineEdit(this);

    mProcessFilterLabel->setBuddy(mProcessFilter);
    connect(mProcessFilter, &QLineEdit::textEdited, this, &LogModeConfigurationWidget::configurationChanged);

    processFilterLayout->addWidget(mProcessFilterLabel);
    processFilterLayout->addWidget(mProcessFilter);
}

bool CronConfigurationWidget::isValid() const
{
    if (mFileList->isEmpty()) {
        logDebug() << "Cron configuration not valid";
        return false;
    }

    if (mProcessFilterGroup->isChecked() && mProcessFilter->text().isEmpty()) {
        logDebug() << "Cron configuration not valid";
        return false;
    }

    logDebug() << "Cron configuration valid";
    return true;
}

void CronConfigurationWidget::saveConfig()
{
    logDebug() << "Saving config from Cron Options...";

    auto *cronConfiguration = Globals::instance().findLogMode(QStringLiteral(CRON_LOG_MODE_ID))->logModeConfiguration<CronConfiguration *>();
    cronConfiguration->setCronPaths(mFileList->paths());

    if (!mProcessFilterGroup->isChecked()) {
        cronConfiguration->setProcessFilter(QLatin1String(""));
    } else {
        cronConfiguration->setProcessFilter(mProcessFilter->text());
    }
}

void CronConfigurationWidget::readConfig()
{
    auto *cronConfiguration = Globals::instance().findLogMode(QStringLiteral(CRON_LOG_MODE_ID))->logModeConfiguration<CronConfiguration *>();

    mFileList->removeAllItems();

    mFileList->addPaths(cronConfiguration->cronPaths());

    if (cronConfiguration->processFilter().isEmpty()) {
        mProcessFilterGroup->setChecked(false);
    } else {
        mProcessFilterGroup->setChecked(true);
        mProcessFilter->setText(cronConfiguration->processFilter());
    }
}

void CronConfigurationWidget::defaultConfig()
{
    // TODO Find a way to read the configuration per default
    readConfig();
}

void CronConfigurationWidget::toggleProcessFilterEnabling(bool enabled)
{
    mProcessFilter->setEnabled(enabled);
    mProcessFilterLabel->setEnabled(enabled);
}
