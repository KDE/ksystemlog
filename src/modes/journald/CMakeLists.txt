add_library(ksystemlog_journald STATIC)

target_sources(ksystemlog_journald PRIVATE
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

    journaldAddressDialog.h
    journaldAnalyzer.h
    journaldConfiguration.h
    journaldConfigurationWidget.h
    journaldFactory.h
    journaldItemBuilder.h
    journaldLocalAnalyzer.h
    journaldLogMode.h
    journaldNetworkAnalyzer.h
    journaldTypes.h
)

ki18n_wrap_ui(ksystemlog_journald journaldAddressDialog.ui journaldConfigurationWidget.ui )


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
    Qt::Concurrent
    Qt::Network
)
