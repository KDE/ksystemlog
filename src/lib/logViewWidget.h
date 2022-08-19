/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QList>
#include <QTreeWidget>

#include "logViewColumns.h"

class LogViewWidgetItem;
class LogViewModel;
class LogLine;
class QActionGroup;
class LogViewWidget : public QTreeWidget
{
    Q_OBJECT

public:
    explicit LogViewWidget(QWidget *parent = nullptr);

    ~LogViewWidget() override;
    void selectAll() override;
    void setColumns(const LogViewColumns &columns);

    int itemCount() const;
    int notHiddenItemCount();

    QList<LogViewWidgetItem *> items();
    QList<LogLine *> logLines();

    /**
     * Return the related widget item of this logLine or NULL if it has not been found
     */
    LogViewWidgetItem *findItem(LogLine *logLine);

    LogViewWidgetItem *findNewestItem();

    LogViewModel *model() const;

    bool hasItemsSelected();
    LogViewWidgetItem *firstSelectedItem();
    LogViewWidgetItem *lastSelectedItem();

    void scrollToNewestItem();

    void expandAll();
    void collapseAll();

    void resizeColumns();

public Q_SLOTS:
    void toggleToolTip(bool enabled);

Q_SIGNALS:
    void columnsChanged(const LogViewColumns &columns);

private Q_SLOTS:
    void toggleHeader(QAction *action);

private:
    LogViewModel *mLogViewModel = nullptr;

    QActionGroup *mHeadersTogglingActions = nullptr;
};
