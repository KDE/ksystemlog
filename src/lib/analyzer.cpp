/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "analyzer.h"

#include "ksystemlogConfig.h"
#include "logging.h"

#include "logViewModel.h"

#include "logFileReader.h"
#include "logMode.h"

Analyzer::Analyzer(LogMode *mode)
    : QObject(nullptr)
    , mLogMode(mode)
    , mInsertionLocking()
{
}

Analyzer::~Analyzer()
{
    // logMode is managed by Globals
    // logViewModel is managed by LogViewWidget
}

bool Analyzer::isParsingPaused() const
{
    return mParsingPaused;
}

void Analyzer::setParsingPaused(bool paused)
{
    mParsingPaused = paused;

    bool watching;
    // If we resume the parsing, then parse files to know if new lines have been appended.
    if (mParsingPaused) {
        logDebug() << "Pausing reading";
        watching = false;
    } else {
        logDebug() << "Relaunch reading";
        watching = true;
    }

    watchLogFiles(watching);
}

void Analyzer::setLogViewModel(LogViewModel *logViewModel)
{
    mLogViewModel = logViewModel;
}

void Analyzer::informOpeningProgress(int currentPosition, int total)
{
    const int each = total / 100;
    if (each == 0) {
        return;
    }

    if (currentPosition % each == 0) {
        Q_EMIT openingProgressed();
    }
}
