add_library(ksystemlog_apache STATIC ${ksystemlog_apache_sources})

target_sources(ksystemlog_apache PRIVATE
	apacheConfiguration.cpp
	apacheConfigurationWidget.cpp
	apacheAnalyzer.cpp
	apacheItemBuilder.cpp
	apacheAccessAnalyzer.cpp
	apacheAccessItemBuilder.cpp
	apacheFactory.cpp
	apacheLogMode.cpp
	apacheAccessLogMode.cpp

        apacheConfiguration.h
        apacheConfigurationWidget.h
        apacheAnalyzer.h
        apacheItemBuilder.h
        apacheAccessAnalyzer.h
        apacheAccessItemBuilder.h
        apacheFactory.h
        apacheLogMode.h
        apacheAccessLogMode.h
    )


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
