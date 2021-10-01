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

#include <QIcon>
#include <QUrl>
#include <QWidget>

class QAction;
class QPushButton;

/**
 * Class which maintains common behavior between
 * FileList classes
 */
class FileListHelper : public QObject
{
    Q_OBJECT

public:
    explicit FileListHelper(QWidget *parent);
    ~FileListHelper() override;

    QStringList findPaths(const QList<QUrl> &urls);
    bool isValidFile(const QUrl &url);
    QStringList expandJoker(const QUrl &url);

    QList<QUrl> openUrls() const;
    QUrl openUrl(const QString &originPath) const;

    /**
     * Change the enabled status of the button and of its QActions
     */
    void setEnabledAction(QPushButton *button, bool enabled);

    /**
     * TODO Do this inline (and remove this method)
     */
    void prepareButton(QPushButton *button, const QIcon &icon, const QObject *receiver, const char *member, QWidget *fileList);

    QAction *prepareButtonAndAction(QPushButton *button, const QIcon &icon);
    QAction *prepareButtonAndAction(QPushButton *button, const QIcon &icon, const QObject *receiver, const char *member);

private:
    QWidget *const mParent = nullptr;
};

