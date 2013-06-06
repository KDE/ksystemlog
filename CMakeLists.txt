project(KSystemLog)

find_package(KDE4 REQUIRED)

include(KDE4Defaults)
include(MacroLibrary)

include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckSymbolExists)
include(CheckFunctionExists)
include(CheckLibraryExists)
include(CheckPrototypeExists)
include(CheckTypeSize)

set(CMAKE_REQUIRED_DEFINITIONS ${_KDE4_PLATFORM_DEFINITIONS})
if(WIN32)
  set(CMAKE_REQUIRED_LIBRARIES ${KDEWIN32_LIBRARIES})
  set(CMAKE_REQUIRED_INCLUDES  ${KDEWIN32_INCLUDES})
endif(WIN32)
add_definitions(${QT_DEFINITIONS} ${QT_QTDBUS_DEFINITIONS} ${KDE4_DEFINITIONS})
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})

# Help Eclipse to parse errors more efficiently
if(CMAKE_COMPILER_IS_GNUCC)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
endif(CMAKE_COMPILER_IS_GNUCC)
if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")
endif(CMAKE_COMPILER_IS_GNUCXX)


add_custom_target(  
    package
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

add_subdirectory(tests) 

add_subdirectory(doc)
