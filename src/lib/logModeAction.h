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

#include <QAction>
#include <QList>
#include <QObject>

class LogModeAction : public QObject
{
    Q_OBJECT

public:
    enum Category { RootCategory, ServicesCategory, OthersCategory };

    Q_DECLARE_FLAGS(Categories, Category)

    explicit LogModeAction(QObject *parent = nullptr);

    ~LogModeAction() override;

    virtual QList<QAction *> innerActions() = 0;

    virtual QAction *actionMenu() = 0;

    void setCategory(Category category);

    Category category() const;

private:
    LogModeAction::Category mCategory = LogModeAction::RootCategory;
    bool mInToolBar = true;
};

