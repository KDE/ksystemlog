/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#pragma once

#include <QDataStream>
#include <QDebug>
#include <QString>

#include "globals.h"

class LogViewColumn
{
public:
    LogViewColumn(const LogViewColumn &column);

    /*
     * TODO For the moment, filtered and grouped parameters are useless : Use them or remove them
     */
    explicit LogViewColumn(const QString &name, bool filtered = true, bool grouped = true);

    virtual ~LogViewColumn();

    QString columnName() const;
    bool isGrouped() const;
    bool isFiltered() const;

    LogViewColumn &operator=(const LogViewColumn &column);

private:
    QString mColumnName;

    bool mFiltered;
    bool mGrouped;
};

QDataStream &operator<<(QDataStream &out, const LogViewColumn &column);
QDebug &operator<<(QDebug &out, const LogViewColumn &column);

