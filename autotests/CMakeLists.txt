include(ECMAddTests)

set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

include_directories( 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../src
  ${CMAKE_CURRENT_SOURCE_DIR}/../src/config
  ${CMAKE_CURRENT_BINARY_DIR}/../src/config
  ${CMAKE_CURRENT_SOURCE_DIR}/../src/lib
  ${CMAKE_CURRENT_SOURCE_DIR}/../src/modes/system
  ${CMAKE_CURRENT_SOURCE_DIR}/../src/modes/kernel
)

add_library(testCore STATIC testUtil.cpp testUtil.h testResources.qrc )
target_link_libraries(testCore PUBLIC
    Qt6::Test
    ksystemlog_lib
    ksystemlog_base_mode
    ksystemlog_config
    ksystemlog_system
    ksystemlog_kernel)


macro(ksystemlog_unit_tests)
	foreach(unitTest ${ARGN})
            ecm_add_test(${unitTest}.cpp TEST_NAME ksystemlog-${unitTest} LINK_LIBRARIES testCore)
	endforeach(unitTest)
endmacro(ksystemlog_unit_tests)

ksystemlog_unit_tests(
	logModeFactoryTest
	systemAnalyzerTest
        kernelAnalyzerTest

    # Not real unit tests
	kioLogFileReaderTest

        # Needs to be ported away from KDELibs4Support:
        # findIncompatibleKioTest
)
