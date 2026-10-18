/*
    SPDX-FileCopyrightText: 2026 shitamo <sitamo@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "syslogDateParser.h"

#include <QLocale>
#include <QRegularExpression>
#include <QTimeZone>

namespace
{
// "Mmm dd HH:MM:SS " : 15 characters for the date, plus the separator
constexpr qsizetype TraditionalDateLength = 15;

int parseMonth(QStringView month)
{
    static const char *const months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    for (int i = 0; i < 12; ++i) {
        if (month == QLatin1String(months[i])) {
            return i + 1;
        }
    }

    // Same fallback as ParsingHelper::parseSyslogMonth()
    return 1;
}

qsizetype skipSpaces(QStringView line, qsizetype pos)
{
    while (pos < line.size() && line.at(pos) == QLatin1Char(' ')) {
        ++pos;
    }
    return pos;
}

const QRegularExpression &iso8601Regex()
{
    // 1: year, 2: month, 3: day, 4: hour, 5: minute, 6: second, 7: fraction, 8: time zone
    static const QRegularExpression regex(QStringLiteral(R"(^(\d{4})-(\d{2})-(\d{2})[T ](\d{2}):(\d{2}):(\d{2})(?:[.,](\d+))?(Z|z|[+-]\d{2}(?::?\d{2})?)?(?=\s|$))"));
    return regex;
}
}

namespace SyslogDateParser
{
bool startsWithIso8601Date(QStringView line)
{
    if (line.size() < 10) {
        return false;
    }

    for (int i : {0, 1, 2, 3, 5, 6, 8, 9}) {
        if (!line.at(i).isDigit()) {
            return false;
        }
    }

    return line.at(4) == QLatin1Char('-') && line.at(7) == QLatin1Char('-');
}

Result parseTraditional(QStringView line, int year)
{
    Result result;

    if (line.size() < TraditionalDateLength) {
        return result;
    }

    const int month = parseMonth(line.mid(0, 3));
    const int day = line.mid(4, 2).trimmed().toInt();
    const int hour = line.mid(7, 2).toInt();
    const int minute = line.mid(10, 2).toInt();
    const int second = line.mid(13, 2).toInt();

    result.dateTime = QDateTime(QDate(year, month, day), QTime(hour, minute, second));
    if (result.dateTime.isValid()) {
        result.consumed = qMin(line.size(), TraditionalDateLength + 1);
    }

    return result;
}

Result parseIso8601(QStringView line)
{
    Result result;

    const QRegularExpressionMatch match = iso8601Regex().matchView(line);
    if (!match.hasMatch()) {
        return result;
    }

    const QDate date(match.capturedView(1).toInt(), match.capturedView(2).toInt(), match.capturedView(3).toInt());

    int msec = 0;
    const QStringView fraction = match.capturedView(7);
    if (!fraction.isEmpty()) {
        // Keep milliseconds only ("123456" -> 123, "5" -> 500)
        QString millis = fraction.left(3).toString();
        while (millis.size() < 3) {
            millis.append(QLatin1Char('0'));
        }
        msec = millis.toInt();
    }

    const QTime time(match.capturedView(4).toInt(), match.capturedView(5).toInt(), match.capturedView(6).toInt(), msec);

    const QStringView zone = match.capturedView(8);
    QDateTime dateTime;
    if (zone.isEmpty()) {
        dateTime = QDateTime(date, time);
    } else {
        int offset = 0;
        if (zone != QLatin1String("Z") && zone != QLatin1String("z")) {
            const int sign = zone.at(0) == QLatin1Char('-') ? -1 : 1;
            const int hours = zone.mid(1, 2).toInt();
            QStringView rest = zone.mid(3);
            if (rest.startsWith(QLatin1Char(':'))) {
                rest = rest.mid(1);
            }
            const int minutes = rest.isEmpty() ? 0 : rest.toInt();
            offset = sign * (hours * 3600 + minutes * 60);
        }
        dateTime = QDateTime(date, time, QTimeZone(offset)).toLocalTime();
    }

    if (dateTime.isValid()) {
        result.dateTime = dateTime;
        result.consumed = skipSpaces(line, match.capturedEnd(0));
    }

    return result;
}

Result parseCustom(QStringView line, const QString &customFormat, int defaultYear)
{
    Result result;

    const QString format = customFormat.simplified();
    if (format.isEmpty()) {
        return result;
    }

    // The timestamp is made of as many space separated words as the format
    const int wordCount = format.count(QLatin1Char(' ')) + 1;

    qsizetype pos = skipSpaces(line, 0);
    QStringList words;
    while (words.size() < wordCount && pos < line.size()) {
        qsizetype end = line.indexOf(QLatin1Char(' '), pos);
        if (end == -1) {
            end = line.size();
        }
        words.append(line.mid(pos, end - pos).toString());
        pos = skipSpaces(line, end);
    }

    if (words.size() < wordCount) {
        return result;
    }

    // Always parse English month and day names, as syslog daemons do not localize them
    QDateTime dateTime = QLocale::c().toDateTime(words.join(QLatin1Char(' ')), format);
    if (!dateTime.isValid()) {
        return result;
    }

    if (!format.contains(QLatin1Char('y'))) {
        dateTime.setDate(QDate(defaultYear, dateTime.date().month(), dateTime.date().day()));
    }

    if (dateTime.timeSpec() != Qt::LocalTime) {
        dateTime = dateTime.toLocalTime();
    }

    if (dateTime.isValid()) {
        result.dateTime = dateTime;
        result.consumed = pos;
    }

    return result;
}

Result parse(QStringView line, Format format, const QString &customFormat, int defaultYear)
{
    switch (format) {
    case Traditional:
        return parseTraditional(line, defaultYear);
    case Iso8601:
        return parseIso8601(line);
    case Custom:
        return parseCustom(line, customFormat, defaultYear);
    case Automatic:
    default:
        if (startsWithIso8601Date(line)) {
            return parseIso8601(line);
        }
        return parseTraditional(line, defaultYear);
    }
}
}
