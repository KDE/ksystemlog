
include_directories( 
	${CMAKE_CURRENT_SOURCE_DIR}/../..
	${CMAKE_CURRENT_SOURCE_DIR}/../../lib
	${CMAKE_CURRENT_BINARY_DIR}/../../config 
	${CMAKE_CURRENT_SOURCE_DIR}/../base
        ${CMAKE_CURRENT_BINARY_DIR}/../base
)

set(ksystemlog_audit_sources
	auditFactory.cpp
	auditConfigurationWidget.cpp
	auditConfiguration.cpp
	auditAnalyzer.cpp
	auditLogMode.cpp
	auditItemBuilder.cpp
        auditConvertor.cpp
)

add_library(ksystemlog_audit STATIC ${ksystemlog_audit_sources})

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
