set(ksystemlog_system_sources
	systemFactory.cpp
	systemConfigurationWidget.cpp
	systemConfiguration.cpp
	systemAnalyzer.cpp
	systemLogMode.cpp
)

add_library(ksystemlog_system STATIC ${ksystemlog_system_sources})

add_dependencies(
	ksystemlog_system 
	
	ksystemlog_base_mode
	ksystemlog_lib
)

target_link_libraries(
	ksystemlog_system
	
	ksystemlog_lib
	ksystemlog_base_mode
	ksystemlog_config
)
