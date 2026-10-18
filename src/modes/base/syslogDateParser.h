/*
    SPDX-FileCopyrightText: 2026 shitamo <sitamo@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QDateTime>
#include <QString>
#include <QStringView>

/**
 * Parses the timestamp found at the beginning of a syslog-style log line.
 *
 * Supported formats:
 *  - Traditional (RFC 3164 / BSD syslog): "Oct  9 23:15:00"
 *  - ISO 8601 / RFC 3339 (rsyslog RSYSLOG_FileFormat, syslog-ng ISO, ...):
 *      "2026-10-09T23:15:00.123456+09:00", "2026-10-09T14:15:00Z",
 *      "2026-10-09T23:15:00+0900", "2026-10-09 23:15:00"
 *  - Custom: a QDateTime format string such as "yyyy/MM/dd HH:mm:ss"
 *  - Automatic: ISO 8601 when the line starts like one, traditional otherwise
 */
namespace SyslogDateParser
{
/**
 * The order must match the choices of the "SyslogDateFormat" entry in ksystemlog.kcfg
 */
enum Format {
    Automatic = 0,
    Traditional = 1,
    Iso8601 = 2,
    Custom = 3,
};

struct Result {
    /// The parsed date, converted to local time. Invalid when parsing failed.
    QDateTime dateTime;
    /// Number of characters of the line used by the timestamp and the separator following it.
    qsizetype consumed = 0;

    [[nodiscard]] bool isValid() const
    {
        return dateTime.isValid();
    }
};

/**
 * @param line the whole log line
 * @param format the format to use
 * @param customFormat QDateTime format string, only used with the Custom format
 * @param defaultYear year used when the timestamp does not contain one
 */
[[nodiscard]] Result parse(QStringView line, Format format, const QString &customFormat, int defaultYear);

[[nodiscard]] Result parseTraditional(QStringView line, int year);
[[nodiscard]] Result parseIso8601(QStringView line);
[[nodiscard]] Result parseCustom(QStringView line, const QString &customFormat, int defaultYear);

/**
 * @return true if the line starts with an ISO 8601 date ("YYYY-MM-DD")
 */
[[nodiscard]] bool startsWithIso8601Date(QStringView line);
}
