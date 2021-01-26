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

#ifndef KERNEL_ANALYZER_H
#define KERNEL_ANALYZER_H

#include <QFile>
#include <QRegExp>

#include <KLocalizedString>

#include "logging.h"

#include "fileAnalyzer.h"

#include "kernelLogMode.h"
#include "localLogFileReader.h"
#include "processOutputLogFileReader.h"

class LogMode;

class KernelAnalyzer : public FileAnalyzer
{
    Q_OBJECT

public:
    explicit KernelAnalyzer(LogMode *logMode);

    ~KernelAnalyzer() override
    {
    }

    LogViewColumns initColumns() override;

protected:
    LogFileReader *createLogFileReader(const LogFile &logFile) override;

    Analyzer::LogFileSortMode logFileSortMode() override
    {
        return Analyzer::AscendingSortedLogFile;
    }

    void startupTime();

    LogLine *parseMessage(const QString &logLine, const LogFile &originalLogFile) override;

    inline void parseComponentMessage(const QString &logLine, QStringList &messages)
    {
        QString message(logLine);
        QString component;

        int doublePointPosition = message.indexOf(QLatin1Char(':'));

        // Estimate the max size of a component
        if (doublePointPosition != -1 && doublePointPosition < 20) {
            component = message.left(doublePointPosition);
            // Remove component length + ": "
            message.remove(0, doublePointPosition + 2);
        }

        messages.append(component);
        messages.append(message.simplified());
    }

protected:
    QDateTime mStartupDateTime;
};

#endif
