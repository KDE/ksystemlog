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

#include "logLevelFileList.h"

#include <QListWidgetItem>
#include <QString>
#include <QStringList>

#include <QPushButton>

// KDE includes
#include <KLocalizedString>
#include <QFileInfo>
#include <QIcon>

#include "defaults.h"

#include "globals.h"
#include "logging.h"

#include "logLevel.h"
#include "logLevelSelectionDialog.h"

int LogLevelFileList::LogLevelRole = 33;

LogLevelFileList::LogLevelFileList(QWidget *parent, const QString &description)
    : FileList(parent, description)
{
    logDebug() << "Initializing specific file list...";

    changeItem = new QPushButton(i18n("&Change Status..."));
    changeItem->setToolTip(i18n("Change the level of the current file(s)"));
    changeItem->setWhatsThis(
        i18n("Changes the level of the current file(s). See KSystemLog documentation for more information about "
             "each log level."));

    // Insert the button just after the "Modify File" button
    buttonsLayout()->insertWidget(2, changeItem);

    QAction *action = mFileListHelper.prepareButtonAndAction(changeItem, QIcon::fromTheme(QStringLiteral("favorites")), this, SLOT(changeItemType()));

    // Insert the action just after the "Modify File" action
    fileList->insertAction(fileList->actions().at(2), action);

    changeItem->setEnabled(false);

    connect(fileList, &QListWidget::itemSelectionChanged, this, &LogLevelFileList::updateSpecificButtons);
    connect(this, &FileList::fileListChanged, this, &LogLevelFileList::updateSpecificButtons);

    disconnect(fileList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(modifyItem(QListWidgetItem *)));
    connect(fileList, &QListWidget::itemDoubleClicked, this, &LogLevelFileList::changeItemType);

    updateSpecificButtons();

    logDebug() << "Specific file list initialized";
}

LogLevelFileList::~LogLevelFileList()
{
    // changeItem is managed automatically
}

void LogLevelFileList::insertItem(LogLevel *level, const QString &itemText, bool missing)
{
    auto item = new QListWidgetItem(level->icon(), itemText, fileList);
    if (missing) {
        item->setForeground(Qt::red);
    }
    item->setData(LogLevelFileList::LogLevelRole, level->id());
}

void LogLevelFileList::addItem()
{
    // Open a standard Filedialog
    const QList<QUrl> urls = mFileListHelper.openUrls();

    const QStringList paths = mFileListHelper.findPaths(urls);
    for (const QString &path : paths) {
        insertItem(Globals::instance().informationLogLevel(), path);
    }

    Q_EMIT fileListChanged();
}

void LogLevelFileList::updateSpecificButtons()
{
    if (!fileList->selectedItems().isEmpty()) {
        changeItem->setEnabled(true);
    } else {
        changeItem->setEnabled(false);
    }
}

void LogLevelFileList::changeItemType()
{
    logDebug() << "Changing item type...";

    LogLevelSelectionDialog logLevelSelectionDialog(this);
    QListWidget *logLevels = logLevelSelectionDialog.logLevels();

    const auto logLevelsItems{Globals::instance().logLevels()};
    for (LogLevel *level : logLevelsItems) {
        logLevels->addItem(new QListWidgetItem(level->icon(), level->name()));
    }

    int choice = logLevelSelectionDialog.exec();

    if (choice == QDialog::Accepted) {
        const QList<QListWidgetItem *> selectedLogLevels = logLevels->selectedItems();
        if (!selectedLogLevels.isEmpty()) {
            QListWidgetItem *logLevel = selectedLogLevels.at(0);
            int selectedLogLevel = logLevels->row(logLevel);

            const QList<QListWidgetItem *> selectedItems = fileList->selectedItems();
            for (QListWidgetItem *item : selectedItems) {
                item->setIcon(logLevel->icon());
                item->setData(LogLevelFileList::LogLevelRole, selectedLogLevel);
            }

            Q_EMIT fileListChanged();
        }
    }
}

LogLevel *LogLevelFileList::level(int i) const
{
    return Globals::instance().logLevels().value(Globals::LogLevelIds(fileList->item(i)->data(LogLevelFileList::LogLevelRole).toInt()));
}

QList<int> LogLevelFileList::levels() const
{
    QList<int> levels;
    const int count = fileList->count();
    levels.reserve(count);
    for (int i = 0; i < count; i++) {
        levels.append(this->level(i)->id());
    }

    return levels;
}

void LogLevelFileList::addPaths(const QStringList &stringList, const QList<int> &valueList)
{
    // A little security test
    if (stringList.size() != valueList.size()) {
        logDebug() << i18n("The two arrays size are different, skipping the reading of generic paths.");
        return;
    }

    QListIterator<QString> itString(stringList);
    QListIterator<int> itInt = (valueList);

    bool missingFiles = false;

    while (itString.hasNext()) {
        int valueInt = itInt.next();
        const QString valueString = itString.next();
        bool missingFile = false;

        QFileInfo checkFile(valueString);
        if (!checkFile.exists()) {
            missingFiles = true;
            missingFile = true;
        }

        LogLevel *level = Globals::instance().logLevels().value(Globals::LogLevelIds(valueInt), Globals::instance().informationLogLevel());
        insertItem(level, valueString, missingFile);
    }

    mWarningBox->setVisible(missingFiles);

    Q_EMIT fileListChanged();
}
