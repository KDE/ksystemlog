add_library(ksystemlog_xsession STATIC)
target_sources(ksystemlog_xsession PRIVATE
	xsessionConfigurationWidget.cpp
	xsessionConfiguration.cpp
	xsessionAnalyzer.cpp
	xsessionItemBuilder.cpp
	xsessionLogMode.cpp
	xsessionFactory.cpp

        xsessionConfigurationWidget.h
        xsessionConfiguration.h
        xsessionAnalyzer.h
        xsessionItemBuilder.h
        xsessionLogMode.h
        xsessionFactory.h
    )

ki18n_wrap_ui(ksystemlog_xsession xsessionConfigurationWidgetBase.ui )


add_dependencies(
	ksystemlog_xsession
	ksystemlog_base_mode 
	ksystemlog_lib
)

target_link_libraries(
    ksystemlog_xsession
	
	ksystemlog_lib
	ksystemlog_base_mode 
	ksystemlog_config

        KF6::KIOWidgets
)
