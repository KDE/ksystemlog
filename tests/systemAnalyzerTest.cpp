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

#include <QList>
#include <QStringList>
#include <QThread>

#include <qtest_kde.h>

#include <kurl.h>
#include <kdirwatch.h>

#include "testUtil.h"

#include "analyzer.h"
#include "globals.h"

#include "logLevel.h"
#include "logFile.h"
#include "logViewModel.h"
#include "logViewWidget.h"

#include "ksystemlogConfig.h"

#include "logging.h"

class SystemAnalyzerTest : public QObject {

	Q_OBJECT
	
private slots:
	
	void initTestCase();
	
	void testMultipleLines();
	void testOneLine();
	void testTwoLines();
	void testStrangeLines();

	void testDeleteProcessIdentifier();
	void testMaxLines();
	void testRemoveDuplicates();

private:
	void compareWithMinTime(QList<LogLine*> lines, const QDateTime& minTime);
	
private:
	TestUtil testUtil;

};

void SystemAnalyzerTest::initTestCase() {
	testUtil.registerLogModeFactories();
}

void SystemAnalyzerTest::testOneLine() {

	LogViewModel* model= NULL;

	Analyzer* systemAnalyzer = testUtil.createAnalyzer(QLatin1String("systemLogMode"), &model);
	QVERIFY(systemAnalyzer);
	QVERIFY(model);

	QList<LogFile> logFiles = testUtil.createLogFiles(QLatin1String(":/testFiles/default/one-line.log"));

	systemAnalyzer->setLogFiles(logFiles);

	systemAnalyzer->watchLogFiles(true);

	QCOMPARE(model->itemCount(), 1);
	QCOMPARE(model->isEmpty(), false);

	QList<LogLine*> logLines = model->logLines();

	QStringList items = QStringList() << QLatin1String("localhost") << QLatin1String("kernel") << QLatin1String("[11663.656000] eth1: no IPv6 routers present");

	const int year = QDate::currentDate().year();
	testUtil.testLine(logLines.at(0), logFiles.at(0).url().path(), logFiles.at(0).defaultLogLevel(), QDateTime(QDate(year, 8, 21), QTime(22, 52, 44)), items

	);

	testUtil.destroyReader(systemAnalyzer);
}

void SystemAnalyzerTest::testTwoLines() {

	LogViewModel* model= NULL;

	Analyzer* systemAnalyzer = testUtil.createAnalyzer(QLatin1String("systemLogMode"), &model);
	QVERIFY(systemAnalyzer);
	QVERIFY(model);

	//Specifical configuration
	KSystemLogConfig::setMaxLines(1000);
	
	QList<LogFile> logFiles = testUtil.createLogFiles(QLatin1String(":/testFiles/default/two-lines.log"));

	systemAnalyzer->setLogFiles(logFiles);

	systemAnalyzer->watchLogFiles(true);

	QCOMPARE(model->itemCount(), 2);
	QCOMPARE(model->isEmpty(), false);

	testUtil.destroyReader(systemAnalyzer);
}

void SystemAnalyzerTest::testMultipleLines() {
	LogViewModel* model= NULL;

	Analyzer* systemAnalyzer = testUtil.createAnalyzer(QLatin1String("systemLogMode"), &model);
	QVERIFY(systemAnalyzer);
	QVERIFY(model);

	//Specifical configuration
	KSystemLogConfig::setMaxLines(1000);
	
	QList<LogFile> logFiles = testUtil.createLogFiles(QLatin1String(":/testFiles/system/system.log"));
	LogFile logFile = logFiles.at(0);

	systemAnalyzer->setLogFiles(logFiles);


	QSignalSpy stateSpy(systemAnalyzer, SIGNAL(logUpdated(int)));
	QList<QVariant> arguments;

	//Each watching relaunch a reading
	systemAnalyzer->watchLogFiles(true);

	//Assert that the model has been updated
	QCOMPARE(model->itemCount(), 24);

	//Assert that the logUpdated signal emits the right count
	QCOMPARE(stateSpy.count(), 1);
	arguments = stateSpy.takeFirst();
	QCOMPARE(arguments.at(0).toInt(), 24);

	//Each watching relaunch a reading
	systemAnalyzer->watchLogFiles(true);

	//Assert that the model has been updated
	QCOMPARE(model->itemCount(), 48);

	//Assert that the logUpdated signal emits the right count
	QCOMPARE(stateSpy.count(), 1);
	arguments = stateSpy.takeFirst();
	QCOMPARE(arguments.at(0).toInt(), 24);

	QStringList addedLines;
	addedLines << QLatin1String("Aug 18 17:04:28 localhost test: Test line 1");
	addedLines << QLatin1String("Aug 18 17:04:30 localhost test: Test line 2");
	
	testUtil.addLogLines(logFile.url().path(), addedLines);

	//Assert that the model has been updated
	QCOMPARE(model->itemCount(), 50);

	//Assert that the logUpdated signal emits the right count
	QCOMPARE(stateSpy.count(), 1);
	arguments = stateSpy.takeFirst();
	QCOMPARE(arguments.at(0).toInt(), 2);

	testUtil.destroyReader(systemAnalyzer);
}

void SystemAnalyzerTest::testStrangeLines() {
	LogViewModel* model= NULL;

	Analyzer* systemAnalyzer = testUtil.createAnalyzer(QLatin1String("systemLogMode"), &model);
	QVERIFY(systemAnalyzer);
	QVERIFY(model);

	//Specifical configuration
	KSystemLogConfig::setMaxLines(1000);
	KSystemLogConfig::setDeleteProcessIdentifier(false);

	QList<LogFile> logFiles = testUtil.createLogFiles(QLatin1String(":/testFiles/system/strange-lines.log"));

	systemAnalyzer->setLogFiles(logFiles);

	systemAnalyzer->watchLogFiles(true);

	QCOMPARE(model->itemCount(), 8);

	//i18n("undefined")
	QString undefined = QLatin1String("");
	
	QStringList items;
    
	const int year = QDate::currentDate().year();

	//Classical log line
	items = QStringList() << QLatin1String("localhost") << QLatin1String("kernel") << QLatin1String("Kernel panic");
	QEXPECT_FAIL("", "This test/code is broken", Abort);
	testUtil.testLine(model->logLines().at(0), logFiles.at(0).url().path(), logFiles.at(0).defaultLogLevel(), QDateTime(QDate(year, 8, 10), QTime(17, 04, 28)), items);

	//-- MARK -- log line
	items = QStringList() << QLatin1String("localhost") << QLatin1String("syslog") << QLatin1String("-- MARK --");
	QEXPECT_FAIL("", "This test/code is broken", Abort);
	testUtil.testLine(model->logLines().at(1), logFiles.at(0).url().path(), logFiles.at(0).defaultLogLevel(), QDateTime(QDate(year, 8, 11), QTime(13, 49, 38)), items);

	//Last message repeated n time log line
	items = QStringList() << QLatin1String("localhost") << QLatin1String("syslog") << QLatin1String("last message repeated 4 times");
	QEXPECT_FAIL("", "This test/code is broken", Abort);
	testUtil.testLine(model->logLines().at(2), logFiles.at(0).url().path(), logFiles.at(0).defaultLogLevel(), QDateTime(QDate(year, 8, 12), QTime(18, 10, 32)), items);

	//"Aug 13 17:04:28 testprocess: Say ouhou  " -> No host name
	items = QStringList() << undefined << QLatin1String("testprocess") << QLatin1String("Say ouhou  ");
	QEXPECT_FAIL("", "This test/code is broken", Abort);
	testUtil.testLine(model->logLines().at(3), logFiles.at(0).url().path(), logFiles.at(0).defaultLogLevel(), QDateTime(QDate(year, 8, 13), QTime(17, 04, 28)), items);

	//"Aug 14 17:04:28 localhost kernel say ouhou" -> No process name and not a syslog message
	items = QStringList() << QLatin1String("localhost") << undefined << QLatin1String("kernel say ouhou");
	QEXPECT_FAIL("", "This test/code is broken", Abort);
	testUtil.testLine(model->logLines().at(4), logFiles.at(0).url().path(), logFiles.at(0).defaultLogLevel(), QDateTime(QDate(year, 8, 14), QTime(17, 04, 28)), items);

	//"Aug 15 22:39:01 localhost /USR/SBIN/CRON[9433]: (root) CMD (  [ -d /var/lib/php5 ] && find /var/lib/php5/ -type f -cmin +$(/usr/lib/php5/maxlifetime) -print0 | xargs -r -0 rm)" -> Long log line
	items = QStringList() << QLatin1String("localhost") << QLatin1String("/USR/SBIN/CRON[9433]")
			<< QLatin1String("(root) CMD (  [ -d /var/lib/php5 ] && find /var/lib/php5/ -type f -cmin +$(/usr/lib/php5/maxlifetime) -print0 | xargs -r -0 rm)");
	QEXPECT_FAIL("", "This test/code is broken", Abort);
	testUtil.testLine(model->logLines().at(5), logFiles.at(0).url().path(), logFiles.at(0).defaultLogLevel(), QDateTime(QDate(year, 8, 15), QTime(22, 39, 01)), items);

	//"blablalbla" -> Invalid line
	items = QStringList() << undefined << undefined << QLatin1String("");
	QEXPECT_FAIL("", "This test/code is broken", Abort);
	QCOMPARE(model->logLines().at(6)->logItems(), items);

	//"" -> Empty line
	items = QStringList() << undefined << undefined << QLatin1String("blablalbla");
	QEXPECT_FAIL("", "This test/code is broken", Abort);
	QCOMPARE(model->logLines().at(7)->logItems(), items);

	testUtil.destroyReader(systemAnalyzer);
}

void SystemAnalyzerTest::testDeleteProcessIdentifier() {
	LogViewModel* model= NULL;

	Analyzer* systemAnalyzer = testUtil.createAnalyzer(QLatin1String("systemLogMode"), &model);
	QVERIFY(systemAnalyzer);
	QVERIFY(model);

	//Specifical configuration
	KSystemLogConfig::setMaxLines(1000);
	KSystemLogConfig::setDeleteProcessIdentifier(true);

	QList<LogFile> logFiles = testUtil.createLogFiles(QLatin1String(":/testFiles/system/delete-process-identifier.log"));

	systemAnalyzer->setLogFiles(logFiles);

	systemAnalyzer->watchLogFiles(true);

	QCOMPARE(model->itemCount(), 2);

	QStringList items;

	//Cron log line
	items = QStringList() << QLatin1String("localhost") << QLatin1String("/USR/SBIN/CRON") << QLatin1String("Hello");
	QCOMPARE(model->logLines().at(0)->logItems(), items);

	//"f" process
	items = QStringList() << QLatin1String("localhost") << QLatin1String("f") <<QLatin1String( "Ola");
	QCOMPARE(model->logLines().at(1)->logItems(), items);

	testUtil.destroyReader(systemAnalyzer);
}

void SystemAnalyzerTest::testMaxLines() {
	LogViewModel* model= NULL;

	Analyzer* systemAnalyzer = testUtil.createAnalyzer(QLatin1String("systemLogMode"), &model);
	QVERIFY(systemAnalyzer);
	QVERIFY(model);

	//Specifical configuration
	KSystemLogConfig::setMaxLines(5);

	QList<LogFile> logFiles = testUtil.createLogFiles(QLatin1String(":/testFiles/system/max-lines.log"));
	LogFile logFile = logFiles.at(0);

	systemAnalyzer->setLogFiles(logFiles);

	systemAnalyzer->watchLogFiles(true);

	QCOMPARE(model->itemCount(), 5);

	//Asserts that there is no line before the oldest one's time
	compareWithMinTime(model->logLines(), QDateTime(QDate(2007, 8, 18), QTime(11, 0, 0)));
	
	QStringList addedLines;

	addedLines << QLatin1String("Aug 18 10:00:00 localhost test: Line 8");
	testUtil.addLogLines(logFile.url().path(), addedLines);

	QCOMPARE(model->itemCount(), 5);
	compareWithMinTime(model->logLines(), QDateTime(QDate(2007, 8, 18), QTime(11, 0, 0)));

	//Specifical configuration
	KSystemLogConfig::setMaxLines(6);
	
	addedLines.clear();
	addedLines << QLatin1String("Aug 18 10:00:00 localhost test: Line 9");
	addedLines << QLatin1String("Aug 18 19:00:00 localhost test: Line 10");
	testUtil.addLogLines(logFile.url().path(), addedLines);

	QCOMPARE(model->itemCount(), 6);
	compareWithMinTime(model->logLines(), QDateTime(QDate(2007, 8, 18), QTime(11, 0, 0)));

	addedLines.clear();
	addedLines << QLatin1String("Aug 18 20:00:00 localhost test: Line 11");
	addedLines << QLatin1String("Aug 18 21:00:00 localhost test: Line 12");
	testUtil.addLogLines(logFile.url().path(), addedLines);

	QCOMPARE(model->itemCount(), 6);
	compareWithMinTime(model->logLines(), QDateTime(QDate(2007, 8, 18), QTime(13, 0, 0)));

	testUtil.destroyReader(systemAnalyzer);
}

void SystemAnalyzerTest::compareWithMinTime(QList<LogLine*> logLines, const QDateTime& minTime) {
	logDebug() << "Min time : " << minTime.toString() << endl;
	
	foreach (LogLine* logLine, logLines) {
		if (logLine->time() < minTime) {
			QFAIL( QString::fromLatin1("The line '%1' has a lesser time than the required min time (%2)").arg(logLine->logItems().join(QLatin1String(" "))).arg(logLine->time().toString()).toUtf8() );
		}
	}

}

void SystemAnalyzerTest::testRemoveDuplicates() {
	LogViewModel* model= NULL;

	Analyzer* systemAnalyzer = testUtil.createAnalyzer(QLatin1String("systemLogMode"), &model);
	QVERIFY(systemAnalyzer);
	QVERIFY(model);

	//Specifical configuration
	KSystemLogConfig::setMaxLines(1000);
	KSystemLogConfig::setDeleteDuplicatedLines(true);

	QList<LogFile> logFiles = testUtil.createLogFiles(QLatin1String(":/testFiles/system/duplicate-lines.log"));

	systemAnalyzer->setLogFiles(logFiles);

	systemAnalyzer->watchLogFiles(true);

	QCOMPARE(model->itemCount(), 5);
}


QTEST_KDEMAIN(SystemAnalyzerTest, GUI)

#include "systemAnalyzerTest.moc"
