
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../lib
    ${CMAKE_CURRENT_BINARY_DIR}/../../config
    ${CMAKE_CURRENT_SOURCE_DIR}/../base
    ${CMAKE_CURRENT_BINARY_DIR}/../base
)

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
    systemd
)
