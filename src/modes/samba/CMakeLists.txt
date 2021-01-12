set(ksystemlog_samba_sources
	sambaConfiguration.cpp
	sambaConfigurationWidget.cpp
	sambaAnalyzer.cpp
	sambaLogMode.cpp
	sambaItemBuilder.cpp
	sambaAccessLogMode.cpp
	netbiosLogMode.cpp
	sambaFactory.cpp
	
)

add_library(ksystemlog_samba STATIC ${ksystemlog_samba_sources})

add_dependencies(
	ksystemlog_samba
	ksystemlog_base_mode
	ksystemlog_lib
)

target_link_libraries(
	ksystemlog_samba
	
	ksystemlog_lib
	ksystemlog_base_mode
	ksystemlog_config
)
