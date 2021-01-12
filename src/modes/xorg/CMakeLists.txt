set(ksystemlog_xorg_sources
	xorgConfigurationWidget.cpp
	xorgConfiguration.cpp
	xorgAnalyzer.cpp
	xorgItemBuilder.cpp
	xorgLogMode.cpp
	xorgFactory.cpp
)

add_library(ksystemlog_xorg STATIC ${ksystemlog_xorg_sources})

add_dependencies(
	ksystemlog_xorg
	
	ksystemlog_base_mode 
	ksystemlog_lib
)

target_link_libraries(
    ksystemlog_xorg
	
	ksystemlog_lib
	ksystemlog_base_mode 
	ksystemlog_config
)
