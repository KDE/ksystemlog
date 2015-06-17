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

#ifndef _JOURNALD_CONFIGURATION_WIDGET_H_
#define _JOURNALD_CONFIGURATION_WIDGET_H_

#include <QGroupBox>
#include <QCheckBox>

#include <KLocalizedString>
//#include <kurlrequester.h>

#include "globals.h"
#include "logging.h"
#include "fileList.h"

#include "logLevel.h"

#include "journaldConfiguration.h"
#include "journaldLogMode.h"

#include "logModeConfigurationWidget.h"

#include "ui_journaldConfigurationWidget.h"

class JournaldConfigurationWidget : public LogModeConfigurationWidget, public Ui::JournaldConfigurationWidget
{
    Q_OBJECT

public:
    JournaldConfigurationWidget()
        : LogModeConfigurationWidget(i18n("Journald Log"), QLatin1String(JOURNALD_MODE_ICON),
                                     i18n("Journald Log"))
    {
        setupUi(this);
    }

    ~JournaldConfigurationWidget() {}

public slots:

    void saveConfig()
    {
//        JournaldConfiguration *configuration = Globals::instance()
//                                                   ->findLogMode(QLatin1String(JOURNALD_LOG_MODE_ID))
//                                                   ->logModeConfiguration<JournaldConfiguration *>();
    }

    void readConfig()
    {
//        JournaldConfiguration *configuration = Globals::instance()
//                                                   ->findLogMode(QLatin1String(JOURNALD_LOG_MODE_ID))
//                                                   ->logModeConfiguration<JournaldConfiguration *>();
    }

    void defaultConfig()
    {
        // TODO Find a way to read the configuration per default
        readConfig();
    }

protected:
    bool isValid() const
    {
        return true;
    }
};

#endif // _JOURNALD_CONFIGURATION_WIDGET_H_
