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

#ifndef LOG_MODE_CONFIGURATION_WIDGET_H
#define LOG_MODE_CONFIGURATION_WIDGET_H

#include <QWidget>
#include <QString>

class LogModeConfigurationWidgetPrivate;

class LogModeConfigurationWidget : public QWidget
{
    Q_OBJECT

public:
    LogModeConfigurationWidget(const QString &itemName, const QString &iconName, const QString &header);

    virtual ~LogModeConfigurationWidget();

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
    LogModeConfigurationWidgetPrivate *const d;
};

#endif // _LOG_MODE_CONFIGURATION_WIDGET_H
