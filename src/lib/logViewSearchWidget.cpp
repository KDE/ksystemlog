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

#include "logViewSearchWidget.h"

#include <QPushButton>
#include <QString>
#include <QCheckBox>
#include <QPalette>
#include <QColor>
#include <QTimer>

#include <QIcon>
#include <KLocalizedString>

#include "logging.h"
#include "logViewWidget.h"
#include "logViewWidgetItem.h"

LogViewSearchWidget::LogViewSearchWidget()
{
    setupUi(this);

    // Get the searchLine base color to be able to restore it later
    mSearchLineBaseColor = searchLine->palette().color(QPalette::Base);
    mSearchLineTextColor = searchLine->palette().color(QPalette::Text);

    // Default configuration of the hiding message timer
    mMessageHidingTimer = new QTimer(this);
    mMessageHidingTimer->setSingleShot(true);
    mMessageHidingTimer->setInterval(2000);
    connect(mMessageHidingTimer, &QTimer::timeout, this, &LogViewSearchWidget::hideMessage);

    // The message widget is hidden by default
    hideMessage();

    closeButton->setIcon(QIcon::fromTheme(QStringLiteral("dialog-close")));
    connect(closeButton, &QAbstractButton::clicked, this, &QWidget::hide);

    next->setIcon(QIcon::fromTheme(QStringLiteral("arrow-down")));
    connect(next, &QAbstractButton::clicked, this, &LogViewSearchWidget::findNext);

    previous->setIcon(QIcon::fromTheme(QStringLiteral("arrow-up")));
    connect(previous, &QAbstractButton::clicked, this, &LogViewSearchWidget::findPrevious);

    searchLabel->setBuddy(searchLine);

    connect(searchLine, SIGNAL(textEdited(QString)), this, SLOT(findFirst(QString)));
    connect(searchLine, &QLineEdit::textEdited, this, &LogViewSearchWidget::highlightAll);

    connect(searchLine, &QLineEdit::returnPressed, this, &LogViewSearchWidget::findNext);

    connect(caseSensitive, SIGNAL(clicked()), this, SLOT(findFirst()));

    connect(caseSensitive, &QAbstractButton::clicked, this, &LogViewSearchWidget::highlightAll);
    connect(highlightAllButton, &QAbstractButton::clicked, this, &LogViewSearchWidget::highlightAll);

    findFirst(searchLine->text());
}

LogViewSearchWidget::~LogViewSearchWidget()
{
}

void LogViewSearchWidget::displaySearch()
{
    searchLine->setFocus();
    searchLine->setSelection(0, searchLine->text().length());
    show();
}

void LogViewSearchWidget::setTreeWidget(LogViewWidget *logViewWidget)
{
    mLogViewWidget = logViewWidget;
}

void LogViewSearchWidget::findFirst(const QString &text)
{
    const bool textIsNotEmpty = !text.isEmpty();
    next->setEnabled(textIsNotEmpty);
    previous->setEnabled(textIsNotEmpty);
    if (textIsNotEmpty) {
        findFirst();
    }
}

void LogViewSearchWidget::findFirst()
{
    internalFind(nullptr, LogViewSearchWidget::Next);
}

void LogViewSearchWidget::findNext()
{
    logDebug() << "Finding next";

    LogViewWidgetItem *lastSelectedItem = mLogViewWidget->lastSelectedItem();
    internalFind(lastSelectedItem, LogViewSearchWidget::Next);
}

void LogViewSearchWidget::findPrevious()
{
    logDebug() << "Finding previous";

    LogViewWidgetItem *firstSelectedItem = mLogViewWidget->firstSelectedItem();
    internalFind(firstSelectedItem, LogViewSearchWidget::Previous);
}

void LogViewSearchWidget::internalFind(LogViewWidgetItem *fromItem, Direction direction)
{
    if (searchLine->text().isEmpty()) {
        return;
    }

    QTreeWidgetItemIterator it(mLogViewWidget, QTreeWidgetItemIterator::NotHidden);
    initIterator(it, direction);

    // Go to the selected position + 1 (if we already found an item)
    if (fromItem != nullptr) {
        while (*it != nullptr) {
            LogViewWidgetItem *item = static_cast<LogViewWidgetItem *>(*it);

            if (item == fromItem) {
                iteratorJump(it, direction);
                break;
            }

            iteratorJump(it, direction);
        }
    }

    // Iterates to fromItem +1 to the last item of the list
    while (*it != nullptr) {
        LogViewWidgetItem *item = static_cast<LogViewWidgetItem *>(*it);

        bool found = findItem(item);
        if (found == true) {
            return;
        }

        iteratorJump(it, direction);
    }

    // If we do not begin the search from the beginning, we do it now
    if (fromItem != nullptr) {
        it = QTreeWidgetItemIterator(mLogViewWidget, QTreeWidgetItemIterator::NotHidden);
        initIterator(it, direction);

        LogViewWidgetItem *item = nullptr;
        while (*it != nullptr && item != fromItem) {
            item = static_cast<LogViewWidgetItem *>(*it);

            bool found = findItem(item);
            if (found == true) {
                showMessage(i18n("Reached end of list."), QStringLiteral("dialog-information"));
                return;
            }

            iteratorJump(it, direction);
        }
    }

    setSearchLineNotFound(true);
}

inline void LogViewSearchWidget::initIterator(QTreeWidgetItemIterator &it, Direction direction)
{
    // Previous direction : Go to the last item
    if (direction == LogViewSearchWidget::Previous) {
        QTreeWidgetItemIterator testedIterator(it);
        while (true) {
            ++testedIterator;
            if (*testedIterator == nullptr) {
                break;
            }

            ++it;
        }
    }
}

inline void LogViewSearchWidget::iteratorJump(QTreeWidgetItemIterator &it, Direction direction)
{
    if (direction == LogViewSearchWidget::Next) {
        ++it;
    } else {
        --it;
    }
}

bool LogViewSearchWidget::compareItem(LogViewWidgetItem *item)
{
    Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive;
    if (caseSensitive->isChecked()) {
        caseSensitivity = Qt::CaseSensitive;
    }

    if (searchLine->text().isEmpty()) {
        return false;
    }

    if (item->logLine()->exportToText().contains(searchLine->text(), caseSensitivity)) {
        return true;
    }

    return false;
}

bool LogViewSearchWidget::findItem(LogViewWidgetItem *item)
{
    if (compareItem(item) == true) {
        unselectAll();

        setSearchLineNotFound(false);
        item->setSelected(true);
        mLogViewWidget->setCurrentItem(item);
        mLogViewWidget->scrollToItem(item);
        return true;
    }

    return false;
}

void LogViewSearchWidget::setSearchLineNotFound(bool notFound)
{
    QPalette palette = searchLine->palette();
    if (notFound == true) {
        palette.setColor(QPalette::Base, QColor(255, 102, 102)); // or Qt::red or QColor(235, 0, 0)
        palette.setColor(QPalette::Text, QColor(255, 255, 255));
    } else {
        palette.setColor(QPalette::Base, mSearchLineBaseColor);
        palette.setColor(QPalette::Text, mSearchLineTextColor);
    }

    searchLine->setPalette(palette);

    if (notFound == true) {
        showMessage(i18n("Phrase not found."), QStringLiteral("dialog-error"));
    } else {
        hideMessage();
    }
}

void LogViewSearchWidget::unselectAll()
{
    const QList<QTreeWidgetItem *> selectedItems = mLogViewWidget->selectedItems();
    for (QTreeWidgetItem *item : selectedItems) {
        item->setSelected(false);
    }
}

void LogViewSearchWidget::showMessage(const QString &text, const QString &iconText)
{
    message->setText(text);
    message->show();

    messageIcon->setPixmap(QIcon::fromTheme(iconText).pixmap(style()->pixelMetric(QStyle::PM_SmallIconSize)));
    messageIcon->show();

    mMessageHidingTimer->start();
}

void LogViewSearchWidget::hideMessage()
{
    message->hide();
    messageIcon->hide();

    mMessageHidingTimer->stop();
}

void LogViewSearchWidget::highlightAll()
{
    if (highlightAllButton->isChecked()) {
        unlightAll();

        logDebug() << "Highlighting all";
        QTreeWidgetItemIterator it(mLogViewWidget, QTreeWidgetItemIterator::All);
        while (*it != nullptr) {
            LogViewWidgetItem *item = static_cast<LogViewWidgetItem *>(*it);

            if (compareItem(item) == true) {
                item->setBackground(item->columnCount() - 1, QColor(255, 255, 16 * 8 + 11));
            }

            ++it;
        }
    } else {
        unlightAll();
    }
}

void LogViewSearchWidget::unlightAll()
{
    logDebug() << "Unlighting all";

    QTreeWidgetItemIterator it(mLogViewWidget, QTreeWidgetItemIterator::All);
    while (*it != nullptr) {
        LogViewWidgetItem *item = static_cast<LogViewWidgetItem *>(*it);

        // We retrieve the default column background using the first column data, where the background never
        // changes
        item->setBackground(item->columnCount() - 1,
                            qvariant_cast<QBrush>(item->data(0, Qt::BackgroundRole)));

        ++it;
    }
}
