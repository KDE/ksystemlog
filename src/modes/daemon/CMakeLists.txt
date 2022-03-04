add_library(ksystemlog_daemon STATIC)

target_sources(ksystemlog_daemon PRIVATE
	daemonConfigurationWidget.cpp
	daemonConfiguration.cpp
	daemonLogMode.cpp
	daemonFactory.cpp

        daemonConfigurationWidget.h
        daemonConfiguration.h
        daemonLogMode.h
        daemonFactory.h
    )


add_dependencies(
	ksystemlog_daemon
	ksystemlog_base_mode 
	ksystemlog_lib
)

target_link_libraries(
	ksystemlog_daemon
	
	ksystemlog_lib
	ksystemlog_base_mode 
	ksystemlog_config
)
