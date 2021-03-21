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

#pragma once

#include <QWidget>

#include "fileListHelper.h"

#include "ui_fileListBase.h"
#include <KMessageWidget>

class QVBoxLayout;

class FileList : public QWidget, public Ui::FileListBase
{
    Q_OBJECT
public:
    explicit FileList(QWidget *parent, const QString &descriptionText);
    ~FileList() override;

    int count() const;

    bool isEmpty() const;

    QStringList paths() const;

    void addPaths(const QStringList &paths);

public Q_SLOTS:
    void removeAllItems();

Q_SIGNALS:
    void fileListChanged();

private:
    void slotLinkClicked(const QString &link);

private Q_SLOTS:

    void updateButtons();

    void removeSelectedItem();
    void moveUpItem();
    void moveDownItem();

protected Q_SLOTS:
    virtual void addItem();

    void modifyItem();
    void modifyItem(QListWidgetItem *item);

protected:
    void removeItem(int id);

    void moveItem(int direction);
    void unselectAllItems();

    /**
     * Convenient method which returns the layout which manage the button list
     */
    QVBoxLayout *buttonsLayout();

    FileListHelper mFileListHelper;

    KMessageWidget *mWarningBox = nullptr;
};

