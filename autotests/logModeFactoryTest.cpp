/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include <QTest>

#include "testUtil.h"

#include "analyzer.h"
#include "globals.h"

#include "logFile.h"
#include "logLevel.h"
#include "logMode.h"
#include "logViewModel.h"
#include "logViewWidget.h"

#include "ksystemlog_debug.h"

class LogModeFactoryTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void initTestCase();

    void testLogModes();

    void testReaderFactory();

private:
    TestUtil testUtil;
};

void LogModeFactoryTest::initTestCase()
{
    testUtil.registerLogModeFactories();
}

void LogModeFactoryTest::testLogModes()
{
    LogMode *systemLogMode = Globals::instance().findLogMode(QStringLiteral("systemLogMode"));
    QVERIFY(systemLogMode);

    QCOMPARE(systemLogMode->id(), QString::fromLatin1("systemLogMode"));
}

void LogModeFactoryTest::testReaderFactory()
{
    LogViewModel *model = nullptr;
    Analyzer *systemAnalyzer = testUtil.createAnalyzer(QStringLiteral("systemLogMode"), &model);

    QVERIFY(systemAnalyzer);
    QVERIFY(model);
    delete systemAnalyzer;
}

QTEST_MAIN(LogModeFactoryTest)

#include "logModeFactoryTest.moc"
