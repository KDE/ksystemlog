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

#include <QColor>
#include <QIcon>
#include <QObject>
#include <QString>
class LogLevel : QObject
{
public:
    explicit LogLevel(int id, const QString &name, const QString &icon, const QColor &color, QObject *parent = nullptr);

    ~LogLevel() override;

    int id() const;
    QString name() const;

    QColor color() const;

    QIcon icon() const;

private:
    int mId;
    QString mName;

    QColor mColor;

    QIcon mIcon;
};

