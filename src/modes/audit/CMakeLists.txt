
add_library(ksystemlog_audit STATIC)
target_sources(ksystemlog_audit PRIVATE
        auditFactory.cpp
        auditConfigurationWidget.cpp
        auditConfiguration.cpp
        auditAnalyzer.cpp
        auditLogMode.cpp
        auditItemBuilder.cpp
        auditConvertor.cpp
)

add_dependencies(
	ksystemlog_audit
	
	ksystemlog_base_mode
	ksystemlog_lib
)

target_link_libraries(
	ksystemlog_audit
	
	ksystemlog_lib
	ksystemlog_base_mode
	ksystemlog_config
	
        PkgConfig::PC_AUDIT
)
