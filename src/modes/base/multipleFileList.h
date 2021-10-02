/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QButtonGroup>
#include <QWidget>

#include <KMessageWidget>

#include "fileListHelper.h"

#include "ui_multipleFileListBase.h"

class MultipleFileList : public QWidget, public Ui::MultipleFileListBase
{
    Q_OBJECT

public:
    MultipleFileList(QWidget *parent, const QString &descriptionText);
    ~MultipleFileList() override;

    bool isOneOfCategoryEmpty() const;

    QStringList paths(int categoryIndex);

    void addPaths(int categoryIndex, const QStringList &paths);

    int addCategory(const QString &itemName, const QString &buttonName);

public Q_SLOTS:
    void removeAllItems();

Q_SIGNALS:
    void fileListChanged();

private Q_SLOTS:

    void updateButtons();

    void removeSelectedItem();
    void moveUpItem();
    void moveDownItem();

protected Q_SLOTS:
    virtual void addItem(int category);

    void modifyItem();
    void modifyItem(QTreeWidgetItem *item);

protected:
    void addItemInternal(QTreeWidgetItem *categoryItem, const QString &path);
    void moveItem(int direction);
    void unselectAllItems();

    void updateEmptyItems();
    void removeEmptyItems();

    void addEmptyItem(QTreeWidgetItem *item);
    bool isEmptyItem(QTreeWidgetItem *item) const;

    bool isFileListsEmpty() const;
    int categoryCount(int index) const;

    QTreeWidgetItem *findCategoryOfChild(QTreeWidgetItem *childItem);

    FileListHelper mFileListHelper;

    QButtonGroup mAddButtons;

private:
    KMessageWidget *mWarningBox = nullptr;
    bool mMissingFiles = false;
};

