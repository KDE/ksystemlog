
include_directories( 
	${CMAKE_CURRENT_SOURCE_DIR}/../..
	${CMAKE_CURRENT_SOURCE_DIR}/../base
	${CMAKE_CURRENT_SOURCE_DIR}/../../lib
	${CMAKE_CURRENT_BINARY_DIR}/../../config 
)

set(ksystemlog_open_sources
	openAnalyzer.cpp
	openFactory.cpp
	openLogMode.cpp
)


add_library(ksystemlog_open STATIC ${ksystemlog_open_sources})

add_dependencies(
	ksystemlog_open 
	ksystemlog_lib
)

target_link_libraries(
	ksystemlog_open
	
	ksystemlog_lib
	ksystemlog_config
	ksystemlog_base_mode
)
