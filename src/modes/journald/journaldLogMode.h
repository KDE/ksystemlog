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

#ifndef _JOURNALD_LOG_MODE_H_
#define _JOURNALD_LOG_MODE_H_

/**
 * Journald Log Mode Identifier
 */
#define JOURNALD_LOG_MODE_ID "journaldLogMode"

/**
 * System Log Icon
 */
#define JOURNALD_MODE_ICON "computer"

#include <QList>

#include "logFile.h"
#include "logMode.h"

enum class JournaldAnalyzerType { Local, Network };
struct JournaldAnalyzerOptions {
    JournaldAnalyzerType analyzerType = JournaldAnalyzerType::Local;
    QString filter;
    QString address;
    quint16 port;
};
Q_DECLARE_METATYPE(JournaldAnalyzerOptions)

class JournaldLogMode : public LogMode
{
    Q_OBJECT

public:
    explicit JournaldLogMode();

    ~JournaldLogMode();

    Analyzer *createAnalyzer(const QVariant &options = QVariant());

    QList<LogFile> createLogFiles();
};

#endif // _JOURNALD_LOG_MODE_H_
