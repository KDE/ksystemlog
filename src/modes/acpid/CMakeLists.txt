add_library(ksystemlog_acpid STATIC)

target_sources(ksystemlog_acpid PRIVATE
	acpidConfigurationWidget.cpp
	acpidConfiguration.cpp
	acpidAnalyzer.cpp
	acpidItemBuilder.cpp
	acpidLogMode.cpp
	acpidFactory.cpp
)


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
