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

#ifndef DAEMON_CONFIGURATION_WIDGET_H
#define DAEMON_CONFIGURATION_WIDGET_H

#include <KLocalizedString>

#include "fileList.h"
#include "globals.h"
#include "logging.h"

#include "logLevel.h"

#include "daemonConfiguration.h"
#include "daemonLogMode.h"

#include "logModeConfigurationWidget.h"

class FileList;

class DaemonConfigurationWidget : public LogModeConfigurationWidget
{
    Q_OBJECT

public:
    DaemonConfigurationWidget();

    ~DaemonConfigurationWidget() override
    {
    }

public Q_SLOTS:

    void saveConfig() override;

    void readConfig() override;

    void defaultConfig() override;

protected:
    bool isValid() const override;

private:
    FileList *mFileList = nullptr;
};

#endif // _DAEMON_CONFIGURATION_WIDGET_H
