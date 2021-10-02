/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QDateTime>
#include <QString>

class LogViewWidgetItem;
class LogLine;

class LogModeItemBuilder
{
public:
    LogModeItemBuilder();

    virtual ~LogModeItemBuilder();

    virtual void prepareItem(LogViewWidgetItem *item) const;

    virtual QString createFormattedText(LogLine *line) const;
    virtual QString createToolTipText(LogLine *line) const;

protected:
    QString labelMessageFormat(const QString &label, const QString &value) const;
    QString messageFormat(const QString &message) const;

    virtual QString formatDate(const QDateTime &dateTime) const;
};

