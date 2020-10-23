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

#include "loadingBar.h"

#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <KLocalizedString>

class LoadingBarPrivate
{
public:
    // Attributes managing the position in the files loading of each log
    int fileCount;
    int currentFileIndex;

    bool firstLoading;

    QLabel *label;

    QProgressBar *progressBar;
};

LoadingBar::LoadingBar(QWidget *parent)
    : QWidget(parent)
    , d(new LoadingBarPrivate())
{
    d->firstLoading = true;

    QHBoxLayout *widgetLayout = new QHBoxLayout(this);

    widgetLayout->addStretch();

    QVBoxLayout *layout = new QVBoxLayout();
    widgetLayout->addLayout(layout);

    widgetLayout->addStretch();

    d->label = new QLabel(i18n("Loading Progress..."));
    d->label->setMinimumWidth(250);
    layout->addWidget(d->label, 1, Qt::AlignBottom);

    d->progressBar = new QProgressBar();
    d->progressBar->setRange(0, 100);
    d->progressBar->setMinimumWidth(250);
    layout->addWidget(d->progressBar, 1, Qt::AlignCenter | Qt::AlignTop);
}

LoadingBar::~LoadingBar()
{
    delete d;
}

QProgressBar *LoadingBar::progressBar()
{
    return d->progressBar;
}

void LoadingBar::startLoading(const LogMode &logMode, const LogFile &logFile, int fileIndex, int fileCount)
{
    emit displayed(true);

    d->progressBar->setValue(0);

    // Several files to load
    if (fileCount > 1 && fileIndex >= 1) {
        if (d->firstLoading)
            d->label->setText(i18np("Loading <b>%2</b>...<br /><i>%3</i> - (<b>%4</b>)",
                                    "Loading <b>%2</b>...<br /><i>%3</i> - (<b>%4</b>/%1 files)", fileCount,
                                    logMode.name(), logFile.url().toLocalFile(), fileIndex));
        else
            d->label->setText(i18np("Reloading <b>%2</b>...<br /><i>%3</i> - (<b>%4</b>)",
                                    "Reloading <b>%2</b>...<br /><i>%3</i> - (<b>%4</b>/%1 files)", fileCount,
                                    logMode.name(), logFile.url().toLocalFile(), fileIndex));
    }
    // Only one file
    else {
        if (d->firstLoading)
            d->label->setText(
                i18n("Loading <b>%1</b>...<br /><i>%2</i>", logMode.name(), logFile.url().toLocalFile()));
        else
            d->label->setText(
                i18n("Reloading <b>%1</b>...<br /><i>%2</i>", logMode.name(), logFile.url().toLocalFile()));
    }
}

void LoadingBar::endLoading()
{
    emit displayed(false);

    d->progressBar->setValue(100);

    // If the endLoading has been called one time, it means it has already been loaded
    d->firstLoading = false;
}

void LoadingBar::progressLoading()
{
    d->progressBar->setValue(d->progressBar->value() + 1);

    // kapp->processEvents();
    qApp->processEvents();
}
