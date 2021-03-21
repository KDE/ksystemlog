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

