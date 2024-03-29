/*
    SPDX-FileCopyrightText: 2007 Nicolas Ternisien <nicolas.ternisien@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "logViewFilterWidget.h"
#include "ksystemlog_debug.h"
#include "logLevel.h"
#include "logViewColumn.h"
#include "logViewWidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QString>

#include <QComboBox>
#include <QMetaEnum>
#include <QStandardItemModel>
#include <QStyledItemDelegate>

#include <KLocalizedString>

class ComboBoxDelegate : public QStyledItemDelegate
{
public:
    explicit ComboBoxDelegate(QWidget *parent)
        : QStyledItemDelegate(parent)
    {
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        auto &refToNonConstOption = const_cast<QStyleOptionViewItem &>(option);
        refToNonConstOption.showDecorationSelected = false;
        QStyledItemDelegate::paint(painter, refToNonConstOption, index);
    }
};

class LogViewFilterWidgetPrivate
{
public:
};

LogViewWidgetSearchLine::LogViewWidgetSearchLine(QWidget *parent)
    : KTreeWidgetSearchLine(parent)
{
    for (int i = 0; i < Globals::LOG_LEVEL_NUM; i++) {
        mPriorities[i] = true;
    }
}

LogViewWidgetSearchLine::~LogViewWidgetSearchLine()
{
}

void LogViewWidgetSearchLine::updateSearch(const QString &pattern)
{
    KTreeWidgetSearchLine::updateSearch(pattern);

    Q_EMIT treeWidgetUpdated();
}

void LogViewWidgetSearchLine::setPriorityEnabled(int priority, bool enabled)
{
    if ((priority < 0) || (priority >= Globals::LOG_LEVEL_NUM)) {
        return;
    }
    mPriorities[priority] = enabled;
    updateSearch(QString());
}

bool LogViewWidgetSearchLine::itemMatches(const QTreeWidgetItem *item, const QString &pattern) const
{
    // Hide item if its priority is not enabled.
    const int priority = item->data(0, Qt::UserRole).toInt();
    if ((priority >= 0) && (priority < Globals::LOG_LEVEL_NUM)) {
        if (!mPriorities[priority]) {
            return false;
        }
    }
    return KTreeWidgetSearchLine::itemMatches(item, pattern);
}

LogViewFilterWidget::LogViewFilterWidget(QWidget *parent)
    : QWidget(parent)
{
    auto filterBarLayout = new QHBoxLayout(this);
    filterBarLayout->setContentsMargins(0, 0, 0, 0);

    mFilterLine = new LogViewWidgetSearchLine();

    mFilterLine->setToolTip(i18n("Type your filter here"));
    mFilterLine->setWhatsThis(i18n("Allows you to only list items that match the content of this text."));
    mFilterLine->setPlaceholderText(i18n("Enter your search here..."));

    auto filterIcon = new QLabel();
    filterIcon->setPixmap(QIcon::fromTheme(QStringLiteral("view-filter")).pixmap(style()->pixelMetric(QStyle::PM_SmallIconSize)));
    filterIcon->setBuddy(mFilterLine);
    filterBarLayout->addWidget(filterIcon);

    auto filterLabel = new QLabel(i18n("Filter:"));
    filterLabel->setBuddy(mFilterLine);
    filterBarLayout->addWidget(filterLabel);

    filterBarLayout->addWidget(mFilterLine);

    initSearchListFilter();

    filterBarLayout->addWidget(mFilterList);

    mPrioritiesComboBox = new QComboBox(this);
    auto delegate = new ComboBoxDelegate(mPrioritiesComboBox);
    mPrioritiesComboBox->setItemDelegate(delegate);
    filterBarLayout->addWidget(mPrioritiesComboBox);

    QMetaEnum const &metaEnum = Globals::instance().logLevelsMetaEnum();

    mPrioritiesModel = new QStandardItemModel(mPrioritiesComboBox);
    mPrioritiesComboBox->setModel(mPrioritiesModel);

    auto item = new QStandardItem(i18n("Select priorities"));
    item->setSelectable(false);
    mPrioritiesModel->appendRow(item);
    connect(mPrioritiesModel, &QStandardItemModel::itemChanged, this, &LogViewFilterWidget::prioritiesChanged);

    // Don't add last enum value into combobox.
    for (int i = 0; i < metaEnum.keyCount() - 1; i++) {
        const int id = metaEnum.value(i);
        LogLevel *logLevel = Globals::instance().logLevelByPriority(id);

        auto item = new QStandardItem(logLevel->name());
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(Qt::Checked, Qt::CheckStateRole);
        item->setData(metaEnum.value(i), Qt::UserRole);
        item->setData(QVariant(logLevel->color()), Qt::ForegroundRole);

        mPrioritiesModel->appendRow(item);
    }
}

LogViewFilterWidget::~LogViewFilterWidget()
{
}

void LogViewFilterWidget::initSearchListFilter()
{
    mFilterList = new QComboBox();

    mFilterList->setToolTip(i18n("Choose the filtered column here"));
    mFilterList->setWhatsThis(
        i18n("Allows you to apply the item filter only on the specified column here. \"<i>All</i>\" column means "
             "no specific filter."));

    mFilterList->addItem(i18n("All"));

    mFilterList->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    connect(mFilterList, SIGNAL(activated(int)), mFilterLine, SLOT(setFocus()));
    connect(mFilterList, &QComboBox::activated, this, &LogViewFilterWidget::changeColumnFilter);
    connect(mFilterList, SIGNAL(activated(int)), mFilterLine, SLOT(updateSearch()));
}

void LogViewFilterWidget::updateFilterColumns(const LogViewColumns &columns)
{
    qCDebug(KSYSTEMLOG) << "Changing columns...";

    // We first delete all items
    mFilterList->clear();

    // Then we insert the default items
    mFilterList->addItem(i18n("All"));

    const auto cols = columns.columns();
    for (const LogViewColumn &column : cols) {
        if (column.isFiltered()) {
            mFilterList->addItem(column.columnName());
        }
    }

    mFilterList->setCurrentIndex(0);
}

void LogViewFilterWidget::changeColumnFilter(int column)
{
    // The user select all columns
    if (column == 0) {
        qCDebug(KSYSTEMLOG) << "Searching on all columns";

        mFilterLine->setSearchColumns(QList<int>());
        return;
    }

    qCDebug(KSYSTEMLOG) << "Searching on " << mFilterList->currentIndex() << " column";

    // currentIndex() - 1 to do not count the "All" columns item
    const QList<int> filterColumns{mFilterList->currentIndex() - 1};

    mFilterLine->setSearchColumns(filterColumns);
}

void LogViewFilterWidget::prioritiesChanged(QStandardItem *item)
{
    const int priority = item->data(Qt::UserRole).toInt();
    const bool priorityEnabled = (item->checkState() == Qt::Checked);
    mFilterLine->setPriorityEnabled(priority, priorityEnabled);
    if (priorityEnabled) {
        qCDebug(KSYSTEMLOG) << "Show entries with priority" << priority;
    } else {
        qCDebug(KSYSTEMLOG) << "Hide entries with priority" << priority;
    }
}

QComboBox *LogViewFilterWidget::filterList() const
{
    return mFilterList;
}

LogViewWidgetSearchLine *LogViewFilterWidget::filterLine() const
{
    return mFilterLine;
}

#include "moc_logViewFilterWidget.cpp"
