set(ksystemlog_daemon_sources
	daemonConfigurationWidget.cpp
	daemonConfiguration.cpp
	daemonLogMode.cpp
	daemonFactory.cpp
)

add_library(ksystemlog_daemon STATIC ${ksystemlog_daemon_sources})

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
