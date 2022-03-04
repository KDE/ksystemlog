add_library(ksystemlog_cups STATIC)

target_sources(ksystemlog_cups PRIVATE
	cupsConfiguration.cpp
	cupsConfigurationWidget.cpp
	
	cupsAnalyzer.cpp
	cupsItemBuilder.cpp
	cupsLogMode.cpp
	
	cupsAccessAnalyzer.cpp
	cupsAccessItemBuilder.cpp
	cupsAccessLogMode.cpp
	
	cupsPageAnalyzer.cpp
	cupsPageItemBuilder.cpp
	cupsPageLogMode.cpp
	
	cupsPdfAnalyzer.cpp
	cupsPdfItemBuilder.cpp
	cupsPdfLogMode.cpp
	
	cupsFactory.cpp

        cupsConfiguration.h
        cupsConfigurationWidget.h

        cupsAnalyzer.h
        cupsItemBuilder.h
        cupsLogMode.h

        cupsAccessAnalyzer.h
        cupsAccessItemBuilder.h
        cupsAccessLogMode.h

        cupsPageAnalyzer.h
        cupsPageItemBuilder.h
        cupsPageLogMode.h

        cupsPdfAnalyzer.h
        cupsPdfItemBuilder.h
        cupsPdfLogMode.h

        cupsFactory.h

)


add_dependencies(
	ksystemlog_cups
	 
	ksystemlog_base_mode
	ksystemlog_lib
)

target_link_libraries(
	ksystemlog_cups
	
	ksystemlog_lib
	ksystemlog_base_mode
	ksystemlog_config
)
