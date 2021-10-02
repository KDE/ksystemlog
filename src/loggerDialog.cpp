/*
    KSystemLog, a system log viewer tool
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "loggerDialog.h"

#include <QProcess>

#include <KLocalizedString>
#include <KMessageBox>
#include <QDesktopServices>

// Project includes
#include "logging.h"

#include "globals.h"
#include "logLevel.h"

LoggerDialog::LoggerDialog(QWidget *parent)
    : QDialog(parent)
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
    connect(loggerManual, &QLabel::linkActivated, this, &LoggerDialog::slotLinkClicked);
    buildMaps();

    // Fill the priority ComboBox
    QList<QString> prioKeys(mPriorities.keys());

    QList<QString>::Iterator itPriority;
    for (itPriority = prioKeys.begin(); itPriority != prioKeys.end(); ++itPriority) {
        priority->addItem(mPriorityIcons[*itPriority], *itPriority);
    }

    // Select the right priority
    for (int i = 0; i < priority->count(); ++i) {
        if (priority->itemText(i) == Globals::instance().noticeLogLevel()->name()) {
            priority->setCurrentIndex(i);
            break;
        }
    }

    // Fill the priority ComboBox
    QList<QString> keys(mFacilities.keys());

    QList<QString>::Iterator itFacility;
    for (itFacility = keys.begin(); itFacility != keys.end(); ++itFacility) {
        facility->addItem(*itFacility);
    }

    // Select the right facility
    for (int i = 0, total = facility->count(); i < total; ++i) {
        if (facility->itemText(i) == i18n("User")) {
            facility->setCurrentIndex(i);
            break;
        }
    }
    tag->setEnabled(false);
}

LoggerDialog::~LoggerDialog()
{
}

void LoggerDialog::slotLinkClicked(const QString &link)
{
    QDesktopServices::openUrl(QUrl::fromUserInput(link));
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
    mFacilities[i18n("Authentication")] = QStringLiteral("auth");
    mFacilities[i18n("Private Authentication")] = QStringLiteral("authpriv");
    mFacilities[i18n("Cron")] = QStringLiteral("cron");
    mFacilities[i18n("Daemon")] = QStringLiteral("daemon");
    mFacilities[i18n("FTP")] = QStringLiteral("ftp");
    mFacilities[i18n("Kernel")] = QStringLiteral("kern");
    mFacilities[i18n("LPR")] = QStringLiteral("lpr");
    mFacilities[i18n("Mail")] = QStringLiteral("mail");
    mFacilities[i18n("News")] = QStringLiteral("news");
    mFacilities[i18n("Syslog")] = QStringLiteral("syslog");
    mFacilities[i18n("User")] = QStringLiteral("user");
    mFacilities[i18n("UUCP")] = QStringLiteral("uucp");

    mFacilities[i18n("Local 0")] = QStringLiteral("local0");
    mFacilities[i18n("Local 1")] = QStringLiteral("local1");
    mFacilities[i18n("Local 2")] = QStringLiteral("local2");
    mFacilities[i18n("Local 3")] = QStringLiteral("local3");
    mFacilities[i18n("Local 4")] = QStringLiteral("local4");
    mFacilities[i18n("Local 5")] = QStringLiteral("local5");
    mFacilities[i18n("Local 6")] = QStringLiteral("local6");
    mFacilities[i18n("Local 7")] = QStringLiteral("local7");

    // Fill the priority map
    mPriorities[Globals::instance().debugLogLevel()->name()] = QStringLiteral("debug");
    mPriorities[Globals::instance().informationLogLevel()->name()] = QStringLiteral("info");
    mPriorities[Globals::instance().noticeLogLevel()->name()] = QStringLiteral("notice");
    mPriorities[Globals::instance().warningLogLevel()->name()] = QStringLiteral("warning");
    mPriorities[Globals::instance().errorLogLevel()->name()] = QStringLiteral("err");
    mPriorities[Globals::instance().criticalLogLevel()->name()] = QStringLiteral("crit");
    mPriorities[Globals::instance().alertLogLevel()->name()] = QStringLiteral("alert");
    mPriorities[Globals::instance().emergencyLogLevel()->name()] = QStringLiteral("emerg");

    // Fill the priority icon map
    mPriorityIcons[Globals::instance().debugLogLevel()->name()] = QIcon::fromTheme(Globals::instance().debugLogLevel()->icon().name());
    mPriorityIcons[Globals::instance().informationLogLevel()->name()] = QIcon::fromTheme(Globals::instance().informationLogLevel()->icon().name());
    mPriorityIcons[Globals::instance().noticeLogLevel()->name()] = QIcon::fromTheme(Globals::instance().noticeLogLevel()->icon().name());
    mPriorityIcons[Globals::instance().warningLogLevel()->name()] = QIcon::fromTheme(Globals::instance().warningLogLevel()->icon().name());
    mPriorityIcons[Globals::instance().errorLogLevel()->name()] = QIcon::fromTheme(Globals::instance().errorLogLevel()->icon().name());
    mPriorityIcons[Globals::instance().criticalLogLevel()->name()] = QIcon::fromTheme(Globals::instance().criticalLogLevel()->icon().name());
    mPriorityIcons[Globals::instance().alertLogLevel()->name()] = QIcon::fromTheme(Globals::instance().alertLogLevel()->icon().name());
    mPriorityIcons[Globals::instance().emergencyLogLevel()->name()] = QIcon::fromTheme(Globals::instance().emergencyLogLevel()->icon().name());
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

    const QString prioritySelected = priority->currentText();

    if (prioritySelected != Globals::instance().noLogLevel()->name()) {
        arguments << QStringLiteral("-p");

        QString p(mFacilities[facility->currentText()]);
        p += QLatin1Char('.');
        p += mPriorities[priority->currentText()];

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
        KMessageBox::error(this,
                           i18n("Unable to find the 'logger' command on your system. Please type "
                                "'logger' in a Konsole to determine whether this command is installed."),
                           i18n("Command not found"));
        return;
    }

    if (process.exitStatus() == QProcess::CrashExit) {
        KMessageBox::error(this, i18n("The 'logger' command has not been properly exited."), i18n("Execution problem"));
        return;
    }

    // No such file or directory
    if (process.exitCode() == 1) {
        KMessageBox::error(this, i18n("This file does not exist, please choose another."), i18n("File not valid"));
        return;
    }

    // Hide the Logger Dialog
    hide();
}
