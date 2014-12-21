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


class XSessionConfigurationWidget : public LogModeConfigurationWidget, public Ui::XSessionConfigurationWidgetBase {

	Q_OBJECT

	public:
		XSessionConfigurationWidget() :
			LogModeConfigurationWidget(i18n("X Session Log"), QLatin1String(  X_SESSION_MODE_ICON ), i18n("X Session Log"))
			{

			setupUi(this);
			/*

			QVBoxLayout* layout = new QVBoxLayout();
			this->setLayout(layout);

			//Authentication log file
			QGroupBox* xsessionBox=new QGroupBox(i18n("X Session Log File"));
			QGridLayout* xsessionLayout = new QGridLayout();
			xsessionBox->setLayout(xsessionLayout);

			layout->addWidget(xsessionBox);

			xsessionLayout->addWidget(new QLabel(i18n("Log file:")), 0, 0);

			xsessionUrlRequester=new KUrlRequester(xsessionBox);

			xsessionLayout->addWidget(xsessionUrlRequester, 0, 1);

			ignoreXorgErrors = new QCheckBox(i18n("Ignore Xorg Errors"), this);
			xsessionLayout->addWidget(xsessionUrlRequester, 1, 0, 1, 2);

			layout->addStretch();
			*/

			xsessionUrlRequester->setToolTip(i18n("You can type or choose the X Session log file (example: <i>~/.xsession-errors</i>)."));
			xsessionUrlRequester->setWhatsThis(i18n("You can type or choose here the X Session log file. This file will be analyzed when you select the <b>X Session log</b> menu. Generally, its name is <i>~/.xsession-errors</i>"));
			xsessionUrlRequester->setMode(KFile::File);
			xsessionUrlRequester->setEnabled(true);

			connect(xsessionUrlRequester, SIGNAL(textChanged(const QString&)), this, SIGNAL(configurationChanged()));
			connect(ignoreXorgErrors, SIGNAL(stateChanged(int)), this, SIGNAL(configurationChanged()));

			connect(ignoreXorgErrors, SIGNAL(toggled(bool)), xorgErrorsDescription, SLOT(setEnabled(bool)));

			xorgErrorsDescriptionDefined = false;

		}

		~XSessionConfigurationWidget() {

		}

	public slots:

		void saveConfig() {
			XSessionConfiguration* configuration = Globals::instance()->findLogMode(QLatin1String( X_SESSION_LOG_MODE_ID ))->logModeConfiguration<XSessionConfiguration*>();

			configuration->setXSessionPath(xsessionUrlRequester->url().path());
			configuration->setIgnoreXorgErrors(ignoreXorgErrors->isChecked());
		}

		void readConfig() {
			XSessionConfiguration* configuration = Globals::instance()->findLogMode(QLatin1String( X_SESSION_LOG_MODE_ID ))->logModeConfiguration<XSessionConfiguration*>();

			xsessionUrlRequester->setUrl(KUrl(configuration->xsessionPath()));
			ignoreXorgErrors->setChecked(configuration->isIgnoreXorgErrors());

			prepareXorgErrorsDescription();
		}

		void defaultConfig() {
			//TODO Find a way to read the configuration per default
			readConfig();
		}

	protected:
		bool isValid() const {
			if (xsessionUrlRequester->url().path().isEmpty()==false) {
				return true;
			}

			return false;

		}

	private:
		void prepareXorgErrorsDescription() {
			XSessionConfiguration* configuration = Globals::instance()->findLogMode(QLatin1String( X_SESSION_LOG_MODE_ID ))->logModeConfiguration<XSessionConfiguration*>();

			//Prepare Ignore Xorg Errors description
			if (xorgErrorsDescriptionDefined == false) {
				QString text = xorgErrorsDescription->text();
				text.append(QLatin1String( "<ul style='margin-top:0px;margin-bottom:0px'>" ));

				foreach(const QString &xorgErrorKeyword, configuration->xorgErrorKeywords()) {
					text.append(i18n("<li><b>%1</b>: ...</li>", xorgErrorKeyword));
				}
				text.append(QLatin1String( "</ul>" ));
				xorgErrorsDescription->setText(text);

				xorgErrorsDescriptionDefined = true;
			}

		}

		bool xorgErrorsDescriptionDefined;
};

#endif // _X_SESSION_CONFIGURATION_WIDGET_H_
