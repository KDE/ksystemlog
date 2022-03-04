add_library(ksystemlog_kernel STATIC)
target_sources(ksystemlog_kernel PRIVATE
	kernelFactory.cpp
	kernelAnalyzer.cpp
	kernelLogMode.cpp
	kernelItemBuilder.cpp

        kernelFactory.h
        kernelAnalyzer.h
        kernelLogMode.h
        kernelItemBuilder.h
    )


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
