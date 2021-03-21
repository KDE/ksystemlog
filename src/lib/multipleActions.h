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

#include <QList>
#include <QObject>
#include <QString>

#include <KActionMenu>

#include "globals.h"

#include "logModeAction.h"
#include <QIcon>
class QAction;

/**
 * This class is used by LogModeFactory to provide multiple actions
 * and be able to retrieve them when necessary, using actions().
 * This method will return QAction added using addInnerAction().
 */
class MultipleActions : public LogModeAction
{
    Q_OBJECT

public:
    MultipleActions(const QIcon &icon, const QString &text, QObject *parent);

    ~MultipleActions() override;

    QList<QAction *> innerActions() override;

    QAction *actionMenu() override;

    /**
     * This method is not called addAction() to avoid name collision with
     * parent class
     */
    void addInnerAction(QAction *action, bool addToMenu = true, bool addToInnerActionsList = true);

private:
    QList<QAction *> mActions;

    KActionMenu *mAction = nullptr;
};

