/*
    SPDX-FileCopyrightText: 2026 shitamo <sitamo@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include <QTest>
#include <QTimeZone>

#include "syslogDateParser.h"

class SyslogDateParserTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testTraditional();
    void testTraditionalSingleDigitDay();
    void testTraditionalTooShort();
    void testIso8601_data();
    void testIso8601();
    void testIso8601Invalid();
    void testAutomatic();
    void testCustom();
    void testCustomWithoutYear();
    void testCustomInvalid();
};

void SyslogDateParserTest::testTraditional()
{
    const QString line = QStringLiteral("Aug 18 17:04:28 localhost kernel: message");
    const auto result = SyslogDateParser::parse(line, SyslogDateParser::Traditional, QString(), 2007);
    QVERIFY(result.isValid());
    QCOMPARE(result.dateTime, QDateTime(QDate(2007, 8, 18), QTime(17, 4, 28)));
    QCOMPARE(line.mid(result.consumed), QStringLiteral("localhost kernel: message"));
}

void SyslogDateParserTest::testTraditionalSingleDigitDay()
{
    const QString line = QStringLiteral("Oct  9 23:15:00 jupiter systemd[1]: Started.");
    const auto result = SyslogDateParser::parse(line, SyslogDateParser::Traditional, QString(), 2026);
    QVERIFY(result.isValid());
    QCOMPARE(result.dateTime, QDateTime(QDate(2026, 10, 9), QTime(23, 15, 0)));
    QCOMPARE(line.mid(result.consumed), QStringLiteral("jupiter systemd[1]: Started."));
}

void SyslogDateParserTest::testTraditionalTooShort()
{
    QVERIFY(!SyslogDateParser::parse(QStringLiteral("blablalbla"), SyslogDateParser::Traditional, QString(), 2026).isValid());
    QVERIFY(!SyslogDateParser::parse(QString(), SyslogDateParser::Automatic, QString(), 2026).isValid());
}

void SyslogDateParserTest::testIso8601_data()
{
    QTest::addColumn<QString>("line");
    QTest::addColumn<QDateTime>("expected");

    const QDate date(2026, 10, 9);

    QTest::newRow("rsyslog RFC 3339 with microseconds") << QStringLiteral("2026-10-09T23:15:00.123456+09:00 jupiter systemd[1]: Started.")
                                                        << QDateTime(date, QTime(23, 15, 0, 123), QTimeZone(9 * 3600)).toLocalTime();
    QTest::newRow("UTC") << QStringLiteral("2026-10-09T14:15:00Z jupiter systemd[1]: Started.")
                         << QDateTime(date, QTime(14, 15, 0), QTimeZone(0)).toLocalTime();
    QTest::newRow("negative offset without colon") << QStringLiteral("2026-10-09T09:15:00.5-0500 jupiter systemd[1]: Started.")
                                                   << QDateTime(date, QTime(9, 15, 0, 500), QTimeZone(-5 * 3600)).toLocalTime();
    QTest::newRow("no time zone") << QStringLiteral("2026-10-09T23:15:00 jupiter systemd[1]: Started.") << QDateTime(date, QTime(23, 15, 0));
    QTest::newRow("space separator") << QStringLiteral("2026-10-09 23:15:00 jupiter systemd[1]: Started.") << QDateTime(date, QTime(23, 15, 0));
}

void SyslogDateParserTest::testIso8601()
{
    QFETCH(QString, line);
    QFETCH(QDateTime, expected);

    const auto result = SyslogDateParser::parse(line, SyslogDateParser::Iso8601, QString(), 2000);
    QVERIFY(result.isValid());
    QCOMPARE(result.dateTime, expected);
    QCOMPARE(line.mid(result.consumed), QStringLiteral("jupiter systemd[1]: Started."));
}

void SyslogDateParserTest::testIso8601Invalid()
{
    QVERIFY(!SyslogDateParser::parse(QStringLiteral("Oct  9 23:15:00 jupiter a: b"), SyslogDateParser::Iso8601, QString(), 2026).isValid());
    QVERIFY(!SyslogDateParser::parse(QStringLiteral("2026-13-09T23:15:00 jupiter a: b"), SyslogDateParser::Iso8601, QString(), 2026).isValid());
    QVERIFY(!SyslogDateParser::parse(QStringLiteral("2026-10-09T23:15:00junk a: b"), SyslogDateParser::Iso8601, QString(), 2026).isValid());
}

void SyslogDateParserTest::testAutomatic()
{
    auto result = SyslogDateParser::parse(QStringLiteral("2026-10-09T23:15:00+09:00 jupiter a: b"), SyslogDateParser::Automatic, QString(), 2000);
    QVERIFY(result.isValid());
    QCOMPARE(result.dateTime, QDateTime(QDate(2026, 10, 9), QTime(23, 15, 0), QTimeZone(9 * 3600)).toLocalTime());

    result = SyslogDateParser::parse(QStringLiteral("Oct  9 23:15:00 jupiter a: b"), SyslogDateParser::Automatic, QString(), 2026);
    QVERIFY(result.isValid());
    QCOMPARE(result.dateTime, QDateTime(QDate(2026, 10, 9), QTime(23, 15, 0)));
}

void SyslogDateParserTest::testCustom()
{
    const QString line = QStringLiteral("2026/10/09 23:15:00 jupiter a: b");
    const auto result = SyslogDateParser::parse(line, SyslogDateParser::Custom, QStringLiteral("yyyy/MM/dd HH:mm:ss"), 2000);
    QVERIFY(result.isValid());
    QCOMPARE(result.dateTime, QDateTime(QDate(2026, 10, 9), QTime(23, 15, 0)));
    QCOMPARE(line.mid(result.consumed), QStringLiteral("jupiter a: b"));
}

void SyslogDateParserTest::testCustomWithoutYear()
{
    // Multiple spaces between words are accepted
    const QString line = QStringLiteral("Oct  9 23:15:00 jupiter a: b");
    const auto result = SyslogDateParser::parse(line, SyslogDateParser::Custom, QStringLiteral("MMM d HH:mm:ss"), 2026);
    QVERIFY(result.isValid());
    QCOMPARE(result.dateTime, QDateTime(QDate(2026, 10, 9), QTime(23, 15, 0)));
    QCOMPARE(line.mid(result.consumed), QStringLiteral("jupiter a: b"));
}

void SyslogDateParserTest::testCustomInvalid()
{
    QVERIFY(!SyslogDateParser::parse(QStringLiteral("2026/10/09 23:15:00 jupiter a: b"), SyslogDateParser::Custom, QString(), 2026).isValid());
    QVERIFY(!SyslogDateParser::parse(QStringLiteral("2026/10/09"), SyslogDateParser::Custom, QStringLiteral("yyyy/MM/dd HH:mm:ss"), 2026).isValid());
    QVERIFY(!SyslogDateParser::parse(QStringLiteral("foo bar baz"), SyslogDateParser::Custom, QStringLiteral("yyyy/MM/dd HH:mm:ss"), 2026).isValid());
}

QTEST_GUILESS_MAIN(SyslogDateParserTest)

#include "syslogDateParserTest.moc"
