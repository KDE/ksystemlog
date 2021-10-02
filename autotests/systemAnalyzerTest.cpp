/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include <QList>
#include <QSignalSpy>
#include <QStringList>
#include <QTest>
#include <QThread>

#include <KDirWatch>

#include "testUtil.h"

#include "analyzer.h"
#include "globals.h"

#include "logFile.h"
#include "logLevel.h"
#include "logViewModel.h"
#include "logViewWidget.h"

#include "ksystemlogConfig.h"

#include "ksystemlog_debug.h"

class SystemAnalyzerTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void initTestCase();

    void testMultipleLines();
    void testOneLine();
    void testTwoLines();
    void testStrangeLines();

    void testDeleteProcessIdentifier();
    void testMaxLines();
    void testRemoveDuplicates();

private:
    void compareWithMinTime(const QList<LogLine *> &lines, const QDateTime &minTime);

private:
    TestUtil testUtil;
};

void SystemAnalyzerTest::initTestCase()
{
    testUtil.registerLogModeFactories();
}

void SystemAnalyzerTest::testOneLine()
{
    LogViewModel *model = nullptr;

    Analyzer *systemAnalyzer = testUtil.createAnalyzer(QStringLiteral("systemLogMode"), &model);
    QVERIFY(systemAnalyzer);
    QVERIFY(model);

    QVector<LogFile> logFiles = testUtil.createLogFiles(QStringLiteral(":/testFiles/default/one-line.log"));

    systemAnalyzer->setLogFiles(logFiles);

    systemAnalyzer->watchLogFiles(true);

    QCOMPARE(model->itemCount(), 1);
    QCOMPARE(model->isEmpty(), false);

    QList<LogLine *> logLines = model->logLines();

    QStringList items = QStringList() << QStringLiteral("localhost") << QStringLiteral("kernel")
                                      << QStringLiteral("[11663.656000] eth1: no IPv6 routers present");

    const int year = QDate::currentDate().year();
    testUtil.testLine(logLines.at(0),
                      logFiles.at(0).url().toLocalFile(),
                      logFiles.at(0).defaultLogLevel(),
                      QDateTime(QDate(year, 8, 21), QTime(22, 52, 44)),
                      items

    );

    testUtil.destroyReader(systemAnalyzer);
}

void SystemAnalyzerTest::testTwoLines()
{
    LogViewModel *model = nullptr;

    Analyzer *systemAnalyzer = testUtil.createAnalyzer(QStringLiteral("systemLogMode"), &model);
    QVERIFY(systemAnalyzer);
    QVERIFY(model);

    // Specifical configuration
    KSystemLogConfig::setMaxLines(1000);

    QVector<LogFile> logFiles = testUtil.createLogFiles(QStringLiteral(":/testFiles/default/two-lines.log"));

    systemAnalyzer->setLogFiles(logFiles);

    systemAnalyzer->watchLogFiles(true);

    QCOMPARE(model->itemCount(), 2);
    QCOMPARE(model->isEmpty(), false);

    testUtil.destroyReader(systemAnalyzer);
}

void SystemAnalyzerTest::testMultipleLines()
{
    LogViewModel *model = nullptr;

    Analyzer *systemAnalyzer = testUtil.createAnalyzer(QStringLiteral("systemLogMode"), &model);
    QVERIFY(systemAnalyzer);
    QVERIFY(model);

    // Specifical configuration
    KSystemLogConfig::setMaxLines(1000);

    QVector<LogFile> logFiles = testUtil.createLogFiles(QStringLiteral(":/testFiles/system/system.log"));
    LogFile logFile = logFiles.at(0);

    systemAnalyzer->setLogFiles(logFiles);

    QSignalSpy stateSpy(systemAnalyzer, &Analyzer::logUpdated);
    QList<QVariant> arguments;

    // Each watching relaunch a reading
    systemAnalyzer->watchLogFiles(true);

    // Assert that the model has been updated
    QCOMPARE(model->itemCount(), 24);

    // Assert that the logUpdated signal emits the right count
    QCOMPARE(stateSpy.count(), 1);
    arguments = stateSpy.takeFirst();
    QCOMPARE(arguments.at(0).toInt(), 24);

    // Each watching relaunch a reading
    systemAnalyzer->watchLogFiles(true);

    // Assert that the model has been updated
    QCOMPARE(model->itemCount(), 48);

    // Assert that the logUpdated signal emits the right count
    QCOMPARE(stateSpy.count(), 1);
    arguments = stateSpy.takeFirst();
    QCOMPARE(arguments.at(0).toInt(), 24);

    QStringList addedLines;
    addedLines << QStringLiteral("Aug 18 17:04:28 localhost test: Test line 1");
    addedLines << QStringLiteral("Aug 18 17:04:30 localhost test: Test line 2");

    testUtil.addLogLines(logFile.url().toLocalFile(), addedLines);

    // Assert that the model has been updated
    QCOMPARE(model->itemCount(), 50);

    // Assert that the logUpdated signal emits the right count
    QCOMPARE(stateSpy.count(), 1);
    arguments = stateSpy.takeFirst();
    QCOMPARE(arguments.at(0).toInt(), 2);

    testUtil.destroyReader(systemAnalyzer);
}

void SystemAnalyzerTest::testStrangeLines()
{
    LogViewModel *model = nullptr;

    Analyzer *systemAnalyzer = testUtil.createAnalyzer(QStringLiteral("systemLogMode"), &model);
    QVERIFY(systemAnalyzer);
    QVERIFY(model);

    // Specifical configuration
    KSystemLogConfig::setMaxLines(1000);
    KSystemLogConfig::setDeleteProcessIdentifier(false);

    QVector<LogFile> logFiles = testUtil.createLogFiles(QStringLiteral(":/testFiles/system/strange-lines.log"));

    systemAnalyzer->setLogFiles(logFiles);

    systemAnalyzer->watchLogFiles(true);

    QCOMPARE(model->itemCount(), 8);

    // i18n("undefined")
    QString undefined = QLatin1String("");

    QStringList items;

    QSKIP("This test/code is broken");
    const int year = QDate::currentDate().year();

    // Classical log line
    items = QStringList() << QStringLiteral("localhost") << QStringLiteral("kernel") << QStringLiteral("Kernel panic");
    testUtil.testLine(model->logLines().at(0),
                      logFiles.at(0).url().toLocalFile(),
                      logFiles.at(0).defaultLogLevel(),
                      QDateTime(QDate(year, 8, 10), QTime(17, 04, 28)),
                      items);

    //-- MARK -- log line
    items = QStringList() << QStringLiteral("localhost") << QStringLiteral("syslog") << QStringLiteral("-- MARK --");
    testUtil.testLine(model->logLines().at(1),
                      logFiles.at(0).url().toLocalFile(),
                      logFiles.at(0).defaultLogLevel(),
                      QDateTime(QDate(year, 8, 11), QTime(13, 49, 38)),
                      items);

    // Last message repeated n time log line
    items = QStringList() << QStringLiteral("localhost") << QStringLiteral("syslog") << QStringLiteral("last message repeated 4 times");
    testUtil.testLine(model->logLines().at(2),
                      logFiles.at(0).url().toLocalFile(),
                      logFiles.at(0).defaultLogLevel(),
                      QDateTime(QDate(year, 8, 12), QTime(18, 10, 32)),
                      items);

    //"Aug 13 17:04:28 testprocess: Say ouhou  " -> No host name
    items = QStringList() << undefined << QStringLiteral("testprocess") << QStringLiteral("Say ouhou  ");
    testUtil.testLine(model->logLines().at(3),
                      logFiles.at(0).url().toLocalFile(),
                      logFiles.at(0).defaultLogLevel(),
                      QDateTime(QDate(year, 8, 13), QTime(17, 04, 28)),
                      items);

    //"Aug 14 17:04:28 localhost kernel say ouhou" -> No process name and not a syslog message
    items = QStringList() << QStringLiteral("localhost") << undefined << QStringLiteral("kernel say ouhou");
    testUtil.testLine(model->logLines().at(4),
                      logFiles.at(0).url().toLocalFile(),
                      logFiles.at(0).defaultLogLevel(),
                      QDateTime(QDate(year, 8, 14), QTime(17, 04, 28)),
                      items);

    //"Aug 15 22:39:01 localhost /USR/SBIN/CRON[9433]: (root) CMD (  [ -d /var/lib/php5 ] && find
    /// var/lib/php5/ -type f -cmin +$(/usr/lib/php5/maxlifetime) -print0 | xargs -r -0 rm)" -> Long log line
    items = QStringList() << QStringLiteral("localhost") << QStringLiteral("/USR/SBIN/CRON[9433]")
                          << QStringLiteral(
                                 "(root) CMD (  [ -d /var/lib/php5 ] && find /var/lib/php5/ -type f -cmin "
                                 "+$(/usr/lib/php5/maxlifetime) -print0 | xargs -r -0 rm)");
    testUtil.testLine(model->logLines().at(5),
                      logFiles.at(0).url().toLocalFile(),
                      logFiles.at(0).defaultLogLevel(),
                      QDateTime(QDate(year, 8, 15), QTime(22, 39, 01)),
                      items);

    //"blablalbla" -> Invalid line
    items = QStringList() << undefined << undefined << QLatin1String("");
    QCOMPARE(model->logLines().at(6)->logItems(), items);

    //"" -> Empty line
    items = QStringList() << undefined << undefined << QStringLiteral("blablalbla");
    QCOMPARE(model->logLines().at(7)->logItems(), items);

    testUtil.destroyReader(systemAnalyzer);
}

void SystemAnalyzerTest::testDeleteProcessIdentifier()
{
    LogViewModel *model = nullptr;

    Analyzer *systemAnalyzer = testUtil.createAnalyzer(QStringLiteral("systemLogMode"), &model);
    QVERIFY(systemAnalyzer);
    QVERIFY(model);

    // Specifical configuration
    KSystemLogConfig::setMaxLines(1000);
    KSystemLogConfig::setDeleteProcessIdentifier(true);

    QVector<LogFile> logFiles = testUtil.createLogFiles(QStringLiteral(":/testFiles/system/delete-process-identifier.log"));

    systemAnalyzer->setLogFiles(logFiles);

    systemAnalyzer->watchLogFiles(true);

    QCOMPARE(model->itemCount(), 2);

    QStringList items;

    // Cron log line
    items = QStringList() << QStringLiteral("localhost") << QStringLiteral("/USR/SBIN/CRON") << QStringLiteral("Hello");
    QCOMPARE(model->logLines().at(0)->logItems(), items);

    //"f" process
    items = QStringList() << QStringLiteral("localhost") << QStringLiteral("f") << QStringLiteral("Ola");
    QCOMPARE(model->logLines().at(1)->logItems(), items);

    testUtil.destroyReader(systemAnalyzer);
}

void SystemAnalyzerTest::testMaxLines()
{
    LogViewModel *model = nullptr;

    Analyzer *systemAnalyzer = testUtil.createAnalyzer(QStringLiteral("systemLogMode"), &model);
    QVERIFY(systemAnalyzer);
    QVERIFY(model);

    // Specifical configuration
    KSystemLogConfig::setMaxLines(5);

    QVector<LogFile> logFiles = testUtil.createLogFiles(QStringLiteral(":/testFiles/system/max-lines.log"));
    LogFile logFile = logFiles.at(0);

    systemAnalyzer->setLogFiles(logFiles);

    systemAnalyzer->watchLogFiles(true);

    QCOMPARE(model->itemCount(), 5);

    // Asserts that there is no line before the oldest one's time
    compareWithMinTime(model->logLines(), QDateTime(QDate(2007, 8, 18), QTime(11, 0, 0)));

    QStringList addedLines;

    addedLines << QStringLiteral("Aug 18 10:00:00 localhost test: Line 8");
    testUtil.addLogLines(logFile.url().toLocalFile(), addedLines);

    QCOMPARE(model->itemCount(), 5);
    compareWithMinTime(model->logLines(), QDateTime(QDate(2007, 8, 18), QTime(11, 0, 0)));

    // Specifical configuration
    KSystemLogConfig::setMaxLines(6);

    addedLines.clear();
    addedLines << QStringLiteral("Aug 18 10:00:00 localhost test: Line 9");
    addedLines << QStringLiteral("Aug 18 19:00:00 localhost test: Line 10");
    testUtil.addLogLines(logFile.url().toLocalFile(), addedLines);

    QCOMPARE(model->itemCount(), 6);
    compareWithMinTime(model->logLines(), QDateTime(QDate(2007, 8, 18), QTime(11, 0, 0)));

    addedLines.clear();
    addedLines << QStringLiteral("Aug 18 20:00:00 localhost test: Line 11");
    addedLines << QStringLiteral("Aug 18 21:00:00 localhost test: Line 12");
    testUtil.addLogLines(logFile.url().toLocalFile(), addedLines);

    QCOMPARE(model->itemCount(), 6);
    compareWithMinTime(model->logLines(), QDateTime(QDate(2007, 8, 18), QTime(13, 0, 0)));

    testUtil.destroyReader(systemAnalyzer);
}

void SystemAnalyzerTest::compareWithMinTime(const QList<LogLine *> &logLines, const QDateTime &minTime)
{
    qCDebug(KSYSTEMLOG) << "Min time : " << minTime.toString();

    for (LogLine *logLine : logLines) {
        if (logLine->time() < minTime) {
            QFAIL(QString::fromLatin1("The line '%1' has a lesser time than the required min time (%2)")
                      .arg(logLine->logItems().join(QLatin1Char(' ')), logLine->time().toString())
                      .toUtf8()
                      .constData());
        }
    }
}

void SystemAnalyzerTest::testRemoveDuplicates()
{
    LogViewModel *model = nullptr;

    Analyzer *systemAnalyzer = testUtil.createAnalyzer(QStringLiteral("systemLogMode"), &model);
    QVERIFY(systemAnalyzer);
    QVERIFY(model);

    // Specifical configuration
    KSystemLogConfig::setMaxLines(1000);
    KSystemLogConfig::setDeleteDuplicatedLines(true);

    QVector<LogFile> logFiles = testUtil.createLogFiles(QStringLiteral(":/testFiles/system/duplicate-lines.log"));

    systemAnalyzer->setLogFiles(logFiles);

    systemAnalyzer->watchLogFiles(true);

    QCOMPARE(model->itemCount(), 5);
}

QTEST_MAIN(SystemAnalyzerTest)

#include "systemAnalyzerTest.moc"
