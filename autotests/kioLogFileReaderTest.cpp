/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>
    SPDX-FileCopyrightText: 2016 Harald Sitter <sitter@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include <QTest>

//#include <qtest_kde.h>

#include "kioLogFileReader.h"
#include "logFile.h"

#include "testUtil.h"

#include "ksystemlog_debug.h"

class KioLogFileReaderTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void initTestCase();

    void testKioLogFileReader();

private:
    TestUtil testUtil;
};

void KioLogFileReaderTest::initTestCase()
{
    qCDebug(KSYSTEMLOG) << "Hello";
}

void KioLogFileReaderTest::testKioLogFileReader()
{
    QString const fixturePath = QFINDTESTDATA("testFiles/logFileReader/file.txt");
    LogFile const logFile(QUrl::fromLocalFile(fixturePath), Globals::instance().informationLogLevel());

    auto logFileReader = new KioLogFileReader(logFile);
    logFileReader->open();

    connect(logFileReader, &KioLogFileReader::lineRead, this, [=](const QString &line) {
        qCDebug(KSYSTEMLOG) << "Line " << line;
        static QFile file(fixturePath);
        static bool open = false;
        if (!open) {
            QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));
            open = true;
        }
        static QTextStream stream(&file);
        QCOMPARE(line, stream.readLine());
    });
    delete logFileReader;
}

QTEST_MAIN(KioLogFileReaderTest)

#include "kioLogFileReaderTest.moc"
