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

#pragma once

#include <QAction>
#include <QList>
#include <QObject>

class LogModeAction : public QObject
{
    Q_OBJECT

public:
    enum Category { RootCategory, ServicesCategory, OthersCategory };

    Q_DECLARE_FLAGS(Categories, Category)

    explicit LogModeAction(QObject *parent = nullptr);

    ~LogModeAction() override;

    virtual QList<QAction *> innerActions() = 0;

    virtual QAction *actionMenu() = 0;

    void setCategory(Category category);

    Category category() const;

private:
    LogModeAction::Category mCategory = LogModeAction::RootCategory;
    bool mInToolBar = true;
};

