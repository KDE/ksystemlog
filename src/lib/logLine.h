/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QDateTime>
#include <QStringList>

#include "globals.h"

#include "logLevel.h"

class LogViewWidget;
class LogViewWidgetItem;

class LogLine
{
public:
    LogLine(long internalId, const QDateTime &dateTime, const QStringList &logItems, const QString &originalFile, LogLevel *level, LogMode *logMode);

    virtual ~LogLine();

    bool isOlderThan(const LogLine &other) const;
    bool isNewerThan(const LogLine &other) const;

    bool equals(const LogLine &other) const;

    bool isSameTime(const LogLine &other) const;

    LogLevel *logLevel() const;

    long internalId() const;
    QDateTime time() const;
    QStringList logItems() const;
    QString sourceFileName() const;

    LogMode *logMode() const;

    void setLogItems(const QStringList &logItems);
    void setLogLevel(LogLevel *level);
    void setLogMode(LogMode *logMode);

    void setRecent(bool recent);
    void setItem(LogViewWidgetItem *item);

    QString formattedText();

    QString exportToText() const;

protected:
    long mInternalId;

    QDateTime mTime;

    QStringList mLogItems;

    QString mOriginalFile;

    LogLevel *mLogLevel = nullptr;

    LogMode *mLogMode = nullptr;

    bool mRecent = false;

    LogViewWidgetItem *mItem = nullptr;

private:
    void initializeItem();
};
