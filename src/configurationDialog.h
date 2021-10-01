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

#include <KConfigDialog>

class GeneralConfigurationWidget;
class LogModeConfigurationWidget;
class ConfigurationDialog : public KConfigDialog
{
    Q_OBJECT

public:
    explicit ConfigurationDialog(QWidget *parent);

    ~ConfigurationDialog() override;

    void showConfiguration();

Q_SIGNALS:
    void configurationSaved();

protected Q_SLOTS:

    void updateWidgets() override;
    void updateWidgetsDefault() override;
    void updateSettings() override;
    void updateButtons();

private Q_SLOTS:
    void updateConfiguration();

private:
    bool hasChanged() override;
    bool isDefault() override;

    void setupGeneralConfiguration();

    void setupLogModeConfigurations();
    GeneralConfigurationWidget *mGeneralConfiguration = nullptr;

    QList<LogModeConfigurationWidget *> mLogModeConfigurations;

    bool mChanged = false;
};

