
########### Build ###############

add_subdirectory(lib)
add_subdirectory(config)

add_subdirectory(modes/base)
add_subdirectory(modes/open)

add_subdirectory(modes/system)
add_subdirectory(modes/kernel)
add_subdirectory(modes/xorg)
add_subdirectory(modes/cron)
add_subdirectory(modes/authentication)
add_subdirectory(modes/daemon)
add_subdirectory(modes/acpid)
add_subdirectory(modes/xsession)
add_subdirectory(modes/apache)
add_subdirectory(modes/postfix)
add_subdirectory(modes/samba)
add_subdirectory(modes/cups)
if (${AUDIT_FOUND})
    add_subdirectory(modes/audit)
endif (${AUDIT_FOUND})
if (${JOURNALD_FOUND})
    add_subdirectory(modes/journald)
endif (${JOURNALD_FOUND})

add_executable(ksystemlog)
target_sources(ksystemlog PRIVATE
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
    mainWindow.h
    logModePluginsLoader.h
    loggerDialog.h
    detailDialog.h
    generalConfigurationWidget.h
    configurationDialog.h
    tabLogViewsWidget.h
    tabLogManager.h
    statusBar.h
    ksystemlogui.qrc
)

ki18n_wrap_ui(ksystemlog
    detailDialogBase.ui
    loggerDialogBase.ui
    generalConfigurationWidgetBase.ui
)


target_link_libraries(ksystemlog
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

    KF6::Archive
    KF6::TextWidgets
)

if (${JOURNALD_FOUND})
    target_link_libraries(ksystemlog
        ksystemlog_journald)
endif(${JOURNALD_FOUND})

if (${AUDIT_FOUND})
    target_link_libraries(ksystemlog
        ksystemlog_audit)
endif(${AUDIT_FOUND})

########### Installation ###############

install(TARGETS ksystemlog ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(PROGRAMS org.kde.ksystemlog.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.ksystemlog.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
