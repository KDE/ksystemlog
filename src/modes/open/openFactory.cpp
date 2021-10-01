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

#include "openFactory.h"

#include "defaults.h"
#include "ksystemlogConfig.h"
#include "logFile.h"
#include "logMode.h"
#include "logging.h"

#include "logModeItemBuilder.h"
#include "openLogMode.h"

OpenLogModeFactory::OpenLogModeFactory(QWidget *parent)
    : mParent(parent)
{
}

OpenLogModeFactory::~OpenLogModeFactory()
{
}

QList<LogMode *> OpenLogModeFactory::createLogModes() const
{
    const QList<LogMode *> logModes{new OpenLogMode(mParent)};
    return logModes;
}

LogModeAction *OpenLogModeFactory::createLogModeAction() const
{
    return nullptr;
}
