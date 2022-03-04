add_library(ksystemlog_cron STATIC)

target_sources(ksystemlog_cron PRIVATE
	cronConfigurationWidget.cpp
	cronConfiguration.cpp
	cronAnalyzer.cpp
	cronItemBuilder.cpp
	cronLogMode.cpp
	cronFactory.cpp

        cronConfigurationWidget.h
        cronConfiguration.h
        cronAnalyzer.h
        cronItemBuilder.h
        cronLogMode.h
        cronFactory.h
    )


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
