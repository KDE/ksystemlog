/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QStandardItem>
#include <QWidget>

#include <KTreeWidgetSearchLine>

#include "globals.h"
#include "logViewColumns.h"

class LogViewWidget;
class LogViewWidgetSearchLine;
class QComboBox;

class LogViewFilterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LogViewFilterWidget(QWidget *parent = nullptr);

    ~LogViewFilterWidget() override;

    QComboBox *filterList() const;
    LogViewWidgetSearchLine *filterLine() const;

public Q_SLOTS:
    void updateFilterColumns(const LogViewColumns &list);

private Q_SLOTS:
    void changeColumnFilter(int column);
    void prioritiesChanged(QStandardItem *item);

Q_SIGNALS:
    void treeWidgetUpdated();

private:
    void initSearchListFilter();
    LogViewWidgetSearchLine *mFilterLine = nullptr;

    /**
     * Filter of the column list
     */
    QComboBox *mFilterList = nullptr;

    QComboBox *mPrioritiesComboBox = nullptr;

    QStandardItemModel *mPrioritiesModel = nullptr;
};

class LogViewWidgetSearchLine : public KTreeWidgetSearchLine
{
    Q_OBJECT

public:
    explicit LogViewWidgetSearchLine(QWidget *parent = nullptr);

    ~LogViewWidgetSearchLine() override;

    // Silence compiler warning
    using KTreeWidgetSearchLine::updateSearch;

    // Reimplemented just to send a signal _AFTER_ the tree updating
    void updateSearch(const QString &pattern = QString()) override;

    void setPriorityEnabled(int priority, bool enabled);

protected:
    bool itemMatches(const QTreeWidgetItem *item, const QString &pattern) const override;

Q_SIGNALS:
    void treeWidgetUpdated();

private:
    bool mPriorities[Globals::LOG_LEVEL_NUM];
};

