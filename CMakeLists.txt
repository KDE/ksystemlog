cmake_minimum_required (VERSION 3.16 FATAL_ERROR)

# KDE Application Version, managed by release script
set (RELEASE_SERVICE_VERSION_MAJOR "21")
set (RELEASE_SERVICE_VERSION_MINOR "07")
set (RELEASE_SERVICE_VERSION_MICRO "70")
set (RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(KSystemlog VERSION ${RELEASE_SERVICE_VERSION})

set (QT_MIN_VERSION "5.15.0")
set (KF5_MIN_VERSION "5.83.0")

find_package (ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH}  ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_definitions(-DTRANSLATION_DOMAIN="ksystemlog")

include(ECMSetupVersion)
include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(FeatureSummary)
include(ECMQtDeclareLoggingCategory)
include(KDEGitCommitHooks)

ecm_setup_version(${RELEASE_SERVICE_VERSION} VARIABLE_PREFIX KSYSTEMLOG
                  VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/ksystemlog_version.h")

find_package (Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Concurrent
    Core
    Network
    Widgets
    Test
    PrintSupport
)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    XmlGui
    CoreAddons
    WidgetsAddons
    ItemViews
    KIO
    Config
    Archive
    DocTools
    I18n
    Completion
    TextWidgets
)

find_package(Journald)
find_package(Audit)

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

if (${JOURNALD_FOUND})
    add_definitions(-DHAVE_JOURNALD)
endif (${JOURNALD_FOUND})

if (${AUDIT_FOUND})
    add_definitions(-DHAVE_AUDIT)
endif (${AUDIT_FOUND})

########### Subfolders ##########
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)
add_definitions(-DKF_DISABLE_DEPRECATED_BEFORE_AND_AT=0x060000)

add_subdirectory(src)

add_subdirectory(autotests)
add_subdirectory(doc)

find_package(KF5I18n CONFIG REQUIRED)
ki18n_install(po)
kdoctools_install(po)
ecm_qt_install_logging_categories(EXPORT KSYSTEMLOG FILE ksystemlog.categories DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR})
kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
