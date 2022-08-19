/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QDataStream>
#include <QDebug>
#include <QObject>
#include <QUrl>

class LogLevel;

class LogFile
{
public:
    LogFile();

    LogFile(const LogFile &logFile);
    LogFile(const QUrl &url, LogLevel *defaultLogLevel);

    ~LogFile();

    bool operator==(const LogFile &other) const;

    LogFile &operator=(const LogFile &column);

    QUrl url() const;

    LogLevel *defaultLogLevel() const;

private:
    QUrl mUrl;

    LogLevel *mDefaultLogLevel = nullptr;
};

QDataStream &operator<<(QDataStream &out, const LogFile &column);
QDebug &operator<<(QDebug &out, const LogFile &column);
