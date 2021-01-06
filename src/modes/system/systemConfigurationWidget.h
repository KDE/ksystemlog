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

#ifndef SYSTEM_CONFIGURATION_WIDGET_H
#define SYSTEM_CONFIGURATION_WIDGET_H

#include "logModeConfigurationWidget.h"

#include <QVBoxLayout>

#include <KLocalizedString>

#include "globals.h"
#include "logging.h"

#include "logLevelFileList.h"

#include "logLevel.h"

#include "systemConfiguration.h"

#include "systemLogMode.h"

class SystemConfigurationWidget : public LogModeConfigurationWidget
{
    Q_OBJECT

public:
    SystemConfigurationWidget();

    ~SystemConfigurationWidget() override {}

    bool isValid() const override;

    void saveConfig() override;

    void readConfig() override;

    void defaultConfig() override;

private:
    LogLevelFileList *mFileList = nullptr;
};

#endif // _SYSTEM_CONFIGURATION_WIDGET_H
