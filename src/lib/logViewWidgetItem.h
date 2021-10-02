/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QTreeWidgetItem>

#include "globals.h"

#include "logLine.h"

class LogViewWidget;

class LogLine;

class LogViewWidgetItem : public QTreeWidgetItem
{
public:
    LogViewWidgetItem(LogViewWidget *logViewWidget, LogLine *line);

    ~LogViewWidgetItem() override;

    bool operator<(const QTreeWidgetItem &other) const override;

    LogLine *logLine() const;

    void toggleToolTip(bool displayed);

private:
    // TODO Move this log line to QTreeWidgetItem::data() method
    LogLine *mLine = nullptr;
};

