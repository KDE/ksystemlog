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

#include "ui_journaldAddressDialog.h"

#include <QDialog>

class JournaldAddressDialog : public QDialog, public Ui::JournaldAddressDialog
{
    Q_OBJECT

public:
    explicit JournaldAddressDialog(QWidget *parent,
                                   const QString &title,
                                   const QString &address = QString(),
                                   const QString &port = QString(),
                                   bool httpsEnabled = false);

    Q_REQUIRED_RESULT QString address() const;
    Q_REQUIRED_RESULT QString port() const;
    Q_REQUIRED_RESULT bool httpsEnabled() const;

public Q_SLOTS:
    void accept() override;
};

