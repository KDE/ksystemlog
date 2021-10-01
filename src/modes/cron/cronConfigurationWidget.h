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

#include "logModeConfigurationWidget.h"

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

#include <KLocalizedString>

#include "globals.h"
#include "logging.h"

#include "fileList.h"

#include "logLevel.h"

#include "cronConfiguration.h"
#include "cronLogMode.h"

class CronConfigurationWidget : public LogModeConfigurationWidget
{
    Q_OBJECT

public:
    CronConfigurationWidget();

    ~CronConfigurationWidget() override
    {
    }

    bool isValid() const override;

    void saveConfig() override;

    void readConfig() override;

    void defaultConfig() override;

private Q_SLOTS:
    void toggleProcessFilterEnabling(bool enabled);

private:
    FileList *mFileList = nullptr;

    QGroupBox *mProcessFilterGroup = nullptr;

    QLineEdit *mProcessFilter = nullptr;
    QLabel *mProcessFilterLabel = nullptr;
};

