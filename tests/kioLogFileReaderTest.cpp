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

#include <qtest_kde.h>

#include "logFile.h"
#include "kioLogFileReader.h"

#include "testUtil.h"

#include "logging.h"

class KioLogFileReaderTest: public QObject {

	Q_OBJECT

private slots:

	void initTestCase();
	
	void testKioLogFileReader();
	
	void readLine(const QString& line);

private:
	TestUtil testUtil;

};


void KioLogFileReaderTest::initTestCase() {
	logDebug() << "Hello" << endl;
}


void KioLogFileReaderTest::testKioLogFileReader() {
	/*
	QList<LogFile> logFiles = testUtil.createLogFiles(":/logs/logFileReader/file.txt");
	LogFile logFile = logFiles.first();
	*/
	
	///home/nicolas/test.txt
  LogFile logFile(QUrl::fromLocalFile("http://localhost/test.txt"), Globals::instance()->informationLogLevel());
	
	KioLogFileReader* logFileReader = new KioLogFileReader(logFile);
	
	logFileReader->open();
	

	connect(logFileReader, SIGNAL(lineRead(QString)), this, SLOT(readLine(QString)));
	
	QTest::qWait(100000);
	
	

}

void KioLogFileReaderTest::readLine(const QString& line) {
	logDebug() << "Line " << line << endl;
}

QTEST_KDEMAIN(KioLogFileReaderTest, GUI)

#include "kioLogFileReaderTest.moc"
