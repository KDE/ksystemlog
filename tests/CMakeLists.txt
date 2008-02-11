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
		
		qt4_add_resources( unitTest_sources testResources.qrc )

		kde4_add_unit_test(
			${unitTest} 
			TESTNAME ksystemlog-${unitTest}
			
			${unitTest_sources}
		)
		
		target_link_libraries(
			${unitTest} 
			${QT_AND_KDECORE_LIBS} 
			${QT_QTTEST_LIBRARY}
			${KDE4_KDEUI_LIBS}
			${KDE4_KIO_LIBS}
			${KDE4_KDECORE_LIBS} 
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
