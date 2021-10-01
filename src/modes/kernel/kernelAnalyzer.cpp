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

#include "kernelAnalyzer.h"

KernelAnalyzer::KernelAnalyzer(LogMode *logMode)
    : FileAnalyzer(logMode)
{
    startupTime();
}

LogViewColumns KernelAnalyzer::initColumns()
{
    LogViewColumns columns;
    columns.addColumn(LogViewColumn(i18n("Date"), true, false));
    columns.addColumn(LogViewColumn(i18n("Component"), true, false));
    columns.addColumn(LogViewColumn(i18n("Message"), true, false));

    return columns;
}

LogFileReader *KernelAnalyzer::createLogFileReader(const LogFile &logFile)
{
    return new ProcessOutputLogFileReader(logFile);
}

void KernelAnalyzer::startupTime()
{
    QFile file(QStringLiteral(UPTIME_FILE));

    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream in(&file);
    const QString line = in.readLine();

    // Format : 1618.72 1382.98 (uptime / something)
    const QStringList times = line.split(QLatin1Char(' '));

    const QString secondsString = times.at(0);
    const QString pureSecondsString = secondsString.left(secondsString.indexOf(QLatin1Char('.')));
    const long updateSeconds = pureSecondsString.toLong();

    mStartupDateTime = QDateTime::currentDateTime().addSecs(-updateSeconds);
    logDebug() << "Startup time : " << mStartupDateTime;
}

LogLine *KernelAnalyzer::parseMessage(const QString &logLine, const LogFile &originalLogFile)
{
    const QRegExp timeRegex(QStringLiteral("\\[\\ *(\\d*)\\.(\\d*)\\]\\s+(.*)"));

    //			QRegExp componentRegexp(timeRegex + "([^\\s:]{,20})[:\\s\\t]+(.*)");
    //			QRegExp messageRegexp(timeRegex + "(.*)");

    QDateTime dateTime(mStartupDateTime);
    QStringList messages;

    int timeExists = timeRegex.indexIn(logLine);

    // If we have the date, we are able to update the start date
    if (timeExists != -1) {
        // logDebug() << componentRegexp.cap(1).toInt() << "and" << componentRegexp.cap(2).toInt();
        dateTime = dateTime.addSecs(timeRegex.cap(1).toInt());
        dateTime = dateTime.addMSecs(timeRegex.cap(2).toInt() / 1000);

        parseComponentMessage(timeRegex.cap(3), messages);
    }
    // Else, the date will never change
    else {
        parseComponentMessage(logLine, messages);
    }

    /*
  logDebug() << "--------------------------------";
  logDebug() << logLine;
  logDebug() << "Secs : " << dateTime.time().second();
  logDebug() << "MSec : " << dateTime.time().msec();
  logDebug() << "Comp : " << messages.at(0);
  logDebug() << "Msg  : " << messages.at(1);
  logDebug() << "--------------------------------";
        */

    auto line = new LogLine(mLogLineInternalIdGenerator++,
                            dateTime,
                            messages,
                            originalLogFile.url().toLocalFile(),
                            Globals::instance().informationLogLevel(),
                            mLogMode);

    return line;
}
