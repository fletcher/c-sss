cmake_minimum_required (VERSION 2.6)


# ==================
# Define Our Project
# ==================

set (My_Project_Title "c-SSS")
set (My_Project_Description "Implementation of Shamir's Secret Sharing in C.")
set (My_Project_Author "Fletcher T. Penney")
set (My_Project_Revised_Date "2017-07-29")
set (My_Project_Version_Major 0)
set (My_Project_Version_Minor 2)
set (My_Project_Version_Patch 2)

set (My_Project_Version "${My_Project_Version_Major}.${My_Project_Version_Minor}.${My_Project_Version_Patch}")

set (My_Project_Copyright_Date "2015-2017")
set (My_Project_Copyright "Copyright © ${My_Project_Copyright_Date} ${My_Project_Author}.")

project (${My_Project_Title})


# Search for included files here
include_directories( ${PROJECT_SOURCE_DIR}/src )
include_directories( ${PROJECT_SOURCE_DIR}/test )
include_directories(${PROJECT_BINARY_DIR})

string(TOUPPER ${My_Project_Title} My_Project_Title_Caps  )
string(REGEX REPLACE " " "_" My_Project_Title_Caps ${My_Project_Title_Caps} )


# ======================
# Process Template Files
# ======================

file(READ ${PROJECT_SOURCE_DIR}/LICENSE.txt My_Project_License)

# string(REGEX REPLACE "\n" "\n\t" My_Project_License_Indent ${My_Project_License})

configure_file (
	"${PROJECT_SOURCE_DIR}/templates/template.c.in"
	"${PROJECT_BINARY_DIR}/template.c"
)

configure_file (
	"${PROJECT_SOURCE_DIR}/templates/template.h.in"
	"${PROJECT_BINARY_DIR}/template.h"
)

# Update the project README, to pull in new version #, etc.
configure_file (
	"${PROJECT_SOURCE_DIR}/templates/README.md.in"
	"${CMAKE_CURRENT_LIST_DIR}/README.md"
)

configure_file (
	"${PROJECT_SOURCE_DIR}/templates/version.h.in"
	"${PROJECT_BINARY_DIR}/version.h"
)


# ============
# Source Files
# ============

# Check whether arc4random() is available
INCLUDE (CheckFunctionExists)
check_function_exists(arc4random_uniform HAVE_ARC4RANDOM) 

if (HAVE_ARC4RANDOM)
	add_definitions(-DHAVE_ARC4RANDOM)
endif ()

# src_files are the primary files, and will be included in doxygen documentation
set(src_files
	src/shamir.c
)

# primary header files
set(header_files
	src/shamir.h
)

# Utility source files will not be included in doxygen
set(src_utility_files
	src/strtok.c
)

set(header_utility_files
	src/strtok.h
	${PROJECT_BINARY_DIR}/version.h
)

# Create a library?
add_library(libShamir STATIC
	${src_files}
	${src_utility_files}
	${header_files}
	${header_utility_files}
)

# remove the extra "lib" from "liblibFOO"
SET_TARGET_PROPERTIES(libShamir PROPERTIES PREFIX "")

# Create a binary?
if (NOT DEFINED TEST)
	add_executable(shamir
		src/main.c
		src/GLibFacade.c
		src/GLibFacade.h
		${header_files}
	)
	
	target_link_libraries(shamir libShamir)
endif()


# Generate doxygen configuration file
string(REPLACE ";" " " doxygen_src_files "${src_files}" )
string(REPLACE ";" " " doxygen_header_files "${header_files}" )

configure_file (
	"${PROJECT_SOURCE_DIR}/templates/doxygen.conf.in"
	"${PROJECT_BINARY_DIR}/doxygen.conf"
)


# ===========================================
# Build Test Suite with CuTest (unit testing)
# ===========================================

set(test_files
	test/CuTest.c
	test/CuTest.h
	${PROJECT_BINARY_DIR}/AllTests.c
)

if (DEFINED TEST)
	add_definitions(-DTEST)

	add_executable(run_tests
		${test_files}
		${src_files}
		${src_utility_files}
		${header_files}
		${header_utility_files}
	)

	# On some systems, need to specifically link m
	target_link_libraries(run_tests m)

	# Process source files to look for tests to run
	add_custom_command (
		OUTPUT ${PROJECT_BINARY_DIR}/AllTests.c
		COMMAND sh ${PROJECT_SOURCE_DIR}/test/make-tests.sh ${PROJECT_SOURCE_DIR}/src/*.c > ${PROJECT_BINARY_DIR}/AllTests.c
	)

	enable_testing()

	add_test( test ${PROJECT_BINARY_DIR}/run_tests)

	# valgrind memory testing
	find_program (MEMORYCHECK_COMMAND valgrind)
	SET (MEMORYCHECK_COMMAND_OPTIONS --leak-check=full --error-exitcode=1)

	add_test( memory_test ${MEMORYCHECK_COMMAND} ${MEMORYCHECK_COMMAND_OPTIONS} ${PROJECT_BINARY_DIR}/run_tests)

endif()


# =======================
# Configure for Target OS
# =======================

# OS X Builds
if (APPLE)

	# Configure backwards-compatible support (if your project allows it)
	SET(CMAKE_OSX_DEPLOYMENT_TARGET "10.4" CACHE STRING "Deployment target for OSX" FORCE)

	# Compile for x86_64 and i386.  ppc no longer supported
	if(CMAKE_BUILD_TYPE MATCHES "Release")
		SET (CMAKE_OSX_ARCHITECTURES x86_64;i386)
	endif(CMAKE_BUILD_TYPE MATCHES "Release")

	# Use PackageMaker for installers?
	if (DEFINED ZIP)
		set (CPACK_GENERATOR ZIP)
	else (DEFINED ZIP)
		set (CPACK_GENERATOR PackageMaker)
	endif (DEFINED ZIP)

endif (APPLE)

# Windows Builds
if (WIN32)

	# Use NSIS to generate installers?
	if (DEFINED ZIP)
		set (CPACK_GENERATOR ZIP)
	else (DEFINED ZIP)
		set (CPACK_GENERATOR NSIS)
	endif (DEFINED ZIP)

# Linux Builds (not cross-compiling for Windows)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

	# Create zip archive
	set (CPACK_GENERATOR ZIP)

	# Some libraries need to be linked on some Linux builds
	if (DEFINED TEST)
		target_link_libraries(run_tests m)

	if (DEFINED HAVE_ARC4RANDOM)
		target_link_libraries(run_tests bsd)
	endif (DEFINED HAVE_ARC4RANDOM)

	endif (DEFINED TEST)

	# On some systems, need to specifically link m (math)
	target_link_libraries(libShamir m)

	if (DEFINED HAVE_ARC4RANDOM)
		target_link_libraries(libShamir bsd)
	endif (DEFINED HAVE_ARC4RANDOM)

	# Statically link libraries -- might make the binary slightly more
	# compatible across Linux distributions, for example
	#
	# You may wish to disable this.
	#
	
	set (CMAKE_FIND_LIBRARY_SUFFIXES ".a")
	set (BUILD_SHARED_LIBRARIES OFF)
	set (CMAKE_EXE_LINKER_FLAGS "-static")

endif (WIN32)


# ==============
# Define targets
# ==============

# Create a library?
# add_library(libFOO STATIC
#	${src_files}
#	${src_utility_files}
#	${header_files}
#	${header_utility_files}
# )

# remove the extra "lib" from "liblibFOO"
# SET_TARGET_PROPERTIES(libFOO PROPERTIES PREFIX "")

# Create a command-line app?
# if (NOT DEFINED TEST)
# 	add_executable(main
# 		src/main.c
# 		src/GLibFacade.c
# 		src/GLibFacade.h
# 		${header_files}
# 	)
# 
#	Link the library to the app?
#	target_link_libraries(main libFOO)
# endif()

# Xcode settings for fat binaries
# set_target_properties(libFOO PROPERTIES XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH "NO")
# set_target_properties(main PROPERTIES XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH "NO")


# ==========================
# Build Installer with CPack
# ==========================

# You're largely on your own here

if (NOT DEFINED TEST)
	install (TARGETS shamir
		DESTINATION bin
	)
endif(NOT DEFINED TEST)

install (FILES ${CMAKE_CURRENT_LIST_DIR}/README.md ${PROJECT_SOURCE_DIR}/LICENSE.txt
	DESTINATION .
)

set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "${My_Project_Description}")
set (CPACK_PACKAGE_VENDOR "${My_Project_Author}")
set (CPACK_PACKAGE_VERSION "${My_Project_Version_Major}.${My_Project_Version_Minor}.${My_Project_Version_Patch}")
set (CPACK_PACKAGE_VERSION_MAJOR "${My_Project_Version_Major}")
set (CPACK_PACKAGE_VERSION_MINOR "${My_Project_Version_Minor}")
set (CPACK_PACKAGE_VERSION_PATCH "My_Project_Version_Patch")

set (CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE.txt")

if (APPLE)
	set (CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-Mac-${CPACK_PACKAGE_VERSION}")
else (APPLE)
	set (CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CMAKE_SYSTEM_NAME}-${CPACK_PACKAGE_VERSION}")
endif (APPLE)

# Use some default install locations (if appropriate)
if (WIN32)
	set (CPACK_INSTALL_PREFIX "")
	set (CPACK_NSIS_MODIFY_PATH ON)
else (WIN32)
	set (CPACK_INSTALL_PREFIX /usr/local)
endif (WIN32)

set (CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT})

include (CPack)
