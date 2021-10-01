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

#include "cronLogMode.h"

#include <KLocalizedString>

#include "logging.h"

#include "cronAnalyzer.h"
#include "cronConfiguration.h"
#include "cronConfigurationWidget.h"
#include "cronItemBuilder.h"

CronLogMode::CronLogMode()
    : LogMode(QStringLiteral(CRON_LOG_MODE_ID), i18n("Cron Log"), QStringLiteral(CRON_MODE_ICON))
{
    d->logModeConfiguration = QSharedPointer<CronConfiguration>(new CronConfiguration());

    d->logModeConfigurationWidget = new CronConfigurationWidget();

    d->itemBuilder = new CronItemBuilder();

    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the planned tasks log (Cron log)."));
    d->action->setWhatsThis(
        i18n("Displays the planned tasks log in the current tab. Cron process is a program in charge of launching "
             "planned tasks on your system, like security checks, or auto-restarting of some services. Use this "
             "menu to see the recently launched processes."));

    auto *cronConfiguration = logModeConfiguration<CronConfiguration *>();
    checkLogFilesPresence(cronConfiguration->cronPaths());
}

CronLogMode::~CronLogMode()
{
}

Analyzer *CronLogMode::createAnalyzer(const QVariant &options)
{
    Q_UNUSED(options)
    return new CronAnalyzer(this);
}

QVector<LogFile> CronLogMode::createLogFiles()
{
    auto *cronConfiguration = logModeConfiguration<CronConfiguration *>();
    return cronConfiguration->findNoModeLogFiles(cronConfiguration->cronPaths());
}
