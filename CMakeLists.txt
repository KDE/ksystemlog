project(KSystemLog)

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
add_subdirectory(src/modes/)

add_subdirectory(tests) 
