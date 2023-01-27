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

        parsingHelper.h
        fileList.h
        logLevelFileList.h
        multipleFileList.h
        fileListHelper.h
        logLevelSelectionDialog.h
        genericConfiguration.h
        syslogAnalyzer.h
    )

ki18n_wrap_ui(ksystemlog_base_mode fileListBase.ui )
ki18n_wrap_ui(ksystemlog_base_mode multipleFileListBase.ui )

ki18n_wrap_ui(ksystemlog_base_mode logLevelSelectionDialogBase.ui )


add_dependencies(
	ksystemlog_base_mode 
	ksystemlog_lib
)

target_link_libraries(
	ksystemlog_base_mode
	
	ksystemlog_lib
	ksystemlog_config

        KF${KF_MAJOR_VERSION}::XmlGui
)
