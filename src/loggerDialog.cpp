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

#include "loggerDialog.h"

#include <QProcess>

#include <KLocalizedString>
#include <kmessagebox.h>

// Project includes
#include "logging.h"

#include "logLevel.h"
#include "globals.h"

class LoggerDialogPrivate
{
public:
    QMap<QString, QString> facilities;

    QMap<QString, QString> priorities;
    QMap<QString, QPixmap> priorityIcons;
};

LoggerDialog::LoggerDialog(QWidget *parent)
    : QDialog(parent)
    , d(new LoggerDialogPrivate())
{
    setupUi(this);

    connect(buttonOK, SIGNAL(clicked()), this, SLOT(sendMessage()));
    connect(buttonCancel, SIGNAL(clicked()), this, SLOT(hide()));

    connect(tagActivation, SIGNAL(toggled(bool)), this, SLOT(changeTagActivation(bool)));
    connect(fileActivation, SIGNAL(toggled(bool)), this, SLOT(changeFileActivation(bool)));
    connect(messageActivation, SIGNAL(toggled(bool)), this, SLOT(changeMessageActivation(bool)));

    connect(file, SIGNAL(textChanged(QString)), this, SLOT(textChanged()));
    connect(message, SIGNAL(textChanged(QString)), this, SLOT(textChanged()));
    connect(tag, SIGNAL(textChanged(QString)), this, SLOT(textChanged()));

    buildMaps();

    // Fill the priority ComboBox
    QList<QString> prioKeys(d->priorities.keys());

    QList<QString>::Iterator itPriority;
    for (itPriority = prioKeys.begin(); itPriority != prioKeys.end(); ++itPriority) {
        priority->addItem(d->priorityIcons[*itPriority], *itPriority);
    }

    // Select the right priority
    for (int i = 0; i < priority->count(); ++i) {
        if (priority->itemText(i) == Globals::instance()->noticeLogLevel()->name()) {
            priority->setCurrentIndex(i);
            break;
        }
    }

    // Fill the priority ComboBox
    QList<QString> keys(d->facilities.keys());

    QList<QString>::Iterator itFacility;
    for (itFacility = keys.begin(); itFacility != keys.end(); ++itFacility) {
        facility->addItem(*itFacility);
    }

    // Select the right facility
    for (int i = 0; i < facility->count(); ++i) {
        if (facility->itemText(i) == i18n("User")) {
            facility->setCurrentIndex(i);
            break;
        }
    }
    tag->setEnabled(false);
}

LoggerDialog::~LoggerDialog()
{
    delete d;
}

void LoggerDialog::initialize()
{
    logDebug() << "Initializing Logger dialog...";

    message->clear();
    message->setFocus();
}

void LoggerDialog::buildMaps()
{
    // Fill the facility map
    d->facilities[i18n("Authentication")] = QLatin1String("auth");
    d->facilities[i18n("Private Authentication")] = QLatin1String("authpriv");
    d->facilities[i18n("Cron")] = QLatin1String("cron");
    d->facilities[i18n("Daemon")] = QLatin1String("daemon");
    d->facilities[i18n("FTP")] = QLatin1String("ftp");
    d->facilities[i18n("Kernel")] = QLatin1String("kern");
    d->facilities[i18n("LPR")] = QLatin1String("lpr");
    d->facilities[i18n("Mail")] = QLatin1String("mail");
    d->facilities[i18n("News")] = QLatin1String("news");
    d->facilities[i18n("Syslog")] = QLatin1String("syslog");
    d->facilities[i18n("User")] = QLatin1String("user");
    d->facilities[i18n("UUCP")] = QLatin1String("uucp");

    d->facilities[i18n("Local 0")] = QLatin1String("local0");
    d->facilities[i18n("Local 1")] = QLatin1String("local1");
    d->facilities[i18n("Local 2")] = QLatin1String("local2");
    d->facilities[i18n("Local 3")] = QLatin1String("local3");
    d->facilities[i18n("Local 4")] = QLatin1String("local4");
    d->facilities[i18n("Local 5")] = QLatin1String("local5");
    d->facilities[i18n("Local 6")] = QLatin1String("local6");
    d->facilities[i18n("Local 7")] = QLatin1String("local7");

    // Fill the priority map
    d->priorities[Globals::instance()->debugLogLevel()->name()] = QLatin1String("debug");
    d->priorities[Globals::instance()->informationLogLevel()->name()] = QLatin1String("info");
    d->priorities[Globals::instance()->noticeLogLevel()->name()] = QLatin1String("notice");
    d->priorities[Globals::instance()->warningLogLevel()->name()] = QLatin1String("warning");
    d->priorities[Globals::instance()->errorLogLevel()->name()] = QLatin1String("err");
    d->priorities[Globals::instance()->criticalLogLevel()->name()] = QLatin1String("crit");
    d->priorities[Globals::instance()->alertLogLevel()->name()] = QLatin1String("alert");
    d->priorities[Globals::instance()->emergencyLogLevel()->name()] = QLatin1String("emerg");

    // Fill the priority icon map
    d->priorityIcons[Globals::instance()->debugLogLevel()->name()]
        = Globals::instance()->debugLogLevel()->icon();
    d->priorityIcons[Globals::instance()->informationLogLevel()->name()]
        = Globals::instance()->informationLogLevel()->icon();
    d->priorityIcons[Globals::instance()->noticeLogLevel()->name()]
        = Globals::instance()->noticeLogLevel()->icon();
    d->priorityIcons[Globals::instance()->warningLogLevel()->name()]
        = Globals::instance()->warningLogLevel()->icon();
    d->priorityIcons[Globals::instance()->errorLogLevel()->name()]
        = Globals::instance()->errorLogLevel()->icon();
    d->priorityIcons[Globals::instance()->criticalLogLevel()->name()]
        = Globals::instance()->criticalLogLevel()->icon();
    d->priorityIcons[Globals::instance()->alertLogLevel()->name()]
        = Globals::instance()->alertLogLevel()->icon();
    d->priorityIcons[Globals::instance()->emergencyLogLevel()->name()]
        = Globals::instance()->emergencyLogLevel()->icon();
}

void LoggerDialog::textChanged()
{
    if (fileActivation->isChecked() && file->url().isEmpty()) {
        buttonOK->setEnabled(false);
        return;
    }

    if (tagActivation->isChecked() && tag->text().isEmpty()) {
        buttonOK->setEnabled(false);
        return;
    }

    if (messageActivation->isChecked() && message->text().isEmpty()) {
        buttonOK->setEnabled(false);
        return;
    }

    buttonOK->setEnabled(true);
}

void LoggerDialog::changeTagActivation(bool activation)
{
    tag->setEnabled(activation);

    textChanged();
}

void LoggerDialog::changeFileActivation(bool activation)
{
    file->setEnabled(activation);

    textChanged();
}

void LoggerDialog::changeMessageActivation(bool activation)
{
    message->setEnabled(activation);

    textChanged();
}

void LoggerDialog::sendMessage()
{
    QProcess process;

    QStringList arguments;

    if (useProcessIdentifier->isChecked()) {
        arguments << QLatin1String("-i");
    }

    if (tagActivation->isChecked()) {
        arguments << QLatin1String("-t");

        arguments << tag->text();
    }

    QString prioritySelected = priority->currentText();

    if (prioritySelected != Globals::instance()->noLogLevel()->name()) {
        arguments << QLatin1String("-p");

        QString p(d->facilities[facility->currentText()]);
        p += QLatin1Char('.');
        p += d->priorities[priority->currentText()];

        arguments << p;
    }

    // If we read the content of a file
    if (fileActivation->isChecked()) {
        arguments << QLatin1String("-f");

        arguments << file->url().path();
    }
    // Else, the user types the content of its message
    else {
        // Remove bad "\n" characters
        arguments << message->text().replace(QLatin1String("\n"), QLatin1String(" "));
    }

    // QProcess::Block, QProcess::Stdout
    process.start(QLatin1String("logger"), arguments);

    // If the launching of the command failed
    if (process.error() == QProcess::FailedToStart) {
        KMessageBox::error(this, i18n(
                                     "Unable to find the 'logger' command on your system. Please type "
                                     "'logger' in a Konsole to determine whether this command is installed."),
                           i18n("Command not found"));
        return;
    }

    if (process.exitStatus() == QProcess::CrashExit) {
        KMessageBox::error(this, i18n("The 'logger' command has not been properly exited."),
                           i18n("Execution problem"));
        return;
    }

    // No such file or directory
    if (process.exitCode() == 1) {
        KMessageBox::error(this, i18n("This file does not exist, please choose another."),
                           i18n("File not valid"));
        return;
    }

    // Hide the Logger Dialog
    hide();
}
