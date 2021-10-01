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

#include "sambaAnalyzer.h"

SambaAnalyzer::SambaAnalyzer(LogMode *logMode)
    : FileAnalyzer(logMode)
{
    mCurrentLogLine = nullptr;
}

LogViewColumns SambaAnalyzer::initColumns()
{
    LogViewColumns columns;

    columns.addColumn(LogViewColumn(i18n("Date"), true, false));
    columns.addColumn(LogViewColumn(i18n("Source File"), true, true));
    columns.addColumn(LogViewColumn(i18n("Function"), true, true));
    columns.addColumn(LogViewColumn(i18n("Line"), true, true));
    columns.addColumn(LogViewColumn(i18n("Message"), true, false));

    return columns;
}

LogFileReader *SambaAnalyzer::createLogFileReader(const LogFile &logFile)
{
    return new LocalLogFileReader(logFile);
}

Analyzer::LogFileSortMode SambaAnalyzer::logFileSortMode()
{
    return Analyzer::AscendingSortedLogFile;
}

LogLine *SambaAnalyzer::parseMessage(const QString &logLine, const LogFile &originalLogFile)
{
    QString line(logLine);

    // The Date
    const int dateBegin = line.indexOf(QLatin1String("["));
    const int dateEnd = line.indexOf(QLatin1String("]"));

    if (dateBegin != -1) {
        const QString strDate = line.mid(dateBegin + 1, dateEnd - dateBegin - 1);

        const QString year = strDate.mid(0, 4);
        const QString month = strDate.mid(5, 2);
        const QString day = strDate.mid(8, 2);

        const QString hour = strDate.mid(11, 2);
        const QString min = strDate.mid(14, 2);
        const QString sec = strDate.mid(17, 2);

        const QDate date = QDate(year.toInt(), month.toInt(), day.toInt());
        const QTime time = QTime(hour.toInt(), min.toInt(), sec.toInt());

        line.remove(0, dateEnd + 2);

        // The source file
        int doubleDot;
        doubleDot = line.indexOf(QLatin1Char(':'));
        QString file = line.left(doubleDot);
        line.remove(0, doubleDot + 1);

        // The function
        int bracket = line.indexOf(QLatin1Char('('));
        const QString function = line.left(bracket);
        line.remove(0, bracket + 1);

        // The line number
        bracket = line.indexOf(QLatin1Char(')'));
        const QString lineNumber = line.left(bracket);

        // Remove the first return character and the two useless space of the first message line
        line.remove(0, bracket + 4);

        QStringList list;
        list.append(file);
        list.append(function);
        list.append(lineNumber);

        logDebug() << "Creating new line ";

        LogLine *returnedLogLine = mCurrentLogLine;

        mCurrentLogLine = new LogLine(mLogLineInternalIdGenerator++,
                                      QDateTime(date, time),
                                      list,
                                      originalLogFile.url().toLocalFile(),
                                      Globals::instance().informationLogLevel(),
                                      mLogMode);

        return returnedLogLine;
    }

    if (line.indexOf(QLatin1String("  ")) != -1) {
        if (mCurrentLogLine) {
            QStringList list = mCurrentLogLine->logItems();

            // A line has already been added
            if (list.count() == 4) {
                const QString currentMessage = list.takeLast();
                list.append(currentMessage + QLatin1String("\n") + line.simplified());
            }
            // First time we add a line for the current Log line
            else {
                list.append(line.simplified());
            }

            mCurrentLogLine->setLogItems(list);
        }
    }

    return nullptr;
}
