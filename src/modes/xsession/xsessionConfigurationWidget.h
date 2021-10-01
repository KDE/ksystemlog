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

#include <QCheckBox>

#include <KLocalizedString>
#include <KUrlRequester>

#include "fileList.h"
#include "globals.h"
#include "logging.h"

#include "logLevel.h"

#include "xsessionConfiguration.h"
#include "xsessionLogMode.h"

#include "logModeConfigurationWidget.h"

#include "ui_xsessionConfigurationWidgetBase.h"

class XSessionConfigurationWidget : public LogModeConfigurationWidget, public Ui::XSessionConfigurationWidgetBase
{
    Q_OBJECT

public:
    XSessionConfigurationWidget();

    ~XSessionConfigurationWidget() override
    {
    }

public Q_SLOTS:

    void saveConfig() override;

    void readConfig() override;

    void defaultConfig() override;

protected:
    bool isValid() const override;

private:
    void prepareXorgErrorsDescription();

    bool mXorgErrorsDescriptionDefined;

    KMessageWidget *mWarningBox = nullptr;
};

