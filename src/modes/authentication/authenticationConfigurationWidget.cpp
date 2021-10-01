/***************************************************************************
 *   KApacheLog, a apache log viewer tool                                  *
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

#include "authenticationConfigurationWidget.h"

AuthenticationConfigurationWidget::AuthenticationConfigurationWidget()
    : LogModeConfigurationWidget(i18n("Authentication Log"), QStringLiteral(AUTHENTICATION_MODE_ICON), i18n("Authentication Log"))
{
    auto layout = new QVBoxLayout(this);

    mWarningBox = new KMessageWidget(this);
    mWarningBox->setVisible(false);
    mWarningBox->setMessageType(KMessageWidget::Warning);
    mWarningBox->setText(i18n("Log file does not exist. Mode will be unavailable."));
    mWarningBox->setCloseButtonVisible(false);
    mWarningBox->setIcon(QIcon::fromTheme(QStringLiteral("dialog-warning")));

    // Authentication log file
    auto authenticationBox = new QGroupBox(i18n("Authentication Log File"));
    auto authenticationLayout = new QVBoxLayout();
    auto filePathLayout = new QHBoxLayout();
    authenticationBox->setLayout(authenticationLayout);

    authenticationLayout->addWidget(mWarningBox);
    authenticationLayout->addLayout(filePathLayout);

    layout->addWidget(authenticationBox);

    filePathLayout->addWidget(new QLabel(i18n("Authentication log file:")));

    mAuthenticationUrlRequester = new KUrlRequester(authenticationBox);
    mAuthenticationUrlRequester->setMode(KFile::File);

    mAuthenticationUrlRequester->setToolTip(i18n("You can type or choose the authentication log file (example: <i>/var/log/auth.log</i>)."));
    mAuthenticationUrlRequester->setWhatsThis(
        i18n("You can type or choose here the authentication log file. This file will be analyzed when you "
             "select the <b>Authentication log</b> menu. Generally, its name is <i>/var/log/auth.log</i>"));
    filePathLayout->addWidget(mAuthenticationUrlRequester);

    connect(mAuthenticationUrlRequester, &KUrlRequester::textChanged, this, &LogModeConfigurationWidget::configurationChanged);

    layout->addStretch();
}

void AuthenticationConfigurationWidget::saveConfig()
{
    auto *authenticationConfiguration =
        Globals::instance().findLogMode(QStringLiteral(AUTHENTICATION_LOG_MODE_ID))->logModeConfiguration<AuthenticationConfiguration *>();

    authenticationConfiguration->setAuthenticationPath(mAuthenticationUrlRequester->url().toLocalFile());
}

void AuthenticationConfigurationWidget::readConfig()
{
    auto *authenticationConfiguration =
        Globals::instance().findLogMode(QStringLiteral(AUTHENTICATION_LOG_MODE_ID))->logModeConfiguration<AuthenticationConfiguration *>();

    const QString path = authenticationConfiguration->authenticationPath();
    const QFileInfo fileInfo(path);
    mWarningBox->setVisible(!fileInfo.exists());

    mAuthenticationUrlRequester->setUrl(QUrl::fromLocalFile(path));
}

void AuthenticationConfigurationWidget::defaultConfig()
{
    // TODO Find a way to read the configuration per default
    readConfig();
}

bool AuthenticationConfigurationWidget::isValid() const
{
    if (!mAuthenticationUrlRequester->url().toLocalFile().isEmpty()) {
        return true;
    }

    return false;
}
