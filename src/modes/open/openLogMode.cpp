/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "openLogMode.h"

#include <QFileDialog>
#include <QString>

#include <KLocalizedString>

#include <KMessageBox>

#include "ksystemlog_debug.h"

#include "logModeItemBuilder.h"
#include "openAnalyzer.h"

OpenLogMode::OpenLogMode(QWidget *parent)
    : LogMode(QStringLiteral(OPEN_LOG_MODE_ID), i18n("Log File"), QStringLiteral(OPEN_MODE_ICON))
    , mParent(parent)
{
    d->logModeConfigurationWidget = nullptr;

    d->itemBuilder = new LogModeItemBuilder();

    d->action = nullptr;
}

OpenLogMode::~OpenLogMode()
{
}

Analyzer *OpenLogMode::createAnalyzer(const QVariant &options)
{
    Q_UNUSED(options)
    return new OpenAnalyzer(this);
}

QVector<LogFile> OpenLogMode::createLogFiles()
{
    // Open a standard Filedialog
    const QUrl openingFileName(QFileDialog::getOpenFileUrl(mParent, i18n("Open Location"), QUrl(), QString()));
    qCDebug(KSYSTEMLOG) << "Opening file : " << openingFileName.url();

    if (openingFileName.isEmpty()) {
        return QVector<LogFile>();
    }

    if (openingFileName.isValid()) {
        const LogFile logFile(openingFileName, Globals::instance().informationLogLevel());
        const QVector<LogFile> logFiles{logFile};

        return logFiles;
    }

    const QString message(i18n("Malformed URL. Unable to open this file."));
    KMessageBox::error(mParent, message, i18n("Unable to open this file."), KMessageBox::Notify);

    return QVector<LogFile>();
}
