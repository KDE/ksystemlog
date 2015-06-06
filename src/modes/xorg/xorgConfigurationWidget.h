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

#ifndef _XORG_CONFIGURATION_WIDGET_H_
#define _XORG_CONFIGURATION_WIDGET_H_

#include <KLocalizedString>

#include "globals.h"
#include "logging.h"
#include "fileList.h"

#include "logLevel.h"
#include "logModeConfigurationWidget.h"

#include "xorgConfiguration.h"
#include "xorgLogMode.h"

class FileList;

class XorgConfigurationWidget : public LogModeConfigurationWidget
{
    Q_OBJECT

public:
    XorgConfigurationWidget()
        : LogModeConfigurationWidget(i18n("X.org Log"), QLatin1String(XORG_MODE_ICON), i18n("X.org Log"))
    {
        QHBoxLayout *layout = new QHBoxLayout();
        this->setLayout(layout);

        fileList
            = new FileList(this, i18n("<p>These files will be analyzed to show the <b>X.org log</b>.</p>"));
        connect(fileList, SIGNAL(fileListChanged()), this, SIGNAL(configurationChanged()));
        layout->addWidget(fileList);
    }

    ~XorgConfigurationWidget() {}

public slots:

    void saveConfig()
    {
        XorgConfiguration *xorgConfiguration = Globals::instance()
                                                   ->findLogMode(QLatin1String(XORG_LOG_MODE_ID))
                                                   ->logModeConfiguration<XorgConfiguration *>();

        xorgConfiguration->setXorgPaths(fileList->paths());
    }

    void readConfig()
    {
        XorgConfiguration *xorgConfiguration = Globals::instance()
                                                   ->findLogMode(QLatin1String(XORG_LOG_MODE_ID))
                                                   ->logModeConfiguration<XorgConfiguration *>();

        fileList->removeAllItems();

        fileList->addPaths(xorgConfiguration->xorgPaths());
    }

    void defaultConfig()
    {
        // TODO Find a way to read the configuration per default
        readConfig();
    }

protected:
    bool isValid() const
    {
        if (fileList->isEmpty() == false) {
            return true;
        }

        return false;
    }

private:
    FileList *fileList;
};

#endif // _XORG_CONFIGURATION_WIDGET_H_
