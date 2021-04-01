
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_BINARY_DIR}/../config
)

set(ksystemlog_lib_sources
    analyzer.cpp
    fileAnalyzer.cpp
    logModeFactory.cpp
    logModeConfiguration.cpp
    logModeConfigurationWidget.cpp
    logModeItemBuilder.cpp
    logManager.cpp
    view.cpp
    globals.cpp
    logMode.cpp
    logLevel.cpp
    logFile.cpp
    logFileReader.cpp
    localLogFileReader.cpp
    processOutputLogFileReader.cpp
    kioLogFileReader.cpp
    logLine.cpp
    logViewModel.cpp
    logViewColumn.cpp
    logViewColumns.cpp
    logViewWidgetItem.cpp
    logViewWidget.cpp
    logViewExport.cpp
    logViewFilterWidget.cpp
    logViewSearchWidget.cpp
    loadingBar.cpp
    logModeAction.cpp
    simpleAction.cpp
    multipleActions.cpp
    levelPrintPage.cpp
)
ecm_qt_declare_logging_category(ksystemlog_lib_sources HEADER ksystemlog_debug.h IDENTIFIER KSYSTEMLOG CATEGORY_NAME log_ksystemlog DESCRIPTION "ksystemlog" EXPORT KSYSTEMLOG)

ki18n_wrap_ui(ksystemlog_lib_sources logViewSearchWidgetBase.ui)

add_library(ksystemlog_lib STATIC ${ksystemlog_lib_sources})

add_dependencies(
    ksystemlog_lib
    ksystemlog_config
)

target_link_libraries(ksystemlog_lib
    ksystemlog_config
    Qt::Widgets
    Qt::PrintSupport
    KF5::I18n
    KF5::Archive
    KF5::WidgetsAddons
    KF5::ItemViews
    KF5::KIOCore
    KF5::Completion
)
