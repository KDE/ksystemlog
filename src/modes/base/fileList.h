/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
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

