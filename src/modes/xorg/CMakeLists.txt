add_library(ksystemlog_xorg STATIC)
target_sources(ksystemlog_xorg PRIVATE
	xorgConfigurationWidget.cpp
	xorgConfiguration.cpp
	xorgAnalyzer.cpp
	xorgItemBuilder.cpp
	xorgLogMode.cpp
	xorgFactory.cpp
)


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
