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

#ifndef FILE_LIST_HELPER_H
#define FILE_LIST_HELPER_H

#include <QUrl>
#include <QWidget>
#include <QIcon>

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
    virtual ~FileListHelper();

    QStringList findPaths(QList<QUrl> urls);
    bool isValidFile(const QUrl &url);
    QStringList expandJoker(const QUrl &url);

    QList<QUrl> openUrls();
    QUrl openUrl(const QString &originPath);

    /**
     * Change the enabled status of the button and of its QActions
     */
    void setEnabledAction(QPushButton *button, bool enabled);

    /**
     * TODO Do this inline (and remove this method)
     */
    void prepareButton(QPushButton *button, const QIcon &icon, const QObject *receiver, const char *member,
                       QWidget *fileList);

    QAction *prepareButtonAndAction(QPushButton *button, const QIcon &icon);
    QAction *prepareButtonAndAction(QPushButton *button, const QIcon &icon, const QObject *receiver,
                                    const char *member);

private:
    QWidget *parent;
};

#endif //_FILE_LIST_HELPER_H
