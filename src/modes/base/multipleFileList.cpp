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

#include "multipleFileList.h"

#include <QAction>
#include <QPushButton>
#include <QHeaderView>

#include <KLocalizedString>
#include <KActionCollection>
#include <KMessageBox>

#include <QIcon>
#include <QMenu>
#include <QFileInfo>

#include "defaults.h"

#include "logging.h"

MultipleFileList::MultipleFileList(QWidget *parent, const QString &descriptionText)
    : QWidget(parent)
    , fileListHelper(this)
{
    logDebug() << "Initializing multiple file list...";

    setupUi(this);

    missingFiles = false;
    warningBox = new KMessageWidget(this);
    warningBox->setVisible(false);
    warningBox->setMessageType(KMessageWidget::Warning);
    warningBox->setText(i18n("Some log files do not exist.\n"
                             "Modes with missing log files will be unavailable."));
    warningBox->setCloseButtonVisible(false);
    warningBox->setIcon(QIcon::fromTheme(QStringLiteral("dialog-warning")));
    vboxLayout->insertWidget(1, warningBox);

    description->setText(descriptionText);

    fileListHelper.prepareButton(modify, QIcon::fromTheme(QStringLiteral("document-open")), this,
                                 SLOT(modifyItem()), fileList);

    fileList->header()->setVisible(false);

    // Add a separator in the MultipleFileList
    QAction *separator = new QAction(this);
    separator->setSeparator(true);
    fileList->addAction(separator);

    fileListHelper.prepareButton(remove, QIcon::fromTheme(QStringLiteral("list-remove")), this,
                                 SLOT(removeSelectedItem()), fileList);

    fileListHelper.prepareButton(up, QIcon::fromTheme(QStringLiteral("go-up")), this, SLOT(moveUpItem()),
                                 fileList);

    fileListHelper.prepareButton(down, QIcon::fromTheme(QStringLiteral("go-down")), this, SLOT(moveDownItem()),
                                 fileList);

    fileListHelper.prepareButton(removeAll, QIcon::fromTheme(QStringLiteral("trash-empty")), this,
                                 SLOT(removeAllItems()), fileList);

    connect(fileList, &QTreeWidget::itemSelectionChanged, this, &MultipleFileList::updateButtons);
    connect(fileList, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this,
            SLOT(modifyItem(QTreeWidgetItem*)));
    connect(this, &MultipleFileList::fileListChanged, this, &MultipleFileList::updateButtons);

    connect(&addButtons, SIGNAL(buttonClicked(int)), this, SLOT(addItem(int)));

    updateButtons();

    logDebug() << "Multiple File list initialized";
}

MultipleFileList::~MultipleFileList()
{
}

void MultipleFileList::updateButtons()
{
    logDebug() << "Updating buttons...";

    if (isFileListsEmpty() == true)
        fileListHelper.setEnabledAction(removeAll, false);
    else
        fileListHelper.setEnabledAction(removeAll, true);

    QList<QTreeWidgetItem *> selectedItems = fileList->selectedItems();

    // If the selection is not empty and a empty item is not selected
    QTreeWidgetItem *categoryItem = nullptr;
    if (selectedItems.isEmpty() == false && isEmptyItem(selectedItems.at(0)) == false) {
        categoryItem = findCategoryOfChild(selectedItems.at(0));
    }

    if (categoryItem != nullptr) {
        int categoryIndex = fileList->indexOfTopLevelItem(categoryItem);

        fileListHelper.setEnabledAction(remove, true);
        fileListHelper.setEnabledAction(modify, true);

        QTreeWidgetItem *selectedItem = selectedItems.at(0);

        // If the item is at the top of the list, it could not be upped anymore
        if (categoryItem->indexOfChild(selectedItem) == 0)
            fileListHelper.setEnabledAction(up, false);
        else
            fileListHelper.setEnabledAction(up, true);

        // If the item is at bottom of the list, it could not be downed anymore
        if (categoryItem->indexOfChild(selectedItem) == categoryCount(categoryIndex) - 1)
            fileListHelper.setEnabledAction(down, false);
        else
            fileListHelper.setEnabledAction(down, true);

    }
    // If nothing is selected, disabled special buttons
    else {
        fileListHelper.setEnabledAction(remove, false);
        fileListHelper.setEnabledAction(modify, false);
        fileListHelper.setEnabledAction(up, false);
        fileListHelper.setEnabledAction(down, false);
    }

    logDebug() << "Buttons updated";
}

bool MultipleFileList::isFileListsEmpty() const
{
    for (int i = 0; i < fileList->topLevelItemCount(); ++i) {
        if (categoryCount(i) != 0) {
            logDebug() << "Is not empty";
            return false;
        }
    }

    logDebug() << "Is empty";
    return true;
}

bool MultipleFileList::isOneOfCategoryEmpty() const
{
    for (int i = 0; i < fileList->topLevelItemCount(); ++i) {
        if (categoryCount(i) == 0) {
            logDebug() << "A category is empty";
            return true;
        }
    }

    logDebug() << "No category is empty";
    return false;
}

int MultipleFileList::categoryCount(int index) const
{
    QTreeWidgetItem *item = fileList->topLevelItem(index);
    if (item == nullptr) {
        logCritical() << "Index out of range" << index;
        return 0;
    }

    int count = 0;
    for (int i = 0; i < item->childCount(); ++i) {
        QTreeWidgetItem *childItem = item->child(i);
        if (isEmptyItem(childItem) == false)
            count++;
    }

    return count;
}

int MultipleFileList::addCategory(const QString &itemName, const QString &buttonName)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(fileList, QStringList(itemName));
    item->setExpanded(true);
    QFont font = item->font(0);
    font.setBold(true);
    item->setFont(0, font);

    int index = fileList->indexOfTopLevelItem(item);

    QPushButton *addButton = new QPushButton(buttonName, this);
    QAction *action
        = fileListHelper.prepareButtonAndAction(addButton, QIcon::fromTheme(QStringLiteral("document-new")));

    // Insert the action in first position
    fileList->insertAction(fileList->actions().at(addButtons.buttons().size()), action);

    addButtons.addButton(addButton, index);

    vboxLayout1->insertWidget(index, addButton);

    updateEmptyItems();

    updateButtons();

    return index;
}

void MultipleFileList::addItem(int category)
{
    logDebug() << "Adding item" << category;

    // Open a standard Filedialog
    QList<QUrl> urls = fileListHelper.openUrls();

    QTreeWidgetItem *categoryItem = fileList->topLevelItem(category);

    QStringList paths = fileListHelper.findPaths(urls);
    foreach (const QString &path, paths) {
        addItemInternal(categoryItem, path);
    }

    updateEmptyItems();

    Q_EMIT fileListChanged();
}

void MultipleFileList::addItemInternal(QTreeWidgetItem *categoryItem, const QString &path)
{
    logDebug() << "Adding" << path << "to" << categoryItem->text(0);
    QTreeWidgetItem *item = new QTreeWidgetItem(QStringList(path));

    QFileInfo checkFile(path);
    if (!checkFile.exists()) {
        missingFiles = true;
        item->setForeground(0, Qt::red);
    }

    categoryItem->addChild(item);
    categoryItem->setExpanded(true);
}

QTreeWidgetItem *MultipleFileList::findCategoryOfChild(QTreeWidgetItem *childItem)
{
    logDebug() << "Finding Category of" << childItem->text(0);

    for (int i = 0; i < fileList->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = fileList->topLevelItem(i);

        if (item->indexOfChild(childItem) != -1) {
            logDebug() << "Category of" << childItem->text(0) << "is" << item->text(0);
            return item;
        }
    }

    logDebug() << "No Category of" << childItem->text(0);
    return nullptr;
}

void MultipleFileList::modifyItem()
{
    QList<QTreeWidgetItem *> selectedItems = fileList->selectedItems();
    modifyItem(selectedItems.at(0));
}

void MultipleFileList::modifyItem(QTreeWidgetItem *item)
{
    // If the user tries to modify a category item, we do nothing
    if (findCategoryOfChild(item) == nullptr || isEmptyItem(item) == true)
        return;

    QString previousPath = item->text(0);

    // Open a standard Filedialog
    QUrl url = fileListHelper.openUrl(previousPath);
    if (url.isEmpty())
        return;

    QList<QUrl> urls;
    urls.append(url);
    QStringList paths = fileListHelper.findPaths(urls);

    // We only take the first path
    if (paths.count() >= 1) {
        item->setText(0, paths.at(0));
    }

    Q_EMIT fileListChanged();
}

void MultipleFileList::removeSelectedItem()
{
    QList<QTreeWidgetItem *> selectedItems = fileList->selectedItems();

    foreach (QTreeWidgetItem *item, selectedItems) {
        QTreeWidgetItem *categoryItem = findCategoryOfChild(item);
        delete categoryItem->takeChild(categoryItem->indexOfChild(item));
    }

    updateEmptyItems();

    Q_EMIT fileListChanged();
}

void MultipleFileList::moveItem(int direction)
{
    QList<QTreeWidgetItem *> selectedItems = fileList->selectedItems();

    QTreeWidgetItem *item = selectedItems.at(0);

    QTreeWidgetItem *categoryItem = findCategoryOfChild(item);
    int itemIndex = categoryItem->indexOfChild(item);

    categoryItem->takeChild(itemIndex);

    unselectAllItems();

    categoryItem->insertChild(itemIndex + direction, item);

    fileList->setCurrentItem(item);
    // item->setSelected(true);

    Q_EMIT fileListChanged();
}

void MultipleFileList::moveUpItem()
{
    moveItem(-1);
}

void MultipleFileList::moveDownItem()
{
    moveItem(1);
}

void MultipleFileList::removeAllItems()
{
    QTreeWidgetItemIterator it(fileList, QTreeWidgetItemIterator::All);
    while (*it != nullptr) {
        QTreeWidgetItem *item = *it;

        QList<QTreeWidgetItem *> children = item->takeChildren();
        foreach (QTreeWidgetItem *childItem, children) {
            delete childItem;
        }

        ++it;
    }

    updateEmptyItems();

    Q_EMIT fileListChanged();
}

void MultipleFileList::unselectAllItems()
{
    QList<QTreeWidgetItem *> selectedItems = fileList->selectedItems();
    foreach (QTreeWidgetItem *item, selectedItems) {
        item->setSelected(false);
    }
}

void MultipleFileList::updateEmptyItems()
{
    logDebug() << "Updating empty items...";

    logDebug() << "Adding empty items...";

    for (int i = 0; i < fileList->topLevelItemCount(); ++i) {
        QTreeWidgetItem *categoryItem = fileList->topLevelItem(i);

        // If it's a category item and it's empty
        if (categoryItem->childCount() == 0) {
            addEmptyItem(categoryItem);
        }
    }

    removeEmptyItems();

    logDebug() << "Empty items updated";
}

void MultipleFileList::removeEmptyItems()
{
    logDebug() << "Removing empty items...";

    // Remove empty items of lists
    for (int categoryIndex = 0; categoryIndex < fileList->topLevelItemCount(); ++categoryIndex) {
        QTreeWidgetItem *categoryItem = fileList->topLevelItem(categoryIndex);

        logDebug() << "Removing empty items of " << categoryItem->text(0);

        for (int i = 0; i < categoryItem->childCount(); ++i) {
            QTreeWidgetItem *childItem = categoryItem->child(i);

            if (isEmptyItem(childItem) == true && categoryItem->childCount() > 1) {
                logDebug() << "Remove a child item";
                delete categoryItem->takeChild(i);
                break;
            }
        }

        logDebug() << "Empty items of " << categoryItem->text(0) << "removed";
    }
}

bool MultipleFileList::isEmptyItem(QTreeWidgetItem *item) const
{
    if (item->font(0).italic() == true)
        return true;
    else
        return false;
}

void MultipleFileList::addEmptyItem(QTreeWidgetItem *item)
{
    logDebug() << "Adding an empty item...";

    QTreeWidgetItem *emptyItem = new QTreeWidgetItem(item, QStringList(i18n("No log file...")));
    item->setExpanded(true);
    QFont font = emptyItem->font(0);
    font.setItalic(true);
    emptyItem->setFont(0, font);
}

void MultipleFileList::addPaths(int category, const QStringList &paths)
{
    missingFiles = false;
    QTreeWidgetItem *categoryItem = fileList->topLevelItem(category);

    foreach (const QString &path, paths) {
        addItemInternal(categoryItem, path);
    }

    updateEmptyItems();

    updateButtons();

    warningBox->setVisible(missingFiles);
}

QStringList MultipleFileList::paths(int category)
{
    QTreeWidgetItem *categoryItem = fileList->topLevelItem(category);
    QTreeWidgetItemIterator it(fileList, QTreeWidgetItemIterator::All);

    QStringList paths;
    while (*it != nullptr) {
        QTreeWidgetItem *item = *it;

        if (categoryItem->indexOfChild(item) != -1) {
            paths.append(item->text(0));
        }

        ++it;
    }

    return paths;
}
