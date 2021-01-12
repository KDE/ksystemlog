set(ksystemlog_authentication_sources
	authenticationConfigurationWidget.cpp
	authenticationConfiguration.cpp
	authenticationAnalyzer.cpp
	authenticationLogMode.cpp
	authenticationFactory.cpp
)

add_library(ksystemlog_authentication STATIC ${ksystemlog_authentication_sources})

add_dependencies(
	ksystemlog_authentication
	ksystemlog_base_mode 
	ksystemlog_lib
)

target_link_libraries(
	ksystemlog_authentication
	
	ksystemlog_lib
	ksystemlog_base_mode 
	ksystemlog_config

        KF5::KIOWidgets
)
