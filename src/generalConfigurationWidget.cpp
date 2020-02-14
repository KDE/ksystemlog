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

#include <KMessageWidget>
#include <KLocalizedString>
#include <QIcon>

#include "logging.h"
#include "defaults.h"
#include "globals.h"
#include "ksystemlogConfig.h"

class GeneralConfigurationWidgetPrivate
{
public:
    QButtonGroup *dateFormatGroup;
    KMessageWidget *warningBox;
};

GeneralConfigurationWidget::GeneralConfigurationWidget()
    : QWidget()
    , d(new GeneralConfigurationWidgetPrivate())
{
    setupUi(this);

    d->warningBox = new KMessageWidget(this);
    d->warningBox->setVisible(false);
    d->warningBox->setMessageType(KMessageWidget::Warning);
    d->warningBox->setText(i18n("This mode is unavailable because its log files do not exist."));
    d->warningBox->setCloseButtonVisible(false);
    d->warningBox->setIcon(QIcon::fromTheme(QStringLiteral("dialog-warning")));
    startupModeVerticalLayout->addWidget(d->warningBox);

    startupLogMode->addItem(QIcon::fromTheme(QStringLiteral(NO_MODE_ICON)), i18n("No Log Mode"),
                            QVariant(QLatin1String("")));
    foreach (LogMode *logMode, Globals::instance().logModes()) {
        // Ignore this special case
        if (logMode->id() == QLatin1String("openLogMode"))
            continue;

        startupLogMode->addItem(logMode->icon(), logMode->name(), QVariant(logMode->id()));
    }
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    connect(startupLogMode, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GeneralConfigurationWidget::configurationChanged);
#else
    connect(startupLogMode, QOverload<int, const QString &>::of(&QComboBox::currentIndexChanged), this, &GeneralConfigurationWidget::configurationChanged);
#endif
    connect(maxLines, QOverload<int>::of(&QSpinBox::valueChanged), this, &GeneralConfigurationWidget::configurationChanged);

    connect(deleteDuplicatedLines, &QAbstractButton::clicked, this, &GeneralConfigurationWidget::configurationChanged);

    connect(deleteProcessId, &QAbstractButton::clicked, this, &GeneralConfigurationWidget::configurationChanged);

    connect(colorizeLogLines, &QAbstractButton::clicked, this, &GeneralConfigurationWidget::configurationChanged);

    d->dateFormatGroup = new QButtonGroup(this);
    d->dateFormatGroup->addButton(formatLongDate, Globals::LongFormat);
    d->dateFormatGroup->addButton(formatShortDate, Globals::ShortFormat);
    d->dateFormatGroup->addButton(formatPreciseDate, Globals::PreciseFormat);

    connect(d->dateFormatGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this, &GeneralConfigurationWidget::configurationChanged);

    addDateFormatExample();
}

GeneralConfigurationWidget::~GeneralConfigurationWidget()
{
    // dateFormatGroup is automatically deleted by Qt

    delete d;
}

void GeneralConfigurationWidget::addDateFormatExample()
{
    foreach (QAbstractButton *button, d->dateFormatGroup->buttons()) {
        Globals::DateFormat currentButtonFormat = (Globals::DateFormat)d->dateFormatGroup->id(button);
        QString formattedDate = Globals::instance().formatDate(currentButtonFormat, QDateTime().currentDateTime());
        button->setText(i18nc("Date format option (date example)", "%1 (%2)", button->text(), formattedDate));
    }
}

void GeneralConfigurationWidget::readConfig()
{
    for (int i = 0; i < startupLogMode->count(); ++i) {
        if (KSystemLogConfig::startupLogMode() == startupLogMode->itemData(i)) {
            startupLogMode->setCurrentIndex(i);
            break;
        }
    }

    maxLines->setValue(KSystemLogConfig::maxLines());

    deleteDuplicatedLines->setChecked(KSystemLogConfig::deleteDuplicatedLines());

    deleteProcessId->setChecked(KSystemLogConfig::deleteProcessIdentifier());

    colorizeLogLines->setChecked(KSystemLogConfig::colorizeLogLines());

    // KLocale::DateFormat dateFormat = (KLocale::DateFormat) KSystemLogConfig::dateFormat();
    QLocale::FormatType dateFormat = (QLocale::FormatType)KSystemLogConfig::dateFormat();
    QAbstractButton *selectedButton = d->dateFormatGroup->button(dateFormat);
    selectedButton->setChecked(true);
}

void GeneralConfigurationWidget::saveConfig() const
{
    logDebug() << "Save config from General preferences";

    KSystemLogConfig::setStartupLogMode(startupLogMode->itemData(startupLogMode->currentIndex()).toString());

    KSystemLogConfig::setMaxLines(maxLines->value());
    KSystemLogConfig::setDeleteDuplicatedLines(deleteDuplicatedLines->isChecked());
    KSystemLogConfig::setDeleteProcessIdentifier(deleteProcessId->isChecked());
    KSystemLogConfig::setColorizeLogLines(colorizeLogLines->isChecked());

    KSystemLogConfig::setDateFormat(d->dateFormatGroup->checkedId());
}

void GeneralConfigurationWidget::defaultConfig()
{
    // TODO Find a way to read the configuration per default
    readConfig();
}

bool GeneralConfigurationWidget::isValid() const
{
    if (maxLines->value() > 0) {
        // Check if log files exist for selected mode.
        QVariant modeID = startupLogMode->currentData();
        if (!modeID.isNull()) {
            QString modeString = modeID.toString();
            LogMode *mode = Globals::instance().findLogMode(modeString);
            if (mode) {
                if (!mode->filesExist()) {
                    logDebug() << "Log files are missing for mode" << mode->name();
                    d->warningBox->setVisible(true);
                } else {
                    logDebug() << "General configuration is valid";
                    d->warningBox->setVisible(false);
                    return true;
                }
            } else {
                // Empty log mode is selected.
                d->warningBox->setVisible(false);
                return true;
            }
        }
    }

    logDebug() << "General configuration is not valid";
    return false;
}
