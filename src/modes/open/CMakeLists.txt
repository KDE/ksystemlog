add_library(ksystemlog_open STATIC)
target_sources(ksystemlog_open PRIVATE
	openAnalyzer.cpp
	openFactory.cpp
	openLogMode.cpp
)



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
