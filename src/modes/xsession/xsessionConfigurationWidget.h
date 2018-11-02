/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
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

#ifndef _X_SESSION_CONFIGURATION_WIDGET_H_
#define _X_SESSION_CONFIGURATION_WIDGET_H_

#include <QGroupBox>
#include <QCheckBox>

#include <KLocalizedString>
#include <kurlrequester.h>

#include "globals.h"
#include "logging.h"
#include "fileList.h"

#include "logLevel.h"

#include "xsessionConfiguration.h"
#include "xsessionLogMode.h"

#include "logModeConfigurationWidget.h"

#include "ui_xsessionConfigurationWidgetBase.h"

class XSessionConfigurationWidget : public LogModeConfigurationWidget,
                                    public Ui::XSessionConfigurationWidgetBase
{
    Q_OBJECT

public:
    XSessionConfigurationWidget()
        : LogModeConfigurationWidget(i18n("X Session Log"), QStringLiteral(X_SESSION_MODE_ICON),
                                     i18n("X Session Log"))
    {
        setupUi(this);

        warningBox = new KMessageWidget(this);
        warningBox->setVisible(false);
        warningBox->setMessageType(KMessageWidget::Warning);
        warningBox->setText(i18n("Log file does not exist. Mode will be unavailable."));
        warningBox->setCloseButtonVisible(false);
        warningBox->setIcon(QIcon::fromTheme(QStringLiteral("dialog-warning")));

        verticalLayout->insertWidget(0, warningBox);

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

        xorgErrorsDescriptionDefined = false;
    }

    ~XSessionConfigurationWidget() {}

public Q_SLOTS:

    void saveConfig() Q_DECL_OVERRIDE
    {
        XSessionConfiguration *configuration = Globals::instance()
                                                   .findLogMode(QStringLiteral(X_SESSION_LOG_MODE_ID))
                                                   ->logModeConfiguration<XSessionConfiguration *>();

        configuration->setXSessionPath(xsessionUrlRequester->url().path());
        configuration->setIgnoreXorgErrors(ignoreXorgErrors->isChecked());
    }

    void readConfig() Q_DECL_OVERRIDE
    {
        XSessionConfiguration *configuration = Globals::instance()
                                                   .findLogMode(QStringLiteral(X_SESSION_LOG_MODE_ID))
                                                   ->logModeConfiguration<XSessionConfiguration *>();

        QString path = configuration->xsessionPath();
        QFileInfo fileInfo(path);
        warningBox->setVisible(!fileInfo.exists());

        xsessionUrlRequester->setUrl(QUrl::fromLocalFile(path));
        ignoreXorgErrors->setChecked(configuration->isIgnoreXorgErrors());

        prepareXorgErrorsDescription();
    }

    void defaultConfig() Q_DECL_OVERRIDE
    {
        // TODO Find a way to read the configuration per default
        readConfig();
    }

protected:
    bool isValid() const Q_DECL_OVERRIDE
    {
        if (xsessionUrlRequester->url().path().isEmpty() == false) {
            return true;
        }

        return false;
    }

private:
    void prepareXorgErrorsDescription()
    {
        XSessionConfiguration *configuration = Globals::instance()
                                                   .findLogMode(QStringLiteral(X_SESSION_LOG_MODE_ID))
                                                   ->logModeConfiguration<XSessionConfiguration *>();

        // Prepare Ignore Xorg Errors description
        if (xorgErrorsDescriptionDefined == false) {
            QString text = xorgErrorsDescription->text();
            text.append(QLatin1String("<ul style='margin-top:0px;margin-bottom:0px'>"));

            foreach (const QString &xorgErrorKeyword, configuration->xorgErrorKeywords()) {
                text.append(i18n("<li><b>%1</b>: ...</li>", xorgErrorKeyword));
            }
            text.append(QLatin1String("</ul>"));
            xorgErrorsDescription->setText(text);

            xorgErrorsDescriptionDefined = true;
        }
    }

    bool xorgErrorsDescriptionDefined;

    KMessageWidget *warningBox;
};

#endif // _X_SESSION_CONFIGURATION_WIDGET_H_
