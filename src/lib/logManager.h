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

#pragma once

#include <QList>

#include "globals.h"

#include "logFile.h"
#include "logMode.h"

#include "logViewColumns.h"
#include "logViewModel.h"

class View;

class LogManagerPrivate;

class LogManager : public QObject
{
    Q_OBJECT

public:
    explicit LogManager(View *view);

    ~LogManager() override;

    View *usedView() const;

    const QTime &lastUpdate() const;

    LogMode *logMode();

    QString title() const;

    void initialize(LogMode *mode, const QVariant &analyzerOptions = QVariant());

    void setParsingPaused(bool paused);
    bool isParsingPaused() const;

    void reload();
    void stopWatching();

    const QVariant &analyzerOptions() const;

protected Q_SLOTS:

    void updateLog(int lineCount);

    void showErrorMessage(const QString &title, const QString &message);

Q_SIGNALS:
    void tabTitleChanged(View *view, const QIcon &icon, const QString &label);

    void windowTitleChanged(const QString &caption);
    void statusBarChanged(const QString &message);

    void reloaded();
    void logUpdated(View *view, int addedLines);

private Q_SLOTS:
    void loadDroppedUrls(const QList<QUrl> &urls);

private:
    void internalInitialize(LogMode *mode, const QVector<LogFile> &logFiles, const QVariant &analyzerOptions = QVariant());

    void cleanPreviousLogMode();

    LogManagerPrivate *d;
};

