/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *   SPDX-FileCopyrightText: 2015 Vyacheslav Matyushin *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#pragma once

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

