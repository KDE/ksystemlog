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

#include "xorgLogMode.h"

#include <KLocalizedString>

#include "logging.h"

#include "xorgAnalyzer.h"
#include "xorgConfiguration.h"
#include "xorgConfigurationWidget.h"
#include "xorgItemBuilder.h"

XorgLogMode::XorgLogMode()
    : LogMode(QStringLiteral(XORG_LOG_MODE_ID), i18n("X.org Log"), QStringLiteral(XORG_MODE_ICON))
{
    d->logModeConfiguration = QSharedPointer<XorgConfiguration>(new XorgConfiguration());

    d->logModeConfigurationWidget = new XorgConfigurationWidget();

    d->itemBuilder = new XorgItemBuilder();

    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the X.org log."));
    d->action->setWhatsThis(
        i18n("Displays the X.org log in the current tab. X.org is the service which displays on screen your "
             "desktop and manage your graphical hardware. See this log if you want to know why you do not have 3D "
             "accelerations or why your input device is not recognized."));

    auto *configuration = logModeConfiguration<XorgConfiguration *>();
    checkLogFilesPresence(configuration->xorgPaths());
}

XorgLogMode::~XorgLogMode()
{
}

Analyzer *XorgLogMode::createAnalyzer(const QVariant &options)
{
    Q_UNUSED(options)
    return new XorgAnalyzer(this);
}

QVector<LogFile> XorgLogMode::createLogFiles()
{
    auto *configuration = logModeConfiguration<XorgConfiguration *>();
    return configuration->findNoModeLogFiles(configuration->xorgPaths());
}
