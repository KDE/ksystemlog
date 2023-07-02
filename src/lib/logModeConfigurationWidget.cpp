/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "logModeConfigurationWidget.h"

LogModeConfigurationWidget::LogModeConfigurationWidget(const QString &itemName, const QString &iconName, const QString &header)
    : QWidget()
    , mItemName(itemName)
    , mIconName(iconName)
    , mHeader(header)
{
}

LogModeConfigurationWidget::~LogModeConfigurationWidget()
{
}

QString LogModeConfigurationWidget::itemName() const
{
    return mItemName;
}

QString LogModeConfigurationWidget::iconName() const
{
    return mIconName;
}

QString LogModeConfigurationWidget::header() const
{
    return mHeader;
}

/**
 * Default implementation
 */
bool LogModeConfigurationWidget::isValid() const
{
    return true;
}

#include "moc_logModeConfigurationWidget.cpp"
