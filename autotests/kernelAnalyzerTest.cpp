/***************************************************************************
 *   KKernelLog, a kernel log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#include <QList>
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

#include "logging.h"

#include "kernelAnalyzer.h"
#include "localLogFileReader.h"

/**
 * Reimplements the Kernel Analyzer using a Local File Reader
 */
class KernelAnalyzerLocalReader : public KernelAnalyzer
{
public:
    KernelAnalyzerLocalReader(LogMode *logMode)
        : KernelAnalyzer(logMode)
    {
    }

    ~KernelAnalyzerLocalReader() override
    {
    }

    QDateTime findStartupTime()
    {
        return mStartupDateTime;
    }

protected:
    LogFileReader *createLogFileReader(const LogFile &logFile) override
    {
        return new LocalLogFileReader(logFile);
    }
};

class KernelAnalyzerTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void initTestCase();

    void testUbuntuDmesg();
    void testSuseDmesg();

private:
    void compareWithMinTime(QList<LogLine *> lines, const QDateTime &minTime);

private:
    TestUtil testUtil;
};

void KernelAnalyzerTest::initTestCase()
{
    testUtil.registerLogModeFactories();
}

void KernelAnalyzerTest::testUbuntuDmesg()
{
    // Specifical configuration
    KSystemLogConfig::setMaxLines(1000);
    KSystemLogConfig::setDeleteDuplicatedLines(false);

    LogMode *logMode = Globals::instance().findLogMode(QStringLiteral("kernelLogMode"));
    auto kernelAnalyzer = new KernelAnalyzerLocalReader(logMode);
    LogViewModel *model = testUtil.defineLogViewModel(kernelAnalyzer);

    QVERIFY(kernelAnalyzer);
    QVERIFY(model);

    QVector<LogFile> logFiles = testUtil.createLogFiles(QStringLiteral(":/testFiles/kernel/ubuntu.dmesg"));

    kernelAnalyzer->setLogFiles(logFiles);

    kernelAnalyzer->watchLogFiles(true);

    QCOMPARE(model->itemCount(), 25);
    QCOMPARE(model->isEmpty(), false);

    QList<LogLine *> logLines = model->logLines();

    QStringList items = QStringList() << QStringLiteral("ADDRCONF(NETDEV_UP)") << QStringLiteral("eth0: link is not ready");
    QDateTime assertedDateTime = kernelAnalyzer->findStartupTime();
    assertedDateTime = assertedDateTime.addSecs(22);
    assertedDateTime = assertedDateTime.addMSecs(232);

    testUtil.testLine(logLines.at(0), logFiles.at(0).url().toLocalFile(), Globals::instance().informationLogLevel(), assertedDateTime, items);

    testUtil.destroyReader(kernelAnalyzer);
}

void KernelAnalyzerTest::testSuseDmesg()
{
    // Specifical configuration
    KSystemLogConfig::setMaxLines(1000);
    KSystemLogConfig::setDeleteDuplicatedLines(false);

    LogMode *logMode = Globals::instance().findLogMode(QStringLiteral("kernelLogMode"));
    auto kernelAnalyzer = new KernelAnalyzerLocalReader(logMode);
    LogViewModel *model = testUtil.defineLogViewModel(kernelAnalyzer);

    QVERIFY(kernelAnalyzer);
    QVERIFY(model);

    QVector<LogFile> logFiles = testUtil.createLogFiles(QStringLiteral(":/testFiles/kernel/suse.dmesg"));

    kernelAnalyzer->setLogFiles(logFiles);

    kernelAnalyzer->watchLogFiles(true);

    QCOMPARE(model->itemCount(), 23);
    QCOMPARE(model->isEmpty(), false);

    QList<LogLine *> logLines = model->logLines();

    QStringList items = QStringList() << QStringLiteral("r8169") << QStringLiteral("eth0: link down");

    testUtil.testLine(logLines.at(0), logFiles.at(0).url().toLocalFile(), Globals::instance().informationLogLevel(), kernelAnalyzer->findStartupTime(), items);

    testUtil.destroyReader(kernelAnalyzer);
}

QTEST_MAIN(KernelAnalyzerTest)

#include "kernelAnalyzerTest.moc"
