/***************************************************************************
 *   KApacheLog, a apache log viewer tool                                  *
 *   Copyright (C) 2007 by Nicolas Ternisien                               *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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

#include "xsessionConfigurationWidget.h"

XSessionConfigurationWidget::XSessionConfigurationWidget()
    : LogModeConfigurationWidget(i18n("X Session Log"), QStringLiteral(X_SESSION_MODE_ICON),
                                 i18n("X Session Log"))
{
    setupUi(this);

    mWarningBox = new KMessageWidget(this);
    mWarningBox->setVisible(false);
    mWarningBox->setMessageType(KMessageWidget::Warning);
    mWarningBox->setText(i18n("Log file does not exist. Mode will be unavailable."));
    mWarningBox->setCloseButtonVisible(false);
    mWarningBox->setIcon(QIcon::fromTheme(QStringLiteral("dialog-warning")));

    verticalLayout->insertWidget(0, mWarningBox);

    xsessionUrlRequester->setToolTip(
        i18n("You can type or choose the X Session log file (example: <i>~/.xsession-errors</i>)."));
    xsessionUrlRequester->setWhatsThis(i18n(
                                           "You can type or choose here the X Session log file. This file will be analyzed when you select "
                                           "the <b>X Session log</b> menu. Generally, its name is <i>~/.xsession-errors</i>"));
    xsessionUrlRequester->setMode(KFile::File);
    xsessionUrlRequester->setEnabled(true);

    connect(xsessionUrlRequester, &KUrlRequester::textChanged, this,
            &LogModeConfigurationWidget::configurationChanged);
    connect(ignoreXorgErrors, &QCheckBox::stateChanged, this, &LogModeConfigurationWidget::configurationChanged);

    connect(ignoreXorgErrors, &QAbstractButton::toggled, xorgErrorsDescription, &QWidget::setEnabled);

    mXorgErrorsDescriptionDefined = false;
}

void XSessionConfigurationWidget::saveConfig()
{
    auto *configuration = Globals::instance()
                          .findLogMode(QStringLiteral(X_SESSION_LOG_MODE_ID))
                          ->logModeConfiguration<XSessionConfiguration *>();

    configuration->setXSessionPath(xsessionUrlRequester->url().toLocalFile());
    configuration->setIgnoreXorgErrors(ignoreXorgErrors->isChecked());
}

void XSessionConfigurationWidget::readConfig()
{
    auto *configuration = Globals::instance()
                          .findLogMode(QStringLiteral(X_SESSION_LOG_MODE_ID))
                          ->logModeConfiguration<XSessionConfiguration *>();

    const QString path = configuration->xsessionPath();
    const QFileInfo fileInfo(path);
    mWarningBox->setVisible(!fileInfo.exists());

    xsessionUrlRequester->setUrl(QUrl::fromLocalFile(path));
    ignoreXorgErrors->setChecked(configuration->isIgnoreXorgErrors());

    prepareXorgErrorsDescription();
}

void XSessionConfigurationWidget::defaultConfig()
{
    // TODO Find a way to read the configuration per default
    readConfig();
}

bool XSessionConfigurationWidget::isValid() const
{
    if (!xsessionUrlRequester->url().toLocalFile().isEmpty()) {
        return true;
    }

    return false;
}

void XSessionConfigurationWidget::prepareXorgErrorsDescription()
{
    auto *configuration = Globals::instance()
                          .findLogMode(QStringLiteral(X_SESSION_LOG_MODE_ID))
                          ->logModeConfiguration<XSessionConfiguration *>();

    // Prepare Ignore Xorg Errors description
    if (!mXorgErrorsDescriptionDefined) {
        QString text = xorgErrorsDescription->text();
        text.append(QLatin1String("<ul style='margin-top:0px;margin-bottom:0px'>"));

        const auto xorgErrorKeywords = configuration->xorgErrorKeywords();
        for (const QString &xorgErrorKeyword : xorgErrorKeywords) {
            text.append(i18n("<li><b>%1</b>: ...</li>", xorgErrorKeyword));
        }
        text.append(QLatin1String("</ul>"));
        xorgErrorsDescription->setText(text);

        mXorgErrorsDescriptionDefined = true;
    }
}
