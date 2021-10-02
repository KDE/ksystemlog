/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "logModeConfiguration.h"

#include "defaults.h"
#include "ksystemlog_debug.h"

#include <KLocalizedString>

#include "logLevel.h"

#include "globals.h"

#include "ksystemlogConfig.h"

LogModeConfiguration::LogModeConfiguration(QObject *parent)
    : QObject(parent)
{
    mConfiguration = KSystemLogConfig::self();
}

LogModeConfiguration::~LogModeConfiguration()
{
    // configuration is managed by KDE
}

// TODO Need a rewritting
/*
Reader* ReaderFactory::createReader(LogMode* logMode) {

    //Using Boot Mode in the current view
    else if (logMode==Globals::bootMode) {
        Reader* reader=new SystemReader(NULL, "boot_reader");
        return(reader);
    }

    //Using Authentication Mode in the current view
    else if (logMode==Globals::authenticationMode) {
        Reader* reader=new SystemReader(NULL, "authentication_reader");
        return(reader);
    }

    //Using Daemon Mode in the current view
    else if (logMode==Globals::daemonMode) {
        Reader* reader=new SystemReader(NULL, "daemon_reader");
        return(reader);
    }

    //Using Cups Mode in the current view
    else if (logMode==Globals::cupsMode) {
        Reader* reader=new CupsReader(NULL, "cups_reader");
        return(reader);
    }

    //Using Cups Access Mode in the current view
    else if (logMode==Globals::cupsAccessMode) {
        Reader* reader=new CupsAccessReader(NULL, "cups_access_reader");
        return(reader);
    }

    //Using Postfix Mode in the current view
    else if (logMode==Globals::postfixMode) {
        Reader* reader=new SystemReader(NULL, "postfix_reader");
        return(reader);
    }

    //Using Samba Mode in the current view
    else if (logMode==Globals::sambaMode) {
        Reader* reader=new SambaReader(NULL, "samba_reader");
        return(reader);
    }

    //Using SSH Mode in the current view
    else if (logMode==Globals::sshMode) {
        Reader* reader=new SSHReader(NULL, "ssh_reader");
        return(reader);
    }

    //Using X Session Mode in the current view
    else if (logMode==Globals::xsessionMode) {
        Reader* reader=new XSessionReader(NULL, "xsession_reader");
        return reader;
    }

  qCCritical(KSYSTEMLOG) << "LogMode not found : returns NULL Reader";
    return NULL;
}
*/

// TODO Move this method in LogModeFactory subclasses
/*
QVector<LogFile> LogFilesFactory::createLogFiles(LogMode* logMode) {

    else if (logMode==Globals::instance().bootMode()) {
        QVector<LogFile> list;
        list.append(LogFilesFactory::instance().getBootLogFile());
        return list;
    }

    else if (logMode==Globals::instance().authenticationMode()) {
        QVector<LogFile> list;
        list.append(LogFilesFactory::instance().getAuthenticationLogFile());
        return list;
    }

    else if (logMode==Globals::instance().daemonMode()) {
        return LogFilesFactory::getDaemonLogFiles();
    }

    else if (logMode==Globals::instance().cupsMode()) {
        return LogFilesFactory::getCupsLogFiles();
    }

    else if (logMode==Globals::instance().cupsAccessMode()) {
        return LogFilesFactory::getCupsAccessLogFiles();

    }

    else if (logMode==Globals::instance().postfixMode()) {
        return LogFilesFactory::getPostfixLogFiles();
    }

    else if (logMode==Globals::instance().sambaMode()) {
        return LogFilesFactory::getSambaLogFiles();
    }

    else if (logMode==Globals::instance().sshMode()) {
        return LogFilesFactory::getSSHLogFiles();
    }


    else if (logMode==Globals::instance().xsessionMode()) {
        return LogFilesFactory::getXSessionLogFiles();
    }

  logError() << "LogFiles not found : returns NULL Reader";

    return QVector<LogFile>();

}


LogFile LogFilesFactory::getBootLogFile() {
    QString file=KSystemLogConfig::bootPath();
    return getGenericLogFile(file);
}

LogFile LogFilesFactory::getAuthenticationLogFile() {
    QString file=KSystemLogConfig::authenticationPath();
    return getGenericLogFile(file);
}

QVector<LogFile> LogFilesFactory::getDaemonLogFiles() {
    QStringList files=KSystemLogConfig::daemonPaths();
    QList<int> levels=KSystemLogConfig::daemonLevels();
    return LogFilesFactory::getGenericLogFiles(files, levels);
}

QVector<LogFile> LogFilesFactory::getCupsLogFiles() {
    QStringList stringList=KSystemLogConfig::cupsPaths();
    return getNoModeLogFiles(stringList);
}

QVector<LogFile> LogFilesFactory::getCupsAccessLogFiles() {
    QStringList stringList=KSystemLogConfig::cupsAccessPaths();
    return getNoModeLogFiles(stringList);
}

QVector<LogFile> LogFilesFactory::getPostfixLogFiles() {
    QStringList files=KSystemLogConfig::postfixPaths();
    QList<int> levels=KSystemLogConfig::postfixLevels();
    return LogFilesFactory::getGenericLogFiles(files, levels);
}

QVector<LogFile> LogFilesFactory::getSambaLogFiles() {
    QStringList stringList=KSystemLogConfig::sambaPaths();
    return getNoModeLogFiles(stringList);
}

QVector<LogFile> LogFilesFactory::getSSHLogFiles() {
    QStringList stringList=KSystemLogConfig::sshPaths();
    return getNoModeLogFiles(stringList);
}

QVector<LogFile> LogFilesFactory::getXSessionLogFiles() {
    QStringList stringList=KSystemLogConfig::xSessionPaths();
    return getNoModeLogFiles(stringList);
}

*/

LogFile LogModeConfiguration::findGenericLogFile(const QString &file)
{
    LogLevel *level = Globals::instance().informationLogLevel();

    const QUrl url = QUrl::fromLocalFile(file);
    if (!url.isValid()) {
        qCWarning(KSYSTEMLOG) << i18n("URL '%1' is not valid, skipping this URL.", url.path());
        return LogFile(QUrl(), Globals::instance().noLogLevel());
    }
    return LogFile(url, level);
}

QVector<LogFile> LogModeConfiguration::findGenericLogFiles(const QStringList &files)
{
    QVector<LogFile> logFiles;

    logFiles.reserve(files.count());
    for (const QString &file : files) {
        logFiles.append(findGenericLogFile(file));
    }

    return logFiles;
}

QVector<LogFile> LogModeConfiguration::findNoModeLogFiles(const QStringList &stringList)
{
    QVector<LogFile> logFiles;

    // Default level used for No Mode logs
    LogLevel *level = Globals::instance().noLogLevel();

    for (const QString &string : stringList) {
        const QUrl url = QUrl::fromLocalFile(string);
        if (!url.isValid()) {
            qCWarning(KSYSTEMLOG) << i18n("URL '%1' is not valid, skipping this URL.", url.path());
            continue;
        }

        logFiles.append(LogFile(url, level));
    }

    return logFiles;
}
