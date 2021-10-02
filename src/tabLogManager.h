/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class QString;

class LogManager;

/**
 * Class that wrap a LogManager inside a tabbed view
 */
class TabLogManager : public QObject
{
    Q_OBJECT

public:
    explicit TabLogManager(LogManager *logManager);

    ~TabLogManager() override;

    LogManager *logManager() const;

    void addNewLinesCount(int newLines);
    void initNewLinesCount();

    QString title() const;

private:
    QString logModeName() const;
    LogManager *const mLogManager;
    int mNewLinesCount = 0;
};

