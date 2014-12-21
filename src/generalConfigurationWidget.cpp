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

#include "generalConfigurationWidget.h"


#include <QCheckBox>
#include <QPushButton>
#include <QButtonGroup>

#include <kglobal.h>
#include <KLocalizedString>
#include <QIcon>
#include <kstandarddirs.h>

#include "logging.h"
#include "defaults.h"
#include "globals.h"
#include "ksystemlogConfig.h"

class GeneralConfigurationWidgetPrivate {
public:
	QButtonGroup* dateFormatGroup;
};

GeneralConfigurationWidget::GeneralConfigurationWidget() :
	QWidget(),
	d(new GeneralConfigurationWidgetPrivate())
	{

	setupUi(this);

	startupLogMode->addItem(QIcon::fromTheme( QLatin1String( NO_MODE_ICON) ), i18n("No Log Mode"), QVariant(QLatin1String( "" ) ));
	foreach(LogMode* logMode, Globals::instance()->logModes()) {
		//Ignore this special case
		if (logMode->id() == QLatin1String( "openLogMode" ))
			continue;

		startupLogMode->addItem(QIcon(logMode->icon()), logMode->name(), QVariant(logMode->id()));
	}

	connect(startupLogMode, SIGNAL(currentIndexChanged(int)), this, SIGNAL(configurationChanged()));

	connect(maxLines, SIGNAL(valueChanged(int)), this, SIGNAL(configurationChanged()));

	connect(deleteDuplicatedLines, SIGNAL(clicked()), this, SIGNAL(configurationChanged()));

	connect(deleteProcessId, SIGNAL(clicked()), this, SIGNAL(configurationChanged()));

	connect(colorizeLogLines, SIGNAL(clicked()), this, SIGNAL(configurationChanged()));

	d->dateFormatGroup = new QButtonGroup(this);
	d->dateFormatGroup->addButton(formatShortDate, KLocale::ShortDate);
	d->dateFormatGroup->addButton(formatLongDate, KLocale::LongDate);
	d->dateFormatGroup->addButton(formatFancyShortDate, KLocale::FancyShortDate);
	d->dateFormatGroup->addButton(formatFancyLongDate, KLocale::FancyLongDate);

	connect(d->dateFormatGroup, SIGNAL(buttonClicked(int)), this, SIGNAL(configurationChanged()));

	addDateFormatExample();
}


GeneralConfigurationWidget::~GeneralConfigurationWidget() {
	//dateFormatGroup is automatically deleted by Qt

	delete d;
}

void GeneralConfigurationWidget::addDateFormatExample() {
	foreach(QAbstractButton* button, d->dateFormatGroup->buttons()) {
		QDateTime currentDateTime(QDateTime::currentDateTime());

		KLocale::DateFormat currentButtonFormat = (KLocale::DateFormat) d->dateFormatGroup->id(button);

		QString formattedDate = KLocale::global()->formatDateTime(currentDateTime, currentButtonFormat, true);

		button->setText( i18nc("Date format Option (Date example)", "%1 (%2)", button->text(), formattedDate) );
	}
}

void GeneralConfigurationWidget::readConfig() {
	for (int i=0; i<startupLogMode->count(); ++i) {
		if (KSystemLogConfig::startupLogMode() == startupLogMode->itemData(i)) {
			startupLogMode->setCurrentIndex(i);
			break;
		}
	}

	maxLines->setValue(KSystemLogConfig::maxLines());

	deleteDuplicatedLines->setChecked(KSystemLogConfig::deleteDuplicatedLines());

	deleteProcessId->setChecked(KSystemLogConfig::deleteProcessIdentifier());

	colorizeLogLines->setChecked(KSystemLogConfig::colorizeLogLines());

	KLocale::DateFormat dateFormat = (KLocale::DateFormat) KSystemLogConfig::dateFormat();
	QAbstractButton* selectedButton = d->dateFormatGroup->button(dateFormat);
	selectedButton->setChecked(true);
}

void GeneralConfigurationWidget::saveConfig() const {
	logDebug() << "Save config from General preferences" << endl;

	KSystemLogConfig::setStartupLogMode(startupLogMode->itemData(startupLogMode->currentIndex()).toString());

	KSystemLogConfig::setMaxLines(maxLines->value());
	KSystemLogConfig::setDeleteDuplicatedLines(deleteDuplicatedLines->isChecked());
	KSystemLogConfig::setDeleteProcessIdentifier(deleteProcessId->isChecked());
	KSystemLogConfig::setColorizeLogLines(colorizeLogLines->isChecked());

	KSystemLogConfig::setDateFormat(d->dateFormatGroup->checkedId());

}

void GeneralConfigurationWidget::defaultConfig() {
	//TODO Find a way to read the configuration per default
	readConfig();
}

bool GeneralConfigurationWidget::isValid() const {
	if (maxLines->value()>0) {
		logDebug() << "General configuration valid" << endl;
		return true;
	}

	logDebug() << "General configuration not valid" << endl;
	return false;
}


