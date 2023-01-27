
add_library(ksystemlog_config STATIC dummyConfig.cpp)
kconfig_add_kcfg_files(ksystemlog_config ksystemlogConfig.kcfgc)

target_link_libraries(ksystemlog_config
        KF${KF_MAJOR_VERSION}::ConfigGui
)
