/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
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

#include <QWidget>

#include "ui_generalConfigurationWidgetBase.h"
class KMessageWidget;
class GeneralConfigurationWidget : public QWidget, public Ui::GeneralConfigurationWidgetBase
{
    Q_OBJECT

public:
    GeneralConfigurationWidget();

    ~GeneralConfigurationWidget() override;

    bool isValid() const;

public Q_SLOTS:
    void saveConfig() const;

    void defaultConfig();

    void readConfig();

Q_SIGNALS:
    void configurationChanged();

private:
    void addDateFormatExample();
    QButtonGroup *mDateFormatGroup = nullptr;
    KMessageWidget *mWarningBox = nullptr;
};

