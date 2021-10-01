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

#include "netbiosLogMode.h"

#include <QAction>

#include <KLocalizedString>

#include "logging.h"

#include "sambaAnalyzer.h"
#include "sambaConfiguration.h"
#include "sambaConfigurationWidget.h"
#include "sambaItemBuilder.h"

NetbiosLogMode::NetbiosLogMode(QSharedPointer<SambaConfiguration> &sambaConfiguration,
                               SambaConfigurationWidget *sambaConfigurationWidget,
                               SambaItemBuilder *itemBuilder)
    : LogMode(QStringLiteral(NETBIOS_LOG_MODE_ID), i18n("Netbios Log"), QStringLiteral(NETBIOS_MODE_ICON))
{
    d->logModeConfiguration = sambaConfiguration;
    d->logModeConfigurationWidget = sambaConfigurationWidget;
    d->itemBuilder = itemBuilder;

    // Netbios Log Action
    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the Netbios log."));
    d->action->setWhatsThis(
        i18n("Displays the Netbios log in the current tab. Netbios is the file sharing protocol developed by "
             "Microsoft."));

    checkLogFilesPresence(sambaConfiguration->netbiosPaths());
}

NetbiosLogMode::~NetbiosLogMode()
{
}

Analyzer *NetbiosLogMode::createAnalyzer(const QVariant &options)
{
    Q_UNUSED(options)
    return new SambaAnalyzer(this);
}

QVector<LogFile> NetbiosLogMode::createLogFiles()
{
    auto *sambaConfiguration = logModeConfiguration<SambaConfiguration *>();
    return sambaConfiguration->findNoModeLogFiles(sambaConfiguration->netbiosPaths());
}
