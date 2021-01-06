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

#include "acpidConfigurationWidget.h"

AcpidConfigurationWidget::AcpidConfigurationWidget()
    : LogModeConfigurationWidget(i18n("Acpid Log"), QStringLiteral(ACPID_MODE_ICON), i18n("Acpid Log"))
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    mFileList
        = new FileList(this, i18n("<p>These files will be analyzed to show the <b>Acpid log</b>.</p>"));
    connect(mFileList, &FileList::fileListChanged, this, &LogModeConfigurationWidget::configurationChanged);
    layout->addWidget(mFileList);
}

void AcpidConfigurationWidget::saveConfig()
{
    AcpidConfiguration *acpidConfiguration = Globals::instance()
                                             .findLogMode(QStringLiteral(ACPID_LOG_MODE_ID))
                                             ->logModeConfiguration<AcpidConfiguration *>();

    acpidConfiguration->setAcpidPaths(mFileList->paths());
}

void AcpidConfigurationWidget::readConfig()
{
    AcpidConfiguration *acpidConfiguration = Globals::instance()
                                             .findLogMode(QStringLiteral(ACPID_LOG_MODE_ID))
                                             ->logModeConfiguration<AcpidConfiguration *>();

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
    if (mFileList->isEmpty() == false) {
        return true;
    }

    return false;
}
