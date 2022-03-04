add_library(ksystemlog_authentication STATIC)

target_sources(ksystemlog_authentication PRIVATE
	authenticationConfigurationWidget.cpp
	authenticationConfiguration.cpp
	authenticationAnalyzer.cpp
	authenticationLogMode.cpp
	authenticationFactory.cpp

        authenticationConfigurationWidget.h
        authenticationConfiguration.h
        authenticationAnalyzer.h
        authenticationLogMode.h
        authenticationFactory.h
    )


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
