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

class LogLinePrivate
{
public:
    long internalId;

    QDateTime time;

    QStringList logItems;

    QString originalFile;

    LogLevel *logLevel;

    LogMode *logMode;

    bool recent;

    LogViewWidgetItem *item;
};

LogLine::LogLine(long internalId, const QDateTime &dateTime, const QStringList &logItems, const QString &file,
                 LogLevel *logLevel, LogMode *logMode)
    :

    d(new LogLinePrivate())
{
    d->internalId = internalId;
    d->time = dateTime;
    d->logItems = logItems;
    d->originalFile = file;
    d->logLevel = logLevel;
    d->logMode = logMode;

    // No linked item when constructs this LogLine
    d->item = nullptr;

    // By default in newly created item has the recent state
    setRecent(true);
}

LogLine::~LogLine()
{
    // logLevel is managed by Globals

    // item is managed by LogMode
    // itemBuilder is managed by LogMode

    delete d;
}

LogMode *LogLine::logMode() const
{
    return d->logMode;
}

void LogLine::setLogMode(LogMode *logMode)
{
    d->logMode = logMode;
}

bool LogLine::equals(const LogLine &other) const
{
    if (logMode()->id() != other.logMode()->id())
        return false;

    if (time() != other.time())
        return false;

    if (logLevel()->id() != other.logLevel()->id())
        return false;

    if (d->logItems != other.d->logItems)
        return false;

    return true;
}

LogLevel *LogLine::logLevel() const
{
    return d->logLevel;
}

void LogLine::setLogLevel(LogLevel *level)
{
    d->logLevel = level;
}

QDateTime LogLine::time() const
{
    return d->time;
}

void LogLine::setLogItems(const QStringList &logItems)
{
    d->logItems = logItems;
}

QStringList LogLine::logItems() const
{
    return d->logItems;
}

QString LogLine::sourceFileName() const
{
    return d->originalFile;
}

bool LogLine::isOlderThan(const LogLine &other) const
{
    if (d->time == other.time())
        return d->internalId < other.internalId();

    return d->time < other.time();
}

bool LogLine::isNewerThan(const LogLine &other) const
{
    if (d->time == other.time())
        return d->internalId > other.internalId();

    return d->time > other.time();
}

bool LogLine::isSameTime(const LogLine &other) const
{
    return d->time == other.time();
}

long LogLine::internalId() const
{
    return d->internalId;
}

void LogLine::setRecent(bool recent)
{
    d->recent = recent;

    if (d->item != nullptr) {
        QFont currentFont = d->item->font(d->item->columnCount() - 1);

        // We avoid doing the same process
        if (d->recent != currentFont.bold()) {
            currentFont.setBold(recent);
            d->item->setFont(d->item->columnCount() - 1, currentFont);
        }
    }
}

QString LogLine::exportToText() const
{
    QString exporting;

    if (d->item == nullptr) {
        logCritical() << "Trying to export text from NULL item";
        return exporting;
    }

    for (int i = 0; i < d->item->columnCount(); ++i) {
        if (i > 0)
            exporting.append(QLatin1Char('\t'));

        exporting.append(d->item->text(i));
    }

    return exporting;
}

QString LogLine::formattedText()
{
    return d->logMode->itemBuilder()->createFormattedText(this);
}

void LogLine::setItem(LogViewWidgetItem *item)
{
    d->item = item;

    initializeItem();
}

void LogLine::initializeItem()
{
    d->logMode->itemBuilder()->prepareItem(d->item);

    // Call methods that change the look of the item
    setRecent(d->recent);

    if (KSystemLogConfig::colorizeLogLines()) {
        // Last column index = d->logItems.count() = (d->logItems.count() -1) +1 (the date column)
        d->item->setForeground(d->logItems.count(), QBrush(d->logLevel->color()));
    }

    d->item->toggleToolTip(KSystemLogConfig::tooltipEnabled());
}
