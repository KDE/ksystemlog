
include_directories( 
	${CMAKE_CURRENT_SOURCE_DIR}/../..
	${CMAKE_CURRENT_SOURCE_DIR}/../base
	${CMAKE_CURRENT_SOURCE_DIR}/../../lib
	${CMAKE_CURRENT_SOURCE_DIR}/../../config 
        ${CMAKE_CURRENT_BINARY_DIR}/../base
        ${CMAKE_CURRENT_BINARY_DIR}/../../config
)

set(ksystemlog_xsession_sources
	xsessionConfigurationWidget.cpp
	xsessionConfiguration.cpp
	xsessionAnalyzer.cpp
	xsessionItemBuilder.cpp
	xsessionLogMode.cpp
	xsessionFactory.cpp
)

ki18n_wrap_ui(ksystemlog_xsession_sources xsessionConfigurationWidgetBase.ui )

add_library(ksystemlog_xsession STATIC ${ksystemlog_xsession_sources})

add_dependencies(
	ksystemlog_xsession
	ksystemlog_base_mode 
	ksystemlog_lib
)

target_link_libraries(
    ksystemlog_xsession
	
	ksystemlog_lib
	ksystemlog_base_mode 
	ksystemlog_config

        KF5::KIOWidgets
)
