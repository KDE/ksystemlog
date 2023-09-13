
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_BINARY_DIR}/../config
)
add_library(ksystemlog_lib STATIC ${ksystemlog_lib_sources})
target_sources(ksystemlog_lib PRIVATE
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

    analyzer.h
    fileAnalyzer.h
    logModeFactory.h
    logModeConfiguration.h
    logModeConfigurationWidget.h
    logModeItemBuilder.h
    logManager.h
    view.h
    globals.h
    logMode.h
    logLevel.h
    logFile.h
    logFileReader.h
    localLogFileReader.h
    processOutputLogFileReader.h
    kioLogFileReader.h
    logLine.h
    logViewModel.h
    logViewColumn.h
    logViewColumns.h
    logViewWidgetItem.h
    logViewWidget.h
    logViewExport.h
    logViewFilterWidget.h
    logViewSearchWidget.h
    loadingBar.h
    logModeAction.h
    simpleAction.h
    multipleActions.h
    levelPrintPage.h
)
ecm_qt_declare_logging_category(ksystemlog_lib HEADER ksystemlog_debug.h IDENTIFIER KSYSTEMLOG CATEGORY_NAME log_ksystemlog DESCRIPTION "ksystemlog" EXPORT KSYSTEMLOG)

ki18n_wrap_ui(ksystemlog_lib logViewSearchWidgetBase.ui)



add_dependencies(
    ksystemlog_lib
    ksystemlog_config
)

target_link_libraries(ksystemlog_lib
    ksystemlog_config
    Qt6::Widgets
    Qt6::PrintSupport
    KF6::I18n
    KF6::Archive
    KF6::WidgetsAddons
    KF6::ItemViews
    KF6::KIOCore
    KF6::Completion
    KF6::ConfigWidgets
)
