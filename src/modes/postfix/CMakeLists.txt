
include_directories( 
	${CMAKE_CURRENT_SOURCE_DIR}/../..
	${CMAKE_CURRENT_SOURCE_DIR}/../../lib
        ${CMAKE_CURRENT_SOURCE_DIR}/../base
	${CMAKE_CURRENT_BINARY_DIR}/../../config 
	${CMAKE_CURRENT_BINARY_DIR}/../base
)

set(ksystemlog_postfix_sources
	postfixFactory.cpp
	postfixConfigurationWidget.cpp
	postfixConfiguration.cpp
	postfixAnalyzer.cpp
	postfixLogMode.cpp
)

kde4_add_library(ksystemlog_postfix STATIC ${ksystemlog_postfix_sources})

add_dependencies(
	ksystemlog_postfix
	
	ksystemlog_base_mode
	ksystemlog_lib
)

target_link_libraries(
	ksystemlog_postfix
	${KDE4_KDEUI_LIBS}
	ksystemlog_lib
	ksystemlog_base_mode
	ksystemlog_config
)
