/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   Copyright (C) 2007 by Nicolas Ternisien                               *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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

#ifndef STATUS_BAR_
#define STATUS_BAR_

#include <QStatusBar>

class QString;
class QLabel;
class QComboBox;
class QPushButton;
namespace KSystemLog {
/**
 * Status Bar
 */
class StatusBar : public QStatusBar
{
    Q_OBJECT

public:
    explicit StatusBar(QWidget *parent);

    ~StatusBar() override;

    void changeLineCountMessage(const QString &lineCountMessage);
    void changeLastModification(const QTime &lastModification);

    void changeMessage(const QString &message);

private Q_SLOTS:
    void toggleHistory();
    void selectLastHistory();

private:
    QLabel *mLineCountLabel = nullptr;
    QComboBox *mMessageList = nullptr;
    QLabel *mLastModificationLabel = nullptr;
    QPushButton *mToggleHistory = nullptr;
};
}

#endif // _STATUS_BAR_
