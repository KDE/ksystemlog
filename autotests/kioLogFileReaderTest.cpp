/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   Copyright (C) 2007 by Nicolas Ternisien                               *
 *   nicolas.ternisien@gmail.com                                           *
 *   Copyright (C) 2016 by Harald Sitter                                   *
 *   sitter@kde.org                                                        *
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

//#include <qtest_kde.h>

#include "kioLogFileReader.h"
#include "logFile.h"

#include "testUtil.h"

#include "logging.h"

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
    logDebug() << "Hello";
}

void KioLogFileReaderTest::testKioLogFileReader()
{
    QString fixturePath = QFINDTESTDATA("testFiles/logFileReader/file.txt");
    LogFile logFile(QUrl::fromLocalFile(fixturePath), Globals::instance().informationLogLevel());

    auto logFileReader = new KioLogFileReader(logFile);
    logFileReader->open();

    connect(logFileReader, &KioLogFileReader::lineRead, this, [=](const QString &line) {
        logDebug() << "Line " << line;
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
