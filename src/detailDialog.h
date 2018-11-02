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

#ifndef _DETAIL_DIALOG_H_
#define _DETAIL_DIALOG_H_

#include <QDialog>

class LogViewWidget;

#include "ui_detailDialogBase.h"

class DetailDialog : public QDialog, public Ui::DetailDialogBase
{
    Q_OBJECT

public:
    explicit DetailDialog(QWidget *parent);

    ~DetailDialog();

public Q_SLOTS:
    void selectionChanged(LogViewWidget *logViewWidget);

private Q_SLOTS:
    void previousItem();
    void nextItem();

private:
    void updateDetails();

    /**
     * Method that replaces the bugged itemAbove() and itemBelow() methods
     */
    void moveToItem(int direction);

    LogViewWidget *logViewWidget;
};

#endif //_DETAIL_DIALOG_H_
