/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include <QApplication>
#include <QCommandLineParser>

#include <KAboutData>
#include <KLocalizedString>

#include "ksystemlog_version.h"
#include "logging.h"
#include "mainWindow.h"

int main(int argc, char **argv)
{
    // enable high dpi support
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);

    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain("ksystemlog");

    KAboutData about(QStringLiteral("ksystemlog"),
                     i18n("KSystemlog"),
                     QStringLiteral(KSYSTEMLOG_VERSION_STRING),
                     i18n("System Logs Viewer by KDE"),
                     KAboutLicense::GPL_V2,
                     i18n("(C) 2007-2015, Nicolas Ternisien"),
                     i18n("Do not hesitate to report bugs and problems to Nicolas Ternisien <a "
                          "href='mailto:nicolas.ternisien@gmail.com'>nicolas.ternisien@gmail.com</a>"),
                     QStringLiteral("https://apps.kde.org/en/ksystemlog"),
                     QString());

    about.setOrganizationDomain("kde.org");

    about.addAuthor(i18n("Nicolas Ternisien"),
                    i18n("Main developer"),
                    QStringLiteral("nicolas.ternisien@gmail.com"),
                    QStringLiteral("https://www.forum-software.org"));
    about.addAuthor(i18n("Vyacheslav Matyushin"), i18n("Journald mode, bugfixes"), QStringLiteral("v.matyushin@gmail.com"));
    about.addCredit(i18n("Bojan Djurkovic"), i18n("Log Printing"), QStringLiteral("dbojan@gmail.com"));
    about.addCredit(i18n("Laurent Montel"), i18n("Bug Fixing"), QStringLiteral("montel@kde.org"));

    KAboutData::setApplicationData(about);

    app.setApplicationDisplayName(about.displayName());

    QApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("utilities-log-viewer")));

    QCommandLineParser parser;
    about.setupCommandLine(&parser);
    parser.setApplicationDescription(about.shortDescription());

    // url to open
    parser.addPositionalArgument(QStringLiteral("URL"), i18n("Document to open."));

    // do the command line parsing
    parser.process(app);

    // handle standard options
    about.processCommandLine(&parser);

    // See if we are starting with session management
    if (app.isSessionRestored()) {
        kRestoreMainWindows<KSystemLog::MainWindow>();
    } else {
        // No session... Just start up normally

        const QStringList args = parser.positionalArguments();

        if (args.isEmpty()) {
            new KSystemLog::MainWindow();
        } else {
            // KSystemLog::MainWindow* mainWindow;
            new KSystemLog::MainWindow();
            for (int i = 0, total = args.count(); i < total; ++i) {
                logDebug() << "Loading file " << args.at(i);

                // TODO Implement this kind of loading
                // LogManager* firstLogManager = d->tabs->createTab();
                // d->tabs->load(Globals::instance().findLogMode("openLogMode"), firstLogManager);
                // Open log mode need to automatically find the passed url : args->url(i)
            }
        }
    }

    return app.exec();
}
