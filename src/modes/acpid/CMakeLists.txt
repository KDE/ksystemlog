
include_directories( 
	${CMAKE_CURRENT_SOURCE_DIR}/../..
	${CMAKE_CURRENT_SOURCE_DIR}/../base
	${CMAKE_CURRENT_SOURCE_DIR}/../../lib
	${CMAKE_CURRENT_SOURCE_DIR}/../../config 
        ${CMAKE_CURRENT_BINARY_DIR}/../base
        ${CMAKE_CURRENT_BINARY_DIR}/../../config
)

set(ksystemlog_acpid_sources
	acpidConfigurationWidget.cpp
	acpidConfiguration.cpp
	acpidAnalyzer.cpp
	acpidItemBuilder.cpp
	acpidLogMode.cpp
	acpidFactory.cpp
)

kde4_add_library(ksystemlog_acpid STATIC ${ksystemlog_acpid_sources})

add_dependencies(
	ksystemlog_acpid
	
	ksystemlog_base_mode 
	ksystemlog_lib
)

target_link_libraries(
	ksystemlog_acpid
	${KDE4_KDEUI_LIBS}
	ksystemlog_lib
	ksystemlog_base_mode 
	ksystemlog_config
)
