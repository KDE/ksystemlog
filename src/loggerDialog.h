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

#include "ui_loggerDialogBase.h"

class LoggerDialog : public QDialog, public Ui::LoggerDialogBase
{
    Q_OBJECT
public:
    explicit LoggerDialog(QWidget *parent = nullptr);

    ~LoggerDialog() override;

    void initialize();

protected Q_SLOTS:
    void sendMessage();

    void textChanged();

    void changeTagActivation(bool activation);
    void changeFileActivation(bool activation);
    void changeMessageActivation(bool activation);

private:
    void slotLinkClicked(const QString &link);
    void buildMaps();
    QMap<QString, QString> mFacilities;

    QMap<QString, QString> mPriorities;
    QMap<QString, QIcon> mPriorityIcons;
};

