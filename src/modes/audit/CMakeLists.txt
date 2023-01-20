
add_library(ksystemlog_audit STATIC)
target_sources(ksystemlog_audit PRIVATE
        auditFactory.cpp auditFactory.h
        auditConfigurationWidget.cpp auditConfigurationWidget.h
        auditConfiguration.cpp auditConfiguration.h
        auditAnalyzer.cpp auditAnalyzer.h
        auditLogMode.cpp auditLogMode.h
        auditItemBuilder.cpp auditItemBuilder.h
        auditConvertor.cpp auditConvertor.h
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
