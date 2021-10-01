/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
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

#include "logFile.h"
#include "logLevel.h"
#include "logMode.h"
#include "logViewModel.h"
#include "logViewWidget.h"

#include "logging.h"

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
