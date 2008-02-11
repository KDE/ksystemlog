
include_directories( 
	${CMAKE_CURRENT_SOURCE_DIR}/../..
	${CMAKE_CURRENT_SOURCE_DIR}/../base
	${CMAKE_CURRENT_SOURCE_DIR}/../../lib
	${CMAKE_CURRENT_SOURCE_DIR}/../../config
        ${CMAKE_CURRENT_BINARY_DIR}/../base
        ${CMAKE_CURRENT_BINARY_DIR}/../../config 
)

set(ksystemlog_xorg_sources
	xorgConfigurationWidget.cpp
	xorgConfiguration.cpp
	xorgAnalyzer.cpp
	xorgItemBuilder.cpp
	xorgLogMode.cpp
	xorgFactory.cpp
)

kde4_add_library(ksystemlog_xorg STATIC ${ksystemlog_xorg_sources})

add_dependencies(
	ksystemlog_xorg
	
	ksystemlog_base_mode 
	ksystemlog_lib
)

target_link_libraries(
    ksystemlog_xorg
	${KDE4_KDEUI_LIBS}
	ksystemlog_lib
	ksystemlog_base_mode 
	ksystemlog_config
)
