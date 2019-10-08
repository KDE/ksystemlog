/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   Copyright (C) 2007 by Nicolas Ternisien                               *
 *   nicolas.ternisien@gmail.com                                           *
 *   Copyright (C) 2015 by Vyacheslav Matyushin                            *
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

#ifndef JOURNALD_CONFIGURATION_WIDGET_H
#define JOURNALD_CONFIGURATION_WIDGET_H

#include "logModeConfigurationWidget.h"
#include "ui_journaldConfigurationWidget.h"

class JournaldConfigurationWidget : public LogModeConfigurationWidget, public Ui::JournaldConfigurationWidget
{
    Q_OBJECT

public:
    JournaldConfigurationWidget();

Q_SIGNALS:
    void configSaved();

public Q_SLOTS:
    void saveConfig() override;
    void readConfig() override;
    void defaultConfig() override;

private Q_SLOTS:
    void updateButtons();
    void addRemoteJournal();
    void modifyRemoteJournal();
    void removeRemoteJournal();
    void tableItemClicked(int row);

private:
    bool haveJournalAddress(const QString &address, const QString &port, bool httpsEnabled) const;
};

#endif // _JOURNALD_CONFIGURATION_WIDGET_H
