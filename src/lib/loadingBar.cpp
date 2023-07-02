/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "loadingBar.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>

#include <KLocalizedString>
LoadingBar::LoadingBar(QWidget *parent)
    : QWidget(parent)
{
    auto widgetLayout = new QHBoxLayout(this);

    widgetLayout->addStretch();

    auto layout = new QVBoxLayout();
    widgetLayout->addLayout(layout);

    widgetLayout->addStretch();

    mLabel = new QLabel(i18n("Loading Progress..."), this);
    mLabel->setMinimumWidth(250);
    layout->addWidget(mLabel, 1, Qt::AlignBottom);

    mProgressBar = new QProgressBar(this);
    mProgressBar->setRange(0, 100);
    mProgressBar->setMinimumWidth(250);
    layout->addWidget(mProgressBar, 1, Qt::AlignCenter | Qt::AlignTop);
}

LoadingBar::~LoadingBar()
{
}

QProgressBar *LoadingBar::progressBar() const
{
    return mProgressBar;
}

void LoadingBar::startLoading(const LogMode &logMode, const LogFile &logFile, int fileIndex, int fileCount)
{
    Q_EMIT displayed(true);

    mProgressBar->setValue(0);

    // Several files to load
    if (fileCount > 1 && fileIndex >= 1) {
        if (mFirstLoading) {
            mLabel->setText(i18np("Loading <b>%2</b>...<br /><i>%3</i> - (<b>%4</b>)",
                                  "Loading <b>%2</b>...<br /><i>%3</i> - (<b>%4</b>/%1 files)",
                                  fileCount,
                                  logMode.name(),
                                  logFile.url().toLocalFile(),
                                  fileIndex));
        } else {
            mLabel->setText(i18np("Reloading <b>%2</b>...<br /><i>%3</i> - (<b>%4</b>)",
                                  "Reloading <b>%2</b>...<br /><i>%3</i> - (<b>%4</b>/%1 files)",
                                  fileCount,
                                  logMode.name(),
                                  logFile.url().toLocalFile(),
                                  fileIndex));
        }
    }
    // Only one file
    else {
        if (mFirstLoading) {
            mLabel->setText(i18n("Loading <b>%1</b>...<br /><i>%2</i>", logMode.name(), logFile.url().toLocalFile()));
        } else {
            mLabel->setText(i18n("Reloading <b>%1</b>...<br /><i>%2</i>", logMode.name(), logFile.url().toLocalFile()));
        }
    }
}

void LoadingBar::endLoading()
{
    Q_EMIT displayed(false);

    mProgressBar->setValue(100);

    // If the endLoading has been called one time, it means it has already been loaded
    mFirstLoading = false;
}

void LoadingBar::progressLoading()
{
    mProgressBar->setValue(mProgressBar->value() + 1);

    // kapp->processEvents();
    qApp->processEvents();
}

#include "moc_loadingBar.cpp"
