set(ksystemlog_cron_sources
	cronConfigurationWidget.cpp
	cronConfiguration.cpp
	cronAnalyzer.cpp
	cronItemBuilder.cpp
	cronLogMode.cpp
	cronFactory.cpp
)

add_library(ksystemlog_cron STATIC ${ksystemlog_cron_sources})

add_dependencies(
	ksystemlog_cron
	
	ksystemlog_base_mode 
	ksystemlog_lib
)

target_link_libraries(
    ksystemlog_cron
	
	ksystemlog_lib
	ksystemlog_base_mode 
	ksystemlog_config
)
