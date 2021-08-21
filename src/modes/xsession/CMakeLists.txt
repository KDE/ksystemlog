add_library(ksystemlog_xsession STATIC)
target_sources(ksystemlog_xsession PRIVATE
	xsessionConfigurationWidget.cpp
	xsessionConfiguration.cpp
	xsessionAnalyzer.cpp
	xsessionItemBuilder.cpp
	xsessionLogMode.cpp
	xsessionFactory.cpp
)

ki18n_wrap_ui(ksystemlog_xsession_sources xsessionConfigurationWidgetBase.ui )


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

        KF5::KIOWidgets
)
