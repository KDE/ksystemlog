
include_directories( 
	${CMAKE_CURRENT_SOURCE_DIR}/../..
	${CMAKE_CURRENT_SOURCE_DIR}/../../lib
	${CMAKE_CURRENT_SOURCE_DIR}/../../config 
	${CMAKE_CURRENT_SOURCE_DIR}/../base
	${CMAKE_CURRENT_BINARY_DIR}/../../config
	${CMAKE_CURRENT_BINARY_DIR}/../base
)

set(ksystemlog_apache_sources
	apacheConfiguration.cpp
	apacheConfigurationWidget.cpp
	apacheAnalyzer.cpp
	apacheItemBuilder.cpp
	apacheAccessAnalyzer.cpp
	apacheAccessItemBuilder.cpp
	apacheFactory.cpp
	apacheLogMode.cpp
	apacheAccessLogMode.cpp
)

add_library(ksystemlog_apache STATIC ${ksystemlog_apache_sources})

add_dependencies(
	ksystemlog_apache
	 
	ksystemlog_base_mode
	ksystemlog_lib
)

target_link_libraries(
	ksystemlog_apache
	
	ksystemlog_lib
	ksystemlog_base_mode
	ksystemlog_config
)
