/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien
    nicolas.ternisien@gmail.com

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "generalConfigurationWidget.h"

#include <QButtonGroup>
#include <QCheckBox>
#include <QPushButton>

#include <KLocalizedString>
#include <KMessageWidget>
#include <QIcon>

#include "defaults.h"
#include "globals.h"
#include "ksystemlogConfig.h"
#include "logging.h"

GeneralConfigurationWidget::GeneralConfigurationWidget()
    : QWidget()
{
    setupUi(this);

    mWarningBox = new KMessageWidget(this);
    mWarningBox->setVisible(false);
    mWarningBox->setMessageType(KMessageWidget::Warning);
    mWarningBox->setText(i18n("This mode is unavailable because its log files do not exist."));
    mWarningBox->setCloseButtonVisible(false);
    mWarningBox->setIcon(QIcon::fromTheme(QStringLiteral("dialog-warning")));
    startupModeVerticalLayout->addWidget(mWarningBox);

    startupLogMode->addItem(QIcon::fromTheme(QStringLiteral(NO_MODE_ICON)), i18n("No Log Mode"), QVariant(QLatin1String("")));
    const auto logModes = Globals::instance().logModes();
    for (LogMode *logMode : logModes) {
        // Ignore this special case
        if (logMode->id() == QLatin1String("openLogMode")) {
            continue;
        }

        startupLogMode->addItem(logMode->icon(), logMode->name(), QVariant(logMode->id()));
    }
    connect(startupLogMode, &QComboBox::currentIndexChanged, this, &GeneralConfigurationWidget::configurationChanged);
    connect(maxLines, &QSpinBox::valueChanged, this, &GeneralConfigurationWidget::configurationChanged);

    connect(deleteDuplicatedLines, &QAbstractButton::clicked, this, &GeneralConfigurationWidget::configurationChanged);

    connect(deleteProcessId, &QAbstractButton::clicked, this, &GeneralConfigurationWidget::configurationChanged);

    connect(colorizeLogLines, &QAbstractButton::clicked, this, &GeneralConfigurationWidget::configurationChanged);

    mDateFormatGroup = new QButtonGroup(this);
    mDateFormatGroup->addButton(formatLongDate, Globals::LongFormat);
    mDateFormatGroup->addButton(formatShortDate, Globals::ShortFormat);
    mDateFormatGroup->addButton(formatPreciseDate, Globals::PreciseFormat);

    connect(mDateFormatGroup, &QButtonGroup::buttonClicked, this, &GeneralConfigurationWidget::configurationChanged);

    addDateFormatExample();
}

GeneralConfigurationWidget::~GeneralConfigurationWidget()
{
}

void GeneralConfigurationWidget::addDateFormatExample()
{
    const auto buttons = mDateFormatGroup->buttons();
    for (QAbstractButton *button : buttons) {
        const auto currentButtonFormat = static_cast<Globals::DateFormat>(mDateFormatGroup->id(button));
        const QString formattedDate = Globals::instance().formatDate(currentButtonFormat, QDateTime().currentDateTime());
        button->setText(i18nc("Date format option (date example)", "%1 (%2)", button->text(), formattedDate));
    }
}

void GeneralConfigurationWidget::readConfig()
{
    for (int i = 0, total = startupLogMode->count(); i < total; ++i) {
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
    const auto dateFormat = (QLocale::FormatType)KSystemLogConfig::dateFormat();
    QAbstractButton *selectedButton = mDateFormatGroup->button(dateFormat);
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

    KSystemLogConfig::setDateFormat(mDateFormatGroup->checkedId());
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
        const QVariant modeID = startupLogMode->currentData();
        if (!modeID.isNull()) {
            const QString modeString = modeID.toString();
            LogMode *mode = Globals::instance().findLogMode(modeString);
            if (mode) {
                if (!mode->filesExist()) {
                    logDebug() << "Log files are missing for mode" << mode->name();
                    mWarningBox->setVisible(true);
                } else {
                    logDebug() << "General configuration is valid";
                    mWarningBox->setVisible(false);
                    return true;
                }
            } else {
                // Empty log mode is selected.
                mWarningBox->setVisible(false);
                return true;
            }
        }
    }

    logDebug() << "General configuration is not valid";
    return false;
}
