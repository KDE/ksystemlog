
include_directories( 
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${CMAKE_CURRENT_BINARY_DIR}/../config
)

set(ksystemlog_lib_sources
	analyzer.cpp
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

ki18n_wrap_ui(ksystemlog_lib_sources logViewSearchWidgetBase.ui)

add_library(ksystemlog_lib STATIC ${ksystemlog_lib_sources})

add_dependencies(
	ksystemlog_lib 
	ksystemlog_config
)

target_link_libraries(ksystemlog_lib
	KF5::KDELibs4Support
	KF5::Archive
	ksystemlog_config
)
