
set(ksystemlog_acpid_sources
	acpidConfigurationWidget.cpp
	acpidConfiguration.cpp
	acpidAnalyzer.cpp
	acpidItemBuilder.cpp
	acpidLogMode.cpp
	acpidFactory.cpp
)

add_library(ksystemlog_acpid STATIC ${ksystemlog_acpid_sources})

add_dependencies(
	ksystemlog_acpid
	
	ksystemlog_base_mode 
	ksystemlog_lib
)

target_link_libraries(
	ksystemlog_acpid
	
	ksystemlog_lib
	ksystemlog_base_mode 
	ksystemlog_config
)
