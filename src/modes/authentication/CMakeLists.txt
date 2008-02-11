
include_directories( 
	${CMAKE_CURRENT_SOURCE_DIR}/../..
	${CMAKE_CURRENT_SOURCE_DIR}/../base
	${CMAKE_CURRENT_SOURCE_DIR}/../../lib
	${CMAKE_CURRENT_SOURCE_DIR}/../../config 
        ${CMAKE_CURRENT_BINARY_DIR}/../base
        ${CMAKE_CURRENT_BINARY_DIR}/../../config
)

set(ksystemlog_authentication_sources
	authenticationConfigurationWidget.cpp
	authenticationConfiguration.cpp
	authenticationAnalyzer.cpp
	authenticationLogMode.cpp
	authenticationFactory.cpp
)

kde4_add_library(ksystemlog_authentication STATIC ${ksystemlog_authentication_sources})

add_dependencies(
	ksystemlog_authentication
	ksystemlog_base_mode 
	ksystemlog_lib
)

target_link_libraries(
	ksystemlog_authentication
	${KDE4_KDEUI_LIBS}
	ksystemlog_lib
	ksystemlog_base_mode 
	ksystemlog_config
)
