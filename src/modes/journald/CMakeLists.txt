set(ksystemlog_journald_sources
    journaldAddressDialog.cpp
    journaldAnalyzer.cpp
    journaldConfiguration.cpp
    journaldConfigurationWidget.cpp
    journaldFactory.cpp
    journaldItemBuilder.cpp
    journaldLocalAnalyzer.cpp
    journaldLogMode.cpp
    journaldNetworkAnalyzer.cpp
    journaldTypes.cpp
)

ki18n_wrap_ui(ksystemlog_journald_sources journaldAddressDialog.ui journaldConfigurationWidget.ui )

add_library(ksystemlog_journald STATIC ${ksystemlog_journald_sources})

add_dependencies(
    ksystemlog_journald

    ksystemlog_base_mode
    ksystemlog_lib
)

target_link_libraries(
    ksystemlog_journald

    ksystemlog_lib
    ksystemlog_base_mode
    ksystemlog_config
    ${JOURNALD_LIBRARIES}
    Qt5::Concurrent
    Qt5::Network
)
