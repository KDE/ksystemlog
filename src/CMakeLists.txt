
########### Build ###############

include_directories( 
	${CMAKE_CURRENT_SOURCE_DIR}/config 
	${CMAKE_CURRENT_SOURCE_DIR}/lib
	${CMAKE_CURRENT_BINARY_DIR}/config
	${CMAKE_CURRENT_BINARY_DIR}/lib 
	${CMAKE_CURRENT_SOURCE_DIR} 
	
	${CMAKE_CURRENT_SOURCE_DIR}/modes/open

	${CMAKE_CURRENT_SOURCE_DIR}/modes/system 
	${CMAKE_CURRENT_SOURCE_DIR}/modes/kernel
	${CMAKE_CURRENT_SOURCE_DIR}/modes/daemon
	${CMAKE_CURRENT_SOURCE_DIR}/modes/xorg
	${CMAKE_CURRENT_SOURCE_DIR}/modes/cron
	${CMAKE_CURRENT_SOURCE_DIR}/modes/acpid
	${CMAKE_CURRENT_SOURCE_DIR}/modes/xsession
	${CMAKE_CURRENT_SOURCE_DIR}/modes/apache
	${CMAKE_CURRENT_SOURCE_DIR}/modes/cups
	${CMAKE_CURRENT_SOURCE_DIR}/modes/samba
	${CMAKE_CURRENT_SOURCE_DIR}/modes/authentication
	${CMAKE_CURRENT_SOURCE_DIR}/modes/postfix 
)

set(ksystemlog_sources
	main.cpp
	mainWindow.cpp
	logModePluginsLoader.cpp
	loggerDialog.cpp
	detailDialog.cpp
	generalConfigurationWidget.cpp
	configurationDialog.cpp
	tabLogViewsWidget.cpp
	tabLogManager.cpp
	statusBar.cpp

)

kde4_add_ui_files(ksystemlog_sources
	detailDialogBase.ui
	loggerDialogBase.ui
	generalConfigurationWidgetBase.ui
)

kde4_add_executable(ksystemlog ${ksystemlog_sources})

target_link_libraries(ksystemlog  
	${KDE4_KIO_LIBS} 
	${KDE4_KDEUI_LIBS}
	ksystemlog_lib
	ksystemlog_config
	ksystemlog_open
	ksystemlog_system
	ksystemlog_kernel
	ksystemlog_xorg 
	ksystemlog_cron 
	ksystemlog_apache
	ksystemlog_authentication 
	ksystemlog_daemon 
	ksystemlog_acpid 
	ksystemlog_xsession
	ksystemlog_postfix 
	ksystemlog_cups 
	ksystemlog_samba 
)

########### Installation ###############

install( TARGETS ksystemlog ${INSTALL_TARGETS_DEFAULT_ARGS})

install(PROGRAMS   ksystemlog.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES   ksystemlogui.rc DESTINATION ${DATA_INSTALL_DIR}/ksystemlog)
