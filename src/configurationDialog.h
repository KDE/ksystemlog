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

#ifndef _CONFIGURATION_DIALOG_H_
#define _CONFIGURATION_DIALOG_H_

#include <kconfigdialog.h>

class ConfigurationDialogPrivate;

class ConfigurationDialog : public KConfigDialog
{
    Q_OBJECT

public:
    ConfigurationDialog(QWidget *parent);

    ~ConfigurationDialog();

    void showConfiguration();

signals:
    void configurationSaved();

protected slots:

    void updateWidgets() Q_DECL_OVERRIDE;
    void updateWidgetsDefault() Q_DECL_OVERRIDE;
    void updateSettings() Q_DECL_OVERRIDE;
    void updateButtons();

private slots:
    void updateConfiguration();

private:
    bool hasChanged() Q_DECL_OVERRIDE;
    bool isDefault() Q_DECL_OVERRIDE;

    ConfigurationDialogPrivate *const d;

    void setupGeneralConfiguration();

    void setupLogModeConfigurations();
};

#endif //_CONFIGURATION_DIALOG_H_
