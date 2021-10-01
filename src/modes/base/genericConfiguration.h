/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
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

#include <QList>
#include <QStringList>

#include "logFile.h"

#include "logModeConfiguration.h"

class GenericLogModeConfigurationPrivate;

class GenericLogModeConfiguration : public LogModeConfiguration
{
    Q_OBJECT

public:
    GenericLogModeConfiguration(const QString &configurationGroup, const QStringList &defaultLogFilesPaths, const QList<int> &defaultLogFilesLevels);
    ~GenericLogModeConfiguration() override;

    QStringList logFilesPaths() const;

    QList<int> logFilesLevels() const;

    void setLogFilesPaths(const QStringList &logFilesPaths);

    void setLogFilesLevels(const QList<int> &logFilesLevels);

    QVector<LogFile> findGenericLogFiles() const;

private:
    QStringList mLogFilesPaths;

    QList<int> mLogFilesLevels;
};

