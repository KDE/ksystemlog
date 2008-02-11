
include_directories( 
	${CMAKE_CURRENT_SOURCE_DIR}/../..
	${CMAKE_CURRENT_SOURCE_DIR}/../../lib
	${CMAKE_CURRENT_SOURCE_DIR}/../../config 
	${CMAKE_CURRENT_SOURCE_DIR}/../base
)

set(ksystemlog_kernel_sources
	kernelFactory.cpp
	kernelAnalyzer.cpp
	kernelLogMode.cpp
	kernelItemBuilder.cpp
)

kde4_add_library(ksystemlog_kernel STATIC ${ksystemlog_kernel_sources})

add_dependencies(
	ksystemlog_kernel 
	ksystemlog_base_mode
	ksystemlog_lib
)

target_link_libraries(
	ksystemlog_kernel
	${KDE4_KDEUI_LIBS}
	ksystemlog_lib
	ksystemlog_base_mode
	ksystemlog_config
)
