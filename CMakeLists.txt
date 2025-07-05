cmake_minimum_required (VERSION 3.16 FATAL_ERROR)

# KDE Application Version, managed by release script
set (RELEASE_SERVICE_VERSION_MAJOR "25")
set (RELEASE_SERVICE_VERSION_MINOR "11")
set (RELEASE_SERVICE_VERSION_MICRO "70")
set (RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(KSystemLog VERSION ${RELEASE_SERVICE_VERSION})

set (QT_MIN_VERSION "6.8.0")
set (KF_MIN_VERSION "6.11.0")

find_package (ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH}  ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_definitions(-DTRANSLATION_DOMAIN="ksystemlog")

include(ECMSetupVersion)
include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(FeatureSummary)
include(ECMQtDeclareLoggingCategory)
include(ECMDeprecationSettings)
include(KDEGitCommitHooks)
include(KDEClangFormat)
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h *.c)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})




ecm_setup_version(${RELEASE_SERVICE_VERSION} VARIABLE_PREFIX KSYSTEMLOG
                  VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/ksystemlog_version.h")

find_package (Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Concurrent
    Core
    Widgets
    Test
    PrintSupport
)

find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS
    XmlGui
    CoreAddons
    WidgetsAddons
    ItemViews
    KIO
    Config
    Archive
    I18n
    Completion
    TextWidgets
    Crash
)
find_package(KF6DocTools ${KF_MIN_VERSION})
set_package_properties(KF6DocTools PROPERTIES DESCRIPTION
    "Tools to generate documentation"
    TYPE OPTIONAL
)

find_package(Journald)
if(JOURNALD_FOUND)
    find_package(Qt6Network ${QT_MIN_VERSION} CONFIG REQUIRED)
    add_definitions(-DHAVE_JOURNALD)
endif()

find_package(Audit)
if(AUDIT_FOUND)
    add_definitions(-DHAVE_AUDIT)
endif()

include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckSymbolExists)
include(CheckFunctionExists)
include(CheckLibraryExists)
include(CheckTypeSize)
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})

# Help Eclipse to parse errors more efficiently
if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
endif(CMAKE_COMPILER_IS_GNUCC)
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")
endif(CMAKE_COMPILER_IS_GNUCXX)


add_custom_target(
    pkg
    COMMAND ./build-package.sh
)

##
# To specify a different install prefix, use :
# cmake -D CMAKE_INSTALL_PREFIX=build .
#
# To help Eclipse discover include paths, use :
# cmake -D CMAKE_VERBOSE_MAKEFILE=true .
##


########### Subfolders ##########
ecm_set_disabled_deprecation_versions(QT 6.9
    KF 6.13.0
)


add_subdirectory(src)

add_subdirectory(autotests)

find_package(KF6I18n CONFIG REQUIRED)
if (KF6DocTools_FOUND)
    kdoctools_install(po)
    add_subdirectory(doc)
endif()

ki18n_install(po)
ecm_qt_install_logging_categories(EXPORT KSYSTEMLOG FILE ksystemlog.categories DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR})
kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
