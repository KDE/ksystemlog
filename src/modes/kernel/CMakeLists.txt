set(ksystemlog_kernel_sources
	kernelFactory.cpp
	kernelAnalyzer.cpp
	kernelLogMode.cpp
	kernelItemBuilder.cpp
)

add_library(ksystemlog_kernel STATIC ${ksystemlog_kernel_sources})

add_dependencies(
	ksystemlog_kernel 
	ksystemlog_base_mode
	ksystemlog_lib
)

target_link_libraries(
	ksystemlog_kernel
	
	ksystemlog_lib
	ksystemlog_base_mode
	ksystemlog_config
)
