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

#ifndef CRON_CONFIGURATION_WIDGET_H
#define CRON_CONFIGURATION_WIDGET_H

#include "logModeConfigurationWidget.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>

#include <KLocalizedString>

#include "globals.h"
#include "logging.h"

#include "fileList.h"

#include "logLevel.h"

#include "cronConfiguration.h"
#include "cronLogMode.h"

class CronConfigurationWidget : public LogModeConfigurationWidget
{
    Q_OBJECT

public:
    CronConfigurationWidget();

    ~CronConfigurationWidget() override
    {
    }

    bool isValid() const override;

    void saveConfig() override;

    void readConfig() override;

    void defaultConfig() override;

private Q_SLOTS:
    void toggleProcessFilterEnabling(bool enabled);

private:
    FileList *mFileList = nullptr;

    QGroupBox *mProcessFilterGroup = nullptr;

    QLineEdit *mProcessFilter = nullptr;
    QLabel *mProcessFilterLabel = nullptr;
};

#endif // _CRON_CONFIGURATION_WIDGET_H
