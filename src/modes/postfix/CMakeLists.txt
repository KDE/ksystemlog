add_library(ksystemlog_postfix STATIC)
target_sources(ksystemlog_postfix PRIVATE
	postfixFactory.cpp
	postfixConfigurationWidget.cpp
	postfixConfiguration.cpp
	postfixAnalyzer.cpp
	postfixLogMode.cpp

        postfixFactory.h
        postfixConfigurationWidget.h
        postfixConfiguration.h
        postfixAnalyzer.h
        postfixLogMode.h
    )


add_dependencies(
	ksystemlog_postfix
	
	ksystemlog_base_mode
	ksystemlog_lib
)

target_link_libraries(
	ksystemlog_postfix
	
	ksystemlog_lib
	ksystemlog_base_mode
	ksystemlog_config
)
