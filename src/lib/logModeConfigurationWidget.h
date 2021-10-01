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

#include <QString>
#include <QWidget>

class LogModeConfigurationWidget : public QWidget
{
    Q_OBJECT

public:
    LogModeConfigurationWidget(const QString &itemName, const QString &iconName, const QString &header);

    ~LogModeConfigurationWidget() override;

    virtual bool isValid() const;

    virtual void saveConfig() = 0;
    virtual void defaultConfig() = 0;
    virtual void readConfig() = 0;

Q_SIGNALS:
    void configurationChanged();

public:
    QString itemName() const;
    QString iconName() const;
    QString header() const;

private:
    QString mItemName;
    QString mIconName;
    QString mHeader;
};

