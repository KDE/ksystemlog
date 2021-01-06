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

#ifndef X_SESSION_CONFIGURATION_WIDGET_H
#define X_SESSION_CONFIGURATION_WIDGET_H

#include <QCheckBox>

#include <KLocalizedString>
#include <KUrlRequester>

#include "globals.h"
#include "logging.h"
#include "fileList.h"

#include "logLevel.h"

#include "xsessionConfiguration.h"
#include "xsessionLogMode.h"

#include "logModeConfigurationWidget.h"

#include "ui_xsessionConfigurationWidgetBase.h"

class XSessionConfigurationWidget : public LogModeConfigurationWidget,
                                    public Ui::XSessionConfigurationWidgetBase
{
    Q_OBJECT

public:
    XSessionConfigurationWidget();

    ~XSessionConfigurationWidget() override {}

public Q_SLOTS:

    void saveConfig() override;

    void readConfig() override;

    void defaultConfig() override;

protected:
    bool isValid() const override;

private:
    void prepareXorgErrorsDescription();

    bool mXorgErrorsDescriptionDefined;

    KMessageWidget *mWarningBox = nullptr;
};

#endif // _X_SESSION_CONFIGURATION_WIDGET_H
