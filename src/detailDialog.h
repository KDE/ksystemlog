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

#include <QDialog>

class LogViewWidget;

#include "ui_detailDialogBase.h"

class DetailDialog : public QDialog, public Ui::DetailDialogBase
{
    Q_OBJECT

public:
    explicit DetailDialog(QWidget *parent);

    ~DetailDialog() override;

public Q_SLOTS:
    void selectionChanged(LogViewWidget *logViewWidget);

private Q_SLOTS:
    void previousItem();
    void nextItem();

private:
    void updateDetails();

    /**
     * Method that replaces the bugged itemAbove() and itemBelow() methods
     */
    void moveToItem(int direction);

    LogViewWidget *mLogViewWidget = nullptr;
};

