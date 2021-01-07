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

#include "logLine.h"

#include "logMode.h"
#include "logModeItemBuilder.h"
#include "logViewWidgetItem.h"

#include "logging.h"
#include "ksystemlogConfig.h"

LogLine::LogLine(long internalId, const QDateTime &dateTime, const QStringList &logItems, const QString &file, LogLevel *logLevel, LogMode *logMode)
{
    mInternalId = internalId;
    mTime = dateTime;
    mLogItems = logItems;
    mOriginalFile = file;
    mLogLevel = logLevel;
    mLogMode = logMode;

    // No linked item when constructs this LogLine
    mItem = nullptr;

    // By default in newly created item has the recent state
    setRecent(true);
}

LogLine::~LogLine()
{
    // logLevel is managed by Globals

    // item is managed by LogMode
    // itemBuilder is managed by LogMode
}

LogMode *LogLine::logMode() const
{
    return mLogMode;
}

void LogLine::setLogMode(LogMode *logMode)
{
    mLogMode = logMode;
}

bool LogLine::equals(const LogLine &other) const
{
    if (logMode()->id() != other.logMode()->id()) {
        return false;
    }

    if (time() != other.time()) {
        return false;
    }

    if (logLevel()->id() != other.logLevel()->id()) {
        return false;
    }

    if (mLogItems != other.mLogItems) {
        return false;
    }

    return true;
}

LogLevel *LogLine::logLevel() const
{
    return mLogLevel;
}

void LogLine::setLogLevel(LogLevel *level)
{
    mLogLevel = level;
}

QDateTime LogLine::time() const
{
    return mTime;
}

void LogLine::setLogItems(const QStringList &logItems)
{
    mLogItems = logItems;
}

QStringList LogLine::logItems() const
{
    return mLogItems;
}

QString LogLine::sourceFileName() const
{
    return mOriginalFile;
}

bool LogLine::isOlderThan(const LogLine &other) const
{
    if (mTime == other.time()) {
        return mInternalId < other.internalId();
    }

    return mTime < other.time();
}

bool LogLine::isNewerThan(const LogLine &other) const
{
    if (mTime == other.time()) {
        return mInternalId > other.internalId();
    }

    return mTime > other.time();
}

bool LogLine::isSameTime(const LogLine &other) const
{
    return mTime == other.time();
}

long LogLine::internalId() const
{
    return mInternalId;
}

void LogLine::setRecent(bool recent)
{
    mRecent = recent;

    if (mItem != nullptr) {
        QFont currentFont = mItem->font(mItem->columnCount() - 1);

        // We avoid doing the same process
        if (mRecent != currentFont.bold()) {
            currentFont.setBold(recent);
            mItem->setFont(mItem->columnCount() - 1, currentFont);
        }
    }
}

QString LogLine::exportToText() const
{
    QString exporting;

    if (mItem == nullptr) {
        logCritical() << "Trying to export text from NULL item";
        return exporting;
    }

    for (int i = 0; i < mItem->columnCount(); ++i) {
        if (i > 0) {
            exporting.append(QLatin1Char('\t'));
        }

        exporting.append(mItem->text(i));
    }

    return exporting;
}

QString LogLine::formattedText()
{
    return mLogMode->itemBuilder()->createFormattedText(this);
}

void LogLine::setItem(LogViewWidgetItem *item)
{
    mItem = item;

    initializeItem();
}

void LogLine::initializeItem()
{
    mLogMode->itemBuilder()->prepareItem(mItem);

    // Call methods that change the look of the item
    setRecent(mRecent);

    if (KSystemLogConfig::colorizeLogLines()) {
        // Last column index = logItems.count() = (logItems.count() -1) +1 (the date column)
        mItem->setForeground(mLogItems.count(), QBrush(mLogLevel->color()));
    }

    mItem->toggleToolTip(KSystemLogConfig::tooltipEnabled());
}
