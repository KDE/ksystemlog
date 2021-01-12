
########### Build ###############


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

ki18n_wrap_ui(ksystemlog_sources
    detailDialogBase.ui
    loggerDialogBase.ui
    generalConfigurationWidgetBase.ui
)

add_executable(ksystemlog ${ksystemlog_sources})

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

    KF5::Archive
    KF5::TextWidgets
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

install(TARGETS ksystemlog ${INSTALL_TARGETS_DEFAULT_ARGS})

install(PROGRAMS org.kde.ksystemlog.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES org.kde.ksystemlog.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
install(FILES ksystemlogui.rc DESTINATION ${KXMLGUI_INSTALL_DIR}/ksystemlog)
