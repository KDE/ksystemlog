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

#include <QList>
#include <QTreeWidget>

#include "logViewColumns.h"

class LogViewWidgetItem;
class LogViewModel;
class LogLine;
class QActionGroup;
class LogViewWidget : public QTreeWidget
{
    Q_OBJECT

public:
    explicit LogViewWidget(QWidget *parent = nullptr);

    ~LogViewWidget() override;
    void selectAll() override;
    void setColumns(const LogViewColumns &columns);

    int itemCount() const;
    int notHiddenItemCount();

    QList<LogViewWidgetItem *> items();
    QList<LogLine *> logLines();

    /**
     * Return the related widget item of this logLine or NULL if it has not been found
     */
    LogViewWidgetItem *findItem(LogLine *logLine);

    LogViewWidgetItem *findNewestItem();

    LogViewModel *model() const;

    bool hasItemsSelected();
    LogViewWidgetItem *firstSelectedItem();
    LogViewWidgetItem *lastSelectedItem();

    void scrollToNewestItem();

    void expandAll();
    void collapseAll();

    void resizeColumns();

public Q_SLOTS:
    void toggleToolTip(bool enabled);

Q_SIGNALS:
    void columnsChanged(const LogViewColumns &columns);

private Q_SLOTS:
    void toggleHeader(QAction *action);

private:
    LogViewModel *mLogViewModel = nullptr;

    QActionGroup *mHeadersTogglingActions = nullptr;
};

