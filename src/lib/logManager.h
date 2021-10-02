/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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

