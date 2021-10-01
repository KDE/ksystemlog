/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *   SPDX-FileCopyrightText: 2015 Vyacheslav Matyushin *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
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

