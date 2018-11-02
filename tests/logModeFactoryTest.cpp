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

#include <QTest>

#include "testUtil.h"

#include "analyzer.h"
#include "globals.h"

#include "logLevel.h"
#include "logMode.h"
#include "logFile.h"
#include "logViewModel.h"
#include "logViewWidget.h"

#include "logging.h"

Q_LOGGING_CATEGORY(KSYSTEMLOG, "ksystemlog")

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
    LogViewModel *model = NULL;
    Analyzer *systemAnalyzer = testUtil.createAnalyzer(QStringLiteral("systemLogMode"), &model);

    QVERIFY(systemAnalyzer);
    QVERIFY(model);
}

QTEST_MAIN(LogModeFactoryTest)

#include "logModeFactoryTest.moc"
