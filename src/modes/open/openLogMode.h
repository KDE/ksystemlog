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

#ifndef OPEN_LOG_MODE_H
#define OPEN_LOG_MODE_H

/**
 * Open Log Mode Identifier
 */
#define OPEN_LOG_MODE_ID "openLogMode"

/**
 * System Log Icon
 */
#define OPEN_MODE_ICON "document-open"

#include <QList>

#include "logFile.h"

#include "logMode.h"

class QWidget;

class OpenLogMode : public LogMode
{
    Q_OBJECT

public:
    explicit OpenLogMode(QWidget *parent);

    ~OpenLogMode() override;

    Analyzer *createAnalyzer(const QVariant &options = QVariant()) override;

    QList<LogFile> createLogFiles() override;

private:
    QWidget *const mParent;
};

#endif // _OPEN_LOG_MODE_H
