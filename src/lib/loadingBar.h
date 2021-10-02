/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "logFile.h"
#include "logMode.h"

class QProgressBar;
class QLabel;
/**
 * TODO Try to have a working Cancel button (for the moment, it only hide the Loading Dialog)
 */
class LoadingBar : public QWidget
{
    Q_OBJECT

public:
    explicit LoadingBar(QWidget *parent = nullptr);

    ~LoadingBar() override;

    QProgressBar *progressBar() const;

public Q_SLOTS:

    void startLoading(const LogMode &logMode, const LogFile &logFile, int fileIndex, int fileCount);
    void endLoading();

    void progressLoading();

Q_SIGNALS:
    void displayed(bool displayed);

private:
    bool mFirstLoading = true;

    QLabel *mLabel = nullptr;

    QProgressBar *mProgressBar = nullptr;
};

