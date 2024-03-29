/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QString>
#include <QWidget>

class LogModeConfigurationWidget : public QWidget
{
    Q_OBJECT

public:
    LogModeConfigurationWidget(const QString &itemName, const QString &iconName, const QString &header);

    ~LogModeConfigurationWidget() override;

    virtual bool isValid() const;

    virtual void saveConfig() = 0;
    virtual void defaultConfig() = 0;
    virtual void readConfig() = 0;

Q_SIGNALS:
    void configurationChanged();

public:
    QString itemName() const;
    QString iconName() const;
    QString header() const;

private:
    QString mItemName;
    QString mIconName;
    QString mHeader;
};
