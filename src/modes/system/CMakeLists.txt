add_library(ksystemlog_system STATIC)
target_sources(ksystemlog_system PRIVATE
	systemFactory.cpp
	systemConfigurationWidget.cpp
	systemConfiguration.cpp
	systemAnalyzer.cpp
	systemLogMode.cpp

        systemFactory.h
        systemConfigurationWidget.h
        systemConfiguration.h
        systemAnalyzer.h
        systemLogMode.h
    )


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
