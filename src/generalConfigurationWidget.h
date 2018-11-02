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

#ifndef _GENERAL_CONFIGURATION_WIDGET_H_
#define _GENERAL_CONFIGURATION_WIDGET_H_

#include <QWidget>

#include "ui_generalConfigurationWidgetBase.h"

class GeneralConfigurationWidgetPrivate;

class GeneralConfigurationWidget : public QWidget, public Ui::GeneralConfigurationWidgetBase
{
    Q_OBJECT

public:
    GeneralConfigurationWidget();

    ~GeneralConfigurationWidget();

    bool isValid() const;

    enum dateFormat {};

public Q_SLOTS:
    void saveConfig() const;

    void defaultConfig();

    void readConfig();

Q_SIGNALS:
    void configurationChanged();

private:
    void addDateFormatExample();

    GeneralConfigurationWidgetPrivate *const d;
};

#endif // _GENERAL_CONFIGURATION_WIDGET_H_
