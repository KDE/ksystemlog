/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "logLevel.h"

LogLevel::LogLevel(int id, const QString &nm, const QString &ic, const QColor &col, QObject *parent)
    : QObject(parent)
    , mId(id)
    , mName(nm)
    , mColor(col)
    , mIcon(QIcon::fromTheme(ic))
{
}

LogLevel::~LogLevel()
{
}

int LogLevel::id() const
{
    return mId;
}

QString LogLevel::name() const
{
    return mName;
}

QColor LogLevel::color() const
{
    return mColor;
}

QIcon LogLevel::icon() const
{
    return mIcon;
}
