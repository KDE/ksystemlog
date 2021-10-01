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

#include "simpleAction.h"

#include <QAction>

#include "logging.h"

SimpleAction::SimpleAction(const QIcon &icon, const QString &text, QObject *parent)
{
    mAction = new QAction(icon, text, parent);
}

SimpleAction::SimpleAction(QAction *originAction, QObject *parent)
{
    mAction = new QAction(parent);
    mAction->setIcon(originAction->icon());
    mAction->setText(originAction->text());
    mAction->setData(originAction->data());
    mAction->setToolTip(originAction->toolTip());
    mAction->setWhatsThis(originAction->whatsThis());
}

SimpleAction::~SimpleAction()
{
    delete mAction;
}

QList<QAction *> SimpleAction::innerActions()
{
    const QList<QAction *> actions{mAction};

    return actions;
}

QAction *SimpleAction::actionMenu()
{
    return mAction;
}
