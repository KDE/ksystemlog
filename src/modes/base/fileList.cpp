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

#include "fileList.h"

#include <QListWidget>

#include <QPushButton>

#include <kactioncollection.h>
#include <kmessagebox.h>
#include <QIcon>
#include <QMenu>
#include <QFileInfo>

#include "defaults.h"

#include "logging.h"

FileList::FileList(QWidget *parent, const QString &descriptionText)
    : QWidget(parent)
    , fileListHelper(this)
{
    logDebug() << "Initializing file list...";

    setupUi(this);

    warningBox = new KMessageWidget(this);
    warningBox->setVisible(false);
    warningBox->setMessageType(KMessageWidget::Warning);
    warningBox->setText(i18n("Some log files do not exist.\n"
                             "If all log files are missing, this mode will be unavailable."));
    warningBox->setCloseButtonVisible(false);
    warningBox->setIcon(QIcon::fromTheme(QLatin1String("dialog-warning")));
    vboxLayout->insertWidget(1, warningBox);

    description->setText(descriptionText);

    fileListHelper.prepareButton(add, QIcon::fromTheme(QLatin1String("document-new")), this, SLOT(addItem()),
                                 fileList);

    fileListHelper.prepareButton(modify, QIcon::fromTheme(QLatin1String("document-open")), this,
                                 SLOT(modifyItem()), fileList);

    // Add a separator in the FileList
    QAction *separator = new QAction(this);
    separator->setSeparator(true);
    fileList->addAction(separator);

    fileListHelper.prepareButton(remove, QIcon::fromTheme(QLatin1String("list-remove")), this,
                                 SLOT(removeSelectedItem()), fileList);

    fileListHelper.prepareButton(up, QIcon::fromTheme(QLatin1String("go-up")), this, SLOT(moveUpItem()),
                                 fileList);

    fileListHelper.prepareButton(down, QIcon::fromTheme(QLatin1String("go-down")), this, SLOT(moveDownItem()),
                                 fileList);

    fileListHelper.prepareButton(removeAll, QIcon::fromTheme(QLatin1String("trash-empty")), this,
                                 SLOT(removeAllItems()), fileList);

    connect(fileList, SIGNAL(itemSelectionChanged()), this, SLOT(updateButtons()));
    connect(fileList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this,
            SLOT(modifyItem(QListWidgetItem *)));
    connect(this, SIGNAL(fileListChanged()), this, SLOT(updateButtons()));

    updateButtons();

    logDebug() << "File list initialized";
}

FileList::~FileList()
{
}

int FileList::count() const
{
    return fileList->count();
}

bool FileList::isEmpty() const
{
    return (fileList->count() == 0);
}

void FileList::addItem()
{
    // Open a standard Filedialog
    QList<QUrl> urls = fileListHelper.openUrls();

    QStringList paths = fileListHelper.findPaths(urls);
    foreach (const QString &path, paths) {
        fileList->addItem(path);
    }

    emit fileListChanged();
}

void FileList::modifyItem()
{
    modifyItem(fileList->item(fileList->currentRow()));
}

void FileList::modifyItem(QListWidgetItem *item)
{
    QString previousPath = item->text();

    // Open a standard Filedialog
    QUrl url = fileListHelper.openUrl(previousPath);

    QList<QUrl> urls;
    urls.append(url);
    QStringList paths = fileListHelper.findPaths(urls);

    // We only take the first path
    if (paths.count() >= 1) {
        item->setText(paths.at(0));
    }

    emit fileListChanged();
}

void FileList::removeSelectedItem()
{
    QList<QListWidgetItem *> selectedItems = fileList->selectedItems();

    foreach (QListWidgetItem *item, selectedItems) {
        delete fileList->takeItem(fileList->row(item));
    }

    // fileList->setCurrentRow(fileList->count()-1);

    emit fileListChanged();
}

void FileList::unselectAllItems()
{
    QList<QListWidgetItem *> selectedItems = fileList->selectedItems();
    foreach (QListWidgetItem *item, selectedItems) {
        item->setSelected(false);
    }
}

void FileList::moveItem(int direction)
{
    QList<QListWidgetItem *> selectedItems = fileList->selectedItems();

    QListWidgetItem *item = selectedItems.at(0);

    int itemIndex = fileList->row(item);

    fileList->takeItem(itemIndex);

    unselectAllItems();

    fileList->insertItem(itemIndex + direction, item);

    fileList->setCurrentRow(fileList->row(item));

    emit fileListChanged();
}

void FileList::moveUpItem()
{
    moveItem(-1);
}

void FileList::moveDownItem()
{
    moveItem(+1);
}

void FileList::removeAllItems()
{
    fileList->clear();

    emit fileListChanged();
}

void FileList::updateButtons()
{
    if (fileList->count() == 0)
        fileListHelper.setEnabledAction(removeAll, false);
    else
        fileListHelper.setEnabledAction(removeAll, true);

    QList<QListWidgetItem *> selectedItems = fileList->selectedItems();
    if (selectedItems.isEmpty() == false) {
        fileListHelper.setEnabledAction(remove, true);
        fileListHelper.setEnabledAction(modify, true);

        QListWidgetItem *selection = selectedItems.at(0);

        // If the item is at the top of the list, it could not be upped anymore
        if (fileList->row(selection) == 0)
            fileListHelper.setEnabledAction(up, false);
        else
            fileListHelper.setEnabledAction(up, true);

        // If the item is at bottom of the list, it could not be downed anymore
        if (fileList->row(selection) == fileList->count() - 1)
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
}

QVBoxLayout *FileList::buttonsLayout()
{
    return vboxLayout1;
}

void FileList::addPaths(const QStringList &paths)
{
    bool missingFiles = false;
    foreach (const QString &path, paths) {
        QListWidgetItem *item = new QListWidgetItem(path);
        QFileInfo checkFile(path);
        if (!checkFile.exists()) {
            item->setForeground(Qt::red);
            missingFiles = true;
        }
        fileList->addItem(item);
    }
    warningBox->setVisible(missingFiles);

    updateButtons();
}

QStringList FileList::paths()
{
    QStringList paths;
    for (int i = 0; i < fileList->count(); i++) {
        paths.append(fileList->item(i)->text());
    }

    return paths;
}
