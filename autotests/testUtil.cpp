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

#include "testUtil.h"
#include <QFile>
#include <QTemporaryFile>
#include <QTest>

//#include <qtest_kde.h>

#include "kernelFactory.h"
#include "systemFactory.h"

#include "logging.h"

#include "logViewModel.h"
#include "logViewWidget.h"

#include "analyzer.h"
static void enableLoggign()
{
    KSYSTEMLOG().setFilterRules(QStringLiteral("ksystemlog=true"));
    Q_INIT_RESOURCE(testResources);
}

Q_COREAPP_STARTUP_FUNCTION(enableLoggign)

TestUtil::TestUtil()
{
}

TestUtil::~TestUtil()
{
}

void TestUtil::registerLogModeFactories() const
{
    logDebug() << "Registering existing log mode factories";
    Globals::instance().registerLogModeFactory(new SystemLogModeFactory());
    Globals::instance().registerLogModeFactory(new KernelLogModeFactory());
}

LogViewModel *TestUtil::defineLogViewModel(Analyzer *analyzer) const
{
    auto logViewWidget = new LogViewWidget();
    auto model = new LogViewModel(logViewWidget);

    analyzer->setLogViewModel(model);

    return model;
}

Analyzer *TestUtil::createAnalyzer(const QString &logModeName, LogViewModel **model) const
{
    Analyzer *analyzer = Globals::instance().findLogMode(logModeName)->createAnalyzer();

    *model = defineLogViewModel(analyzer);

    return analyzer;
}

void TestUtil::destroyReader(Analyzer *analyzer) const
{
    // TODO Unable to delete the created LogViewWidget, because the LogViewModel is not accessible from Reader
    delete analyzer;
}

QVector<LogFile> TestUtil::createLogFiles(const QString &resourceFileName) const
{
    QTemporaryFile *tempFile = QTemporaryFile::createNativeFile(resourceFileName);
    logDebug() << "Using log file name " << tempFile;
    tempFile->setPermissions(QFile::WriteUser | QFile::ReadUser | QFile::ReadOwner | QFile::WriteOwner);

    LogLevel *informationLogLevel = Globals::instance().informationLogLevel();

    QVector<LogFile> logFiles;
    LogFile logFile(QUrl::fromLocalFile(tempFile->fileName()), informationLogLevel);
    logFiles.append(logFile);

    return logFiles;
}

void TestUtil::testLine(LogLine *line, const QString &originalFileName, LogLevel *logLevel, const QDateTime &time, const QStringList &items) const
{
    QCOMPARE(line->time(), time);
    QCOMPARE(line->sourceFileName(), originalFileName);
    QCOMPARE(line->logLevel()->id(), logLevel->id());

    // Test log line items
    QStringList logItems = line->logItems();
    QCOMPARE(logItems.count(), items.count());
    QCOMPARE(logItems, items);
}

void TestUtil::addLogLines(const QString &fileName, const QStringList &addedLines) const
{
    // Wait 1 sec to be sure the dirty signal will be emitted
    QTest::qWait(1000);

    QFile data(fileName);
    if (data.open(QFile::Append | QIODevice::Text)) {
        logDebug() << "Opening " << fileName << " for writing " << addedLines.count() << " line(s).";

        QTextStream out(&data);
        for (const QString &line : addedLines) {
            out << line << '\n';
        }

        out.flush();
        data.close();
    } else {
        QFAIL(QString::fromLatin1("Unable to open the test log file %1").arg(fileName).toUtf8().constData());
    }

    // Wait 3 secs to be sure the log file changed have been processed
    QTest::qWait(1000);
}
