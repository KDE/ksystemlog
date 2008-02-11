
include_directories( 
	${CMAKE_CURRENT_SOURCE_DIR}/../..
	${CMAKE_CURRENT_SOURCE_DIR}/../base
	${CMAKE_CURRENT_SOURCE_DIR}/../../lib
	${CMAKE_CURRENT_SOURCE_DIR}/../../config 
        ${CMAKE_CURRENT_BINARY_DIR}/../base
        ${CMAKE_CURRENT_BINARY_DIR}/../../config
)

set(ksystemlog_cron_sources
	cronConfigurationWidget.cpp
	cronConfiguration.cpp
	cronAnalyzer.cpp
	cronItemBuilder.cpp
	cronLogMode.cpp
	cronFactory.cpp
)

kde4_add_library(ksystemlog_cron STATIC ${ksystemlog_cron_sources})

add_dependencies(
	ksystemlog_cron
	
	ksystemlog_base_mode 
	ksystemlog_lib
)

target_link_libraries(
    ksystemlog_cron
	${KDE4_KDEUI_LIBS}
	ksystemlog_lib
	ksystemlog_base_mode 
	ksystemlog_config
)
