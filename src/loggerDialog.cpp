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
#include <KMessageBox>

// Project includes
#include "logging.h"

#include "logLevel.h"
#include "globals.h"

class LoggerDialogPrivate
{
public:
    QMap<QString, QString> facilities;

    QMap<QString, QString> priorities;
    QMap<QString, QIcon> priorityIcons;
};

LoggerDialog::LoggerDialog(QWidget *parent)
    : QDialog(parent)
    , d(new LoggerDialogPrivate())
{
    setupUi(this);

    connect(buttonOK, &QAbstractButton::clicked, this, &LoggerDialog::sendMessage);
    connect(buttonCancel, &QAbstractButton::clicked, this, &QWidget::hide);

    connect(tagActivation, &QAbstractButton::toggled, this, &LoggerDialog::changeTagActivation);
    connect(fileActivation, &QAbstractButton::toggled, this, &LoggerDialog::changeFileActivation);
    connect(messageActivation, &QAbstractButton::toggled, this, &LoggerDialog::changeMessageActivation);

    connect(file, &KUrlRequester::textChanged, this, &LoggerDialog::textChanged);
    connect(message, &QLineEdit::textChanged, this, &LoggerDialog::textChanged);
    connect(tag, &QLineEdit::textChanged, this, &LoggerDialog::textChanged);

    buildMaps();

    // Fill the priority ComboBox
    QList<QString> prioKeys(d->priorities.keys());

    QList<QString>::Iterator itPriority;
    for (itPriority = prioKeys.begin(); itPriority != prioKeys.end(); ++itPriority) {
        priority->addItem(d->priorityIcons[*itPriority], *itPriority);
    }

    // Select the right priority
    for (int i = 0; i < priority->count(); ++i) {
        if (priority->itemText(i) == Globals::instance().noticeLogLevel()->name()) {
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
    d->facilities[i18n("Authentication")] = QStringLiteral("auth");
    d->facilities[i18n("Private Authentication")] = QStringLiteral("authpriv");
    d->facilities[i18n("Cron")] = QStringLiteral("cron");
    d->facilities[i18n("Daemon")] = QStringLiteral("daemon");
    d->facilities[i18n("FTP")] = QStringLiteral("ftp");
    d->facilities[i18n("Kernel")] = QStringLiteral("kern");
    d->facilities[i18n("LPR")] = QStringLiteral("lpr");
    d->facilities[i18n("Mail")] = QStringLiteral("mail");
    d->facilities[i18n("News")] = QStringLiteral("news");
    d->facilities[i18n("Syslog")] = QStringLiteral("syslog");
    d->facilities[i18n("User")] = QStringLiteral("user");
    d->facilities[i18n("UUCP")] = QStringLiteral("uucp");

    d->facilities[i18n("Local 0")] = QStringLiteral("local0");
    d->facilities[i18n("Local 1")] = QStringLiteral("local1");
    d->facilities[i18n("Local 2")] = QStringLiteral("local2");
    d->facilities[i18n("Local 3")] = QStringLiteral("local3");
    d->facilities[i18n("Local 4")] = QStringLiteral("local4");
    d->facilities[i18n("Local 5")] = QStringLiteral("local5");
    d->facilities[i18n("Local 6")] = QStringLiteral("local6");
    d->facilities[i18n("Local 7")] = QStringLiteral("local7");

    // Fill the priority map
    d->priorities[Globals::instance().debugLogLevel()->name()] = QStringLiteral("debug");
    d->priorities[Globals::instance().informationLogLevel()->name()] = QStringLiteral("info");
    d->priorities[Globals::instance().noticeLogLevel()->name()] = QStringLiteral("notice");
    d->priorities[Globals::instance().warningLogLevel()->name()] = QStringLiteral("warning");
    d->priorities[Globals::instance().errorLogLevel()->name()] = QStringLiteral("err");
    d->priorities[Globals::instance().criticalLogLevel()->name()] = QStringLiteral("crit");
    d->priorities[Globals::instance().alertLogLevel()->name()] = QStringLiteral("alert");
    d->priorities[Globals::instance().emergencyLogLevel()->name()] = QStringLiteral("emerg");

    // Fill the priority icon map
    d->priorityIcons[Globals::instance().debugLogLevel()->name()]
        = QIcon::fromTheme(Globals::instance().debugLogLevel()->icon().name());
    d->priorityIcons[Globals::instance().informationLogLevel()->name()]
        = QIcon::fromTheme(Globals::instance().informationLogLevel()->icon().name());
    d->priorityIcons[Globals::instance().noticeLogLevel()->name()]
        = QIcon::fromTheme(Globals::instance().noticeLogLevel()->icon().name());
    d->priorityIcons[Globals::instance().warningLogLevel()->name()]
        = QIcon::fromTheme(Globals::instance().warningLogLevel()->icon().name());
    d->priorityIcons[Globals::instance().errorLogLevel()->name()]
        = QIcon::fromTheme(Globals::instance().errorLogLevel()->icon().name());
    d->priorityIcons[Globals::instance().criticalLogLevel()->name()]
        = QIcon::fromTheme(Globals::instance().criticalLogLevel()->icon().name());
    d->priorityIcons[Globals::instance().alertLogLevel()->name()]
        = QIcon::fromTheme(Globals::instance().alertLogLevel()->icon().name());
    d->priorityIcons[Globals::instance().emergencyLogLevel()->name()]
        = QIcon::fromTheme(Globals::instance().emergencyLogLevel()->icon().name());
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
        arguments << QStringLiteral("-i");
    }

    if (tagActivation->isChecked()) {
        arguments << QStringLiteral("-t");

        arguments << tag->text();
    }

    QString prioritySelected = priority->currentText();

    if (prioritySelected != Globals::instance().noLogLevel()->name()) {
        arguments << QStringLiteral("-p");

        QString p(d->facilities[facility->currentText()]);
        p += QLatin1Char('.');
        p += d->priorities[priority->currentText()];

        arguments << p;
    }

    // If we read the content of a file
    if (fileActivation->isChecked()) {
        arguments << QStringLiteral("-f");

        arguments << file->url().toLocalFile();
    }
    // Else, the user types the content of its message
    else {
        // Remove bad "\n" characters
        arguments << message->text().replace(QLatin1String("\n"), QLatin1String(" "));
    }

    // QProcess::Block, QProcess::Stdout
    process.start(QStringLiteral("logger"), arguments);

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
