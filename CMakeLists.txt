project(KSystemlog)

# KDE Application Version, managed by release script
set (KDE_APPLICATIONS_VERSION_MAJOR "17")
set (KDE_APPLICATIONS_VERSION_MINOR "03")
set (KDE_APPLICATIONS_VERSION_MICRO "90")
set (KDE_APPLICATIONS_VERSION "${KDE_APPLICATIONS_VERSION_MAJOR}.${KDE_APPLICATIONS_VERSION_MINOR}.${KDE_APPLICATIONS_VERSION_MICRO}")

cmake_minimum_required (VERSION 2.8.12 FATAL_ERROR)
set (QT_MIN_VERSION "5.2.0")
set (KF5_MIN_VERSION "5.0.0")

find_package (ECM 1.7.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_definitions(-DTRANSLATION_DOMAIN="ksystemlog")

include(ECMSetupVersion)
include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(FeatureSummary)

ecm_setup_version(${KDE_APPLICATIONS_VERSION} VARIABLE_PREFIX KSYSTEMLOG
                  VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/ksystemlog_version.h")

find_package (Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    Widgets
    Test
    PrintSupport
)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    XmlGui
    CoreAddons
    WidgetsAddons
    ItemViews
    IconThemes
    KIO
    Config
    Archive
    DocTools
    I18n
    Completion
    TextWidgets
)

find_package(Journald)

include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckSymbolExists)
include(CheckFunctionExists)
include(CheckLibraryExists)
include(CheckTypeSize)

if(WIN32)
  set(CMAKE_REQUIRED_LIBRARIES ${KDEWIN32_LIBRARIES})
  set(CMAKE_REQUIRED_INCLUDES  ${KDEWIN32_INCLUDES})
endif(WIN32)
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS -DQT_NO_URL_CAST_FROM_STRING)
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

########### Subfolders ##########

add_subdirectory(src)
add_subdirectory(src/lib)
add_subdirectory(src/config)

add_subdirectory(src/modes/base)
add_subdirectory(src/modes/open)

add_subdirectory(src/modes/system)
add_subdirectory(src/modes/kernel)
add_subdirectory(src/modes/xorg)
add_subdirectory(src/modes/cron)
add_subdirectory(src/modes/authentication)
add_subdirectory(src/modes/daemon)
add_subdirectory(src/modes/acpid)
add_subdirectory(src/modes/xsession)
add_subdirectory(src/modes/apache)
add_subdirectory(src/modes/postfix)
add_subdirectory(src/modes/samba)
add_subdirectory(src/modes/cups)
if (${JOURNALD_FOUND})
    add_subdirectory(src/modes/journald)
endif (${JOURNALD_FOUND})

add_subdirectory(tests)
add_subdirectory(doc)

find_package(KF5I18n CONFIG REQUIRED)
ki18n_install(po)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
