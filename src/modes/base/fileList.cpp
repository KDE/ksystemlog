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
#include <QDesktopServices>

#include <QIcon>
#include <QFileInfo>

#include "defaults.h"

#include "logging.h"

FileList::FileList(QWidget *parent, const QString &descriptionText)
    : QWidget(parent)
    , mFileListHelper(this)
{
    logDebug() << "Initializing file list...";

    setupUi(this);

    mWarningBox = new KMessageWidget(this);
    mWarningBox->setVisible(false);
    mWarningBox->setMessageType(KMessageWidget::Warning);
    mWarningBox->setText(i18n("Some log files do not exist.\n"
                              "If all log files are missing, this mode will be unavailable."));
    mWarningBox->setCloseButtonVisible(false);
    mWarningBox->setIcon(QIcon::fromTheme(QStringLiteral("dialog-warning")));
    vboxLayout->insertWidget(1, mWarningBox);

    description->setText(descriptionText);
    connect(description, &QLabel::linkActivated, this, &FileList::slotLinkClicked);

    mFileListHelper.prepareButton(add, QIcon::fromTheme(QStringLiteral("document-new")), this, SLOT(addItem()),
                                  fileList);

    mFileListHelper.prepareButton(modify, QIcon::fromTheme(QStringLiteral("document-open")), this,
                                  SLOT(modifyItem()), fileList);

    // Add a separator in the FileList
    auto *separator = new QAction(this);
    separator->setSeparator(true);
    fileList->addAction(separator);

    mFileListHelper.prepareButton(remove, QIcon::fromTheme(QStringLiteral("list-remove")), this,
                                  SLOT(removeSelectedItem()), fileList);

    mFileListHelper.prepareButton(up, QIcon::fromTheme(QStringLiteral("go-up")), this, SLOT(moveUpItem()),
                                  fileList);

    mFileListHelper.prepareButton(down, QIcon::fromTheme(QStringLiteral("go-down")), this, SLOT(moveDownItem()),
                                  fileList);

    mFileListHelper.prepareButton(removeAll, QIcon::fromTheme(QStringLiteral("trash-empty")), this,
                                  SLOT(removeAllItems()), fileList);

    connect(fileList, &QListWidget::itemSelectionChanged, this, &FileList::updateButtons);
    connect(fileList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this,
            SLOT(modifyItem(QListWidgetItem*)));
    connect(this, &FileList::fileListChanged, this, &FileList::updateButtons);

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
    return fileList->count() == 0;
}

void FileList::slotLinkClicked(const QString &link)
{
    QDesktopServices::openUrl(QUrl::fromUserInput(link));
}

void FileList::addItem()
{
    // Open a standard Filedialog
    const QList<QUrl> urls = mFileListHelper.openUrls();

    const QStringList paths = mFileListHelper.findPaths(urls);
    for (const QString &path : paths) {
        fileList->addItem(path);
    }

    Q_EMIT fileListChanged();
}

void FileList::modifyItem()
{
    modifyItem(fileList->item(fileList->currentRow()));
}

void FileList::modifyItem(QListWidgetItem *item)
{
    const QString previousPath = item->text();

    // Open a standard Filedialog
    const QUrl url = mFileListHelper.openUrl(previousPath);

    QList<QUrl> urls;
    urls.append(url);
    const QStringList paths = mFileListHelper.findPaths(urls);

    // We only take the first path
    if (paths.count() >= 1) {
        item->setText(paths.at(0));
    }

    Q_EMIT fileListChanged();
}

void FileList::removeSelectedItem()
{
    const QList<QListWidgetItem *> selectedItems = fileList->selectedItems();

    for (QListWidgetItem *item : selectedItems) {
        delete fileList->takeItem(fileList->row(item));
    }

    // fileList->setCurrentRow(fileList->count()-1);

    Q_EMIT fileListChanged();
}

void FileList::unselectAllItems()
{
    const QList<QListWidgetItem *> selectedItems = fileList->selectedItems();
    for (QListWidgetItem *item : selectedItems) {
        item->setSelected(false);
    }
}

void FileList::moveItem(int direction)
{
    const QList<QListWidgetItem *> selectedItems = fileList->selectedItems();

    QListWidgetItem *item = selectedItems.at(0);

    const int itemIndex = fileList->row(item);

    fileList->takeItem(itemIndex);

    unselectAllItems();

    fileList->insertItem(itemIndex + direction, item);

    fileList->setCurrentRow(fileList->row(item));

    Q_EMIT fileListChanged();
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

    Q_EMIT fileListChanged();
}

void FileList::updateButtons()
{
    if (fileList->count() == 0) {
        mFileListHelper.setEnabledAction(removeAll, false);
    } else {
        mFileListHelper.setEnabledAction(removeAll, true);
    }

    const QList<QListWidgetItem *> selectedItems = fileList->selectedItems();
    if (!selectedItems.isEmpty()) {
        mFileListHelper.setEnabledAction(remove, true);
        mFileListHelper.setEnabledAction(modify, true);

        QListWidgetItem *selection = selectedItems.at(0);

        // If the item is at the top of the list, it could not be upped anymore
        if (fileList->row(selection) == 0) {
            mFileListHelper.setEnabledAction(up, false);
        } else {
            mFileListHelper.setEnabledAction(up, true);
        }

        // If the item is at bottom of the list, it could not be downed anymore
        if (fileList->row(selection) == fileList->count() - 1) {
            mFileListHelper.setEnabledAction(down, false);
        } else {
            mFileListHelper.setEnabledAction(down, true);
        }
    }
    // If nothing is selected, disabled special buttons
    else {
        mFileListHelper.setEnabledAction(remove, false);
        mFileListHelper.setEnabledAction(modify, false);
        mFileListHelper.setEnabledAction(up, false);
        mFileListHelper.setEnabledAction(down, false);
    }
}

QVBoxLayout *FileList::buttonsLayout()
{
    return vboxLayout1;
}

void FileList::addPaths(const QStringList &paths)
{
    bool missingFiles = false;
    for (const QString &path : paths) {
        auto *item = new QListWidgetItem(path);
        const QFileInfo checkFile(path);
        if (!checkFile.exists()) {
            item->setForeground(Qt::red);
            missingFiles = true;
        }
        fileList->addItem(item);
    }
    mWarningBox->setVisible(missingFiles);

    updateButtons();
}

QStringList FileList::paths() const
{
    QStringList paths;
    paths.reserve(fileList->count());
    for (int i = 0; i < fileList->count(); i++) {
        paths.append(fileList->item(i)->text());
    }

    return paths;
}
