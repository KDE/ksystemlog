include_directories( 
	${KDE4_INCLUDE_DIR} 
	${QT_INCLUDES}  
)

set(ksystemlog_config_SRCS
	dummyConfig.cpp
)

kde4_add_kcfg_files(ksystemlog_config_SRCS ksystemlogConfig.kcfgc)

kde4_add_library(ksystemlog_config STATIC ${ksystemlog_config_SRCS})

target_link_libraries(ksystemlog_config
	${KDE4_KDEUI_LIBS}
)
