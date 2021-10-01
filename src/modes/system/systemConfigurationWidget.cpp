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

#include "systemConfigurationWidget.h"

SystemConfigurationWidget::SystemConfigurationWidget()
    : LogModeConfigurationWidget(i18n("System Log"), QStringLiteral(SYSTEM_MODE_ICON), i18n("System Log"))
{
    auto layout = new QVBoxLayout(this);

    const QString description = i18n("<p>These files will be analyzed to show the <b>System logs</b>.</p>");

    mFileList = new LogLevelFileList(this, description);

    connect(mFileList, &FileList::fileListChanged, this, &LogModeConfigurationWidget::configurationChanged);

    layout->addWidget(mFileList);
}

bool SystemConfigurationWidget::isValid() const
{
    if (!mFileList->isEmpty()) {
        logDebug() << "System configuration valid";
        return true;
    }

    logDebug() << "System configuration not valid";
    return false;
}

void SystemConfigurationWidget::saveConfig()
{
    logDebug() << "Saving config from System Options...";

    auto *systemConfiguration = Globals::instance().findLogMode(QStringLiteral(SYSTEM_LOG_MODE_ID))->logModeConfiguration<SystemConfiguration *>();
    systemConfiguration->setLogFilesPaths(mFileList->paths());
    systemConfiguration->setLogFilesLevels(mFileList->levels());
}

void SystemConfigurationWidget::readConfig()
{
    auto *systemConfiguration = Globals::instance().findLogMode(QStringLiteral(SYSTEM_LOG_MODE_ID))->logModeConfiguration<SystemConfiguration *>();

    mFileList->removeAllItems();

    mFileList->addPaths(systemConfiguration->logFilesPaths(), systemConfiguration->logFilesLevels());
}

void SystemConfigurationWidget::defaultConfig()
{
    // TODO Find a way to read the configuration per default
    readConfig();
}
