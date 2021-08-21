add_library(ksystemlog_base_mode STATIC)

target_sources(ksystemlog_base_mode PRIVATE
	parsingHelper.cpp
	fileList.cpp
	logLevelFileList.cpp
	multipleFileList.cpp
	fileListHelper.cpp
	logLevelSelectionDialog.cpp
	genericConfiguration.cpp
	syslogAnalyzer.cpp
)

ki18n_wrap_ui(ksystemlog_base_mode_sources fileListBase.ui )
ki18n_wrap_ui(ksystemlog_base_mode_sources multipleFileListBase.ui )

ki18n_wrap_ui(ksystemlog_base_mode_sources logLevelSelectionDialogBase.ui )


add_dependencies(
	ksystemlog_base_mode 
	ksystemlog_lib
)

target_link_libraries(
	ksystemlog_base_mode
	
	ksystemlog_lib
	ksystemlog_config

        KF5::XmlGui
)
