/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QTreeWidgetItemIterator>
#include <QWidget>

#include "ui_logViewSearchWidgetBase.h"

class LogViewWidget;
class LogViewWidgetItem;

class LogViewSearchWidget : public QWidget, public Ui::LogViewSearchWidgetBase
{
    Q_OBJECT

public:
    explicit LogViewSearchWidget(QWidget *parent = nullptr);

    ~LogViewSearchWidget() override;

    void setTreeWidget(LogViewWidget *logViewWidget);

public Q_SLOTS:
    void displaySearch();

    void findFirst();

    void findFirst(const QString &);
    void findNext();
    void findPrevious();

private Q_SLOTS:
    void hideMessage();

    void highlightAll();

private:
    enum Direction {
        Next,
        Previous
    };
    Q_DECLARE_FLAGS(Directions, Direction)

    void showMessage(const QString &text, const QString &iconText);

    void internalFind(LogViewWidgetItem *fromItem, Direction direction);

    void setSearchLineNotFound(bool notFound);
    void unselectAll();
    bool findItem(LogViewWidgetItem *item);

    bool compareItem(LogViewWidgetItem *item);

    void unlightAll();

    inline void initIterator(QTreeWidgetItemIterator &it, Direction direction);
    inline void iteratorJump(QTreeWidgetItemIterator &it, Direction direction);
    LogViewWidget *mLogViewWidget = nullptr;

    QColor mSearchLineBaseColor;
    QColor mSearchLineTextColor;

    QTimer *mMessageHidingTimer = nullptr;
};
