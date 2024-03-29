/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "journaldLogMode.h"

#include <QAction>

#include <KLocalizedString>

#include "ksystemlog_debug.h"

#include "journaldConfiguration.h"
#include "journaldConfigurationWidget.h"
#include "journaldItemBuilder.h"
#include "journaldLocalAnalyzer.h"
#include "journaldNetworkAnalyzer.h"

JournaldLogMode::JournaldLogMode()
    : LogMode(QLatin1String(JOURNALD_LOG_MODE_ID), i18n("Journald Log"), QLatin1String(JOURNALD_MODE_ICON))
{
    d->logModeConfiguration = QSharedPointer<JournaldConfiguration>(new JournaldConfiguration());

    d->logModeConfigurationWidget = new JournaldConfigurationWidget();
    connect(qobject_cast<JournaldConfigurationWidget *>(d->logModeConfigurationWidget),
            &JournaldConfigurationWidget::configSaved,
            this,
            &JournaldLogMode::menuChanged);

    d->itemBuilder = new JournaldItemBuilder();

    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the Journald log."));
    d->action->setWhatsThis(i18n("Displays the journald log in the current tab."));
}

Analyzer *JournaldLogMode::createAnalyzer(const QVariant &analyzerOptions)
{
    JournaldAnalyzerOptions options = analyzerOptions.value<JournaldAnalyzerOptions>();
    switch (options.analyzerType) {
    case JournaldAnalyzerType::Local:
        return new JournaldLocalAnalyzer(this, options.filter);
    case JournaldAnalyzerType::Network:
        return new JournaldNetworkAnalyzer(this, options);
    }

    return new JournaldLocalAnalyzer(this);
}

QVector<LogFile> JournaldLogMode::createLogFiles()
{
    // No log file for journald.
    return QVector<LogFile>();
}

void JournaldLogMode::updateJournalFilters(const JournalAddress &address, const JournalFilters &filters)
{
    mRemoteJournalFilters[address] = filters;
}

JournalFilters JournaldLogMode::filters(const JournalAddress &address) const
{
    return mRemoteJournalFilters[address];
}

#include "moc_journaldLogMode.cpp"
