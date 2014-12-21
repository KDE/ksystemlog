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

#include <kapplication.h>
#include <k4aboutdata.h>
#include <kcmdlineargs.h>
#include <KLocalizedString>
#include <kurl.h>

#include "mainWindow.h"
#include "logging.h"

int main(int argc, char** argv) {

	K4AboutData about(
			"ksystemlog",
			0,
			ki18n("KSystemlog"),
			"0.4",
			ki18n("System Logs Viewer for KDE"),
			K4AboutData::License_GPL_V2,
			ki18n("(C) 2007, Nicolas Ternisien"),
			ki18n("Do not hesitate to report bugs and problems to Nicolas Ternisien <a href='mailto:nicolas.ternisien@gmail.com'>nicolas.ternisien@gmail.com</a>"),
			"http://ksystemlog.forum-software.org",
			"nicolas.ternisien@gmail.com"
	);

	about.addAuthor(
			ki18n("Nicolas Ternisien"),
			ki18n("Main developer"),
			"nicolas.ternisien@gmail.com",
			"http://www.forum-software.org"
	);

	about.setProgramIconName(QLatin1String( "utilities-log-viewer" ));
	about.addCredit(ki18n("Bojan Djurkovic"), ki18n("Log Printing"), "dbojan@gmail.com");

	KCmdLineArgs::init(argc, argv, &about);

	KCmdLineOptions options;
	options.add("+[URL]", ki18n("Document to open"));

	KCmdLineArgs::addCmdLineOptions( options );

	KApplication app;

	//See if we are starting with session management
	if (app.isSessionRestored()) {
		RESTORE(KSystemLog::MainWindow);
	}
	else {
		//No session... Just start up normally
		KCmdLineArgs* args=KCmdLineArgs::parsedArgs();
		if (args->count()==0) {
			new KSystemLog::MainWindow();
		}
		else {
			/*KSystemLog::MainWindow* mainWindow;*/
			new KSystemLog::MainWindow();
			for (int i = 0; i < args->count(); i++) {
				logDebug() << "Loading file " << args->url(i) << endl;
				//TODO Implement this kind of loading
				//LogManager* firstLogManager = d->tabs->createTab();
				//d->tabs->load(Globals::instance()->findLogMode("openLogMode"), firstLogManager);
				//Open log mode need to automatically find the passed url : args->url(i)

			}
		}

		args->clear();
	}

	return app.exec();
}
