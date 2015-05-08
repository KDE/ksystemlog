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

macro(ksystemlog_unit_tests)
	foreach(unitTest ${ARGN})
		set(unitTest_sources
			${unitTest}.cpp
			testUtil.cpp
		)
		
		qt5_add_resources( unitTest_sources testResources.qrc )

                add_executable(${unitTest}
                               ${unitTest_sources})
                add_test(ksystemlog-${unitTest} ${unitTest})
                ecm_mark_as_test(${unitTest})
		
		target_link_libraries(
			${unitTest} 
			 
			Qt5::Test
			
			KF5::KIOCore
			KF5::KDELibs4Support 
			ksystemlog_lib
			ksystemlog_base_mode
			ksystemlog_config
			ksystemlog_system
			ksystemlog_kernel
		)
	endforeach(unitTest)
endmacro(ksystemlog_unit_tests)

ksystemlog_unit_tests(
	logModeFactoryTest
	systemAnalyzerTest
	kernelAnalyzerTest

    # Not real unit tests
	kioLogFileReaderTest
	findIncompatibleKioTest
)
