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

#include <QStringList>

#include "logModeConfiguration.h"

#include "defaults.h"
#include "logging.h"

#include "sambaLogMode.h"

#include "ksystemlogConfig.h"

class SambaConfiguration : public LogModeConfiguration
{
    Q_OBJECT

public:
    SambaConfiguration();

    ~SambaConfiguration() override;

    QStringList sambaPaths() const;

    QStringList sambaAccessPaths() const;

    QStringList netbiosPaths() const;

    void setSambaPaths(const QStringList &sambaPaths);

    void setNetbiosPaths(const QStringList &netbiosPaths);

    void setSambaAccessPaths(const QStringList &sambaAccessPaths);

private:
    QStringList mSambaPaths;

    QStringList mSambaAccessPaths;

    QStringList mNetbiosPaths;
};

