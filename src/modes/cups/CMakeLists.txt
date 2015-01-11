
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/../..
        ${CMAKE_CURRENT_SOURCE_DIR}/../base
        ${CMAKE_CURRENT_SOURCE_DIR}/../../lib
        ${CMAKE_CURRENT_SOURCE_DIR}/../../config
        ${CMAKE_CURRENT_BINARY_DIR}/../base
        ${CMAKE_CURRENT_BINARY_DIR}/../../config
)

set(ksystemlog_cups_sources
	cupsConfiguration.cpp
	cupsConfigurationWidget.cpp
	
	cupsAnalyzer.cpp
	cupsItemBuilder.cpp
	cupsLogMode.cpp
	
	cupsAccessAnalyzer.cpp
	cupsAccessItemBuilder.cpp
	cupsAccessLogMode.cpp
	
	cupsPageAnalyzer.cpp
	cupsPageItemBuilder.cpp
	cupsPageLogMode.cpp
	
	cupsPdfAnalyzer.cpp
	cupsPdfItemBuilder.cpp
	cupsPdfLogMode.cpp
	
	cupsFactory.cpp
	
)

add_library(ksystemlog_cups STATIC ${ksystemlog_cups_sources})

add_dependencies(
	ksystemlog_cups
	 
	ksystemlog_base_mode
	ksystemlog_lib
)

target_link_libraries(
	ksystemlog_cups
	
	ksystemlog_lib
	ksystemlog_base_mode
	ksystemlog_config
)
