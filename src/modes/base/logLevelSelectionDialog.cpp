/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   SPDX-FileCopyrightText: 2007 Nicolas Ternisien *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 ***************************************************************************/

#include "logLevelSelectionDialog.h"

#include <QPushButton>

#include <KStandardGuiItem>

LogLevelSelectionDialog::LogLevelSelectionDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setIcon(KStandardGuiItem::ok().icon());

    QPushButton *cancelButton = buttonBox->button(QDialogButtonBox::Cancel);
    cancelButton->setIcon(KStandardGuiItem::cancel().icon());
}

LogLevelSelectionDialog::~LogLevelSelectionDialog()
{
}

QListWidget *LogLevelSelectionDialog::logLevels() const
{
    return logLevelsList;
}
