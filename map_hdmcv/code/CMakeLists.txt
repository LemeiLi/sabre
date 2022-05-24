cmake_minimum_required(VERSION 2.8.5) # setting this is required
project(hal C)                         # this sets the project name

include(GNUInstallDirs)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(HAL_VERSION_SO 1)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

set(custom_compiler_flags)
include(CheckCCompilerFlag)
option(ENABLE_CUSTOM_COMPILER_FLAGS "Enables custom compiler flags" ON)
if (ENABLE_CUSTOM_COMPILER_FLAGS)
    if (("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang") OR ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU"))
		# Appends elements to the list.(custom_compiler_flags)
        list(APPEND custom_compiler_flags
            -std=gnu11
            -Wall
            -Wextra
            -Wstrict-prototypes
            -Wwrite-strings
            -Wshadow
            -Winit-self
            -Wcast-align
            -Wformat=2
            -Wmissing-prototypes
            -Wstrict-overflow=2
            -Wcast-qual
            -Wundef
            -Wswitch-default
            -Wconversion
            -Wc++-compat
            -fstack-protector-strong
            -Wdouble-promotion
            -Wparentheses
            -Wformat-overflow
            -Wunused-macros
            -Wswitch-enum
        )
    elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
        # Disable warning c4001 - nonstandard extension 'single line comment' was used
        # Define _CRT_SECURE_NO_WARNINGS to disable deprecation warnings for "insecure" C library functions
        list(APPEND custom_compiler_flags
            /GS
            /Za
            /sdl
            /W4
            /wd4001
            /D_CRT_SECURE_NO_WARNINGS
        )
    endif()
endif()

# apply custom compiler flags
foreach(compiler_flag ${custom_compiler_flags})
    #remove problematic characters
    string(REGEX REPLACE "[^a-zA-Z0-9]" "" current_variable ${compiler_flag})

	check_c_compiler_flag(${compiler_flag} "FLAG_SUPPORTED_${current_variable}")
    if (FLAG_SUPPORTED_${current_variable})
        list(APPEND supported_compiler_flags)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${compiler_flag}")
    endif()
endforeach()

set(CMAKE_BUILD_TYPE Release)
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_FLAGS "-Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O3")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${supported_compiler_flags}")

###############################################################################
## include directory and subdir ###############################################
###############################################################################

include_directories(${PROJECT_SOURCE_DIR}/inc)

add_subdirectory(infra)
add_subdirectory(dal)
add_subdirectory(bll)
add_subdirectory(uil)

file(GLOB_RECURSE sources                                                             
	uil/*.c
	bll/*.c 
	dal/*.c 
	infra/*.c)

file(GLOB 3dsources 
	3dparty/cjson/*.c
	3dparty/zlog/src/*.c)

if(NOT WIN32)
	get_filename_component(full_path_zlog_win 
		${PROJECT_SOURCE_DIR}/3dparty/zlog/src/zlog_win.c ABSOLUTE)
	get_filename_component(full_path_zlog_chk_conf 
		${PROJECT_SOURCE_DIR}/3dparty/zlog/src/zlog_chk_conf.c ABSOLUTE)
	message(STATUS "remove " ${full_path_zlog_win})
	message(STATUS "remove " ${full_path_zlog_chk_conf})
	list(REMOVE_ITEM 3dsources ${full_path_zlog_win})
	list(REMOVE_ITEM 3dsources ${full_path_zlog_chk_conf})
	foreach(file IN LISTS 3dsources)
		message(STATUS ${file})
	endforeach(file)
endif()


###############################################################################
## target definitions #########################################################
###############################################################################
#link_directories(3dparty/cjson)
#link_directories(3dparty/zlog)


add_library(hal SHARED ${sources} ${3dsources})
add_library(hal_static STATIC ${sources} ${3dsources})
add_executable(test_image test/test_image.c)
add_executable(test_insgps test/test_insgps.c)
add_executable(test_lidar test/test_lidar.c)

target_compile_options(test_image PUBLIC -g -std=gnu11 -Wfloat-conversion -Wint-to-pointer-cast)
target_compile_options(test_insgps PUBLIC -g -std=gnu11 -Wfloat-conversion -Wint-to-pointer-cast)
target_compile_options(test_lidar PUBLIC -g -std=gnu11 -Wfloat-conversion -Wint-to-pointer-cast)
target_include_directories(test_image PUBLIC ./inc)
target_include_directories(test_insgps PUBLIC ./inc)
target_include_directories(test_lidar PUBLIC ./inc)

set_target_properties(hal
    PROPERTIES
	SOVERSION "${HAL_VERSION_SO}"
    VERSION "${PROJECT_VERSION}")

# This copies all resource files in the build directory.
# We need this, because we want to work with paths relative to the executable.
file(COPY ${data} DESTINATION resources)

###############################################################################
## dependencies ###############################################################
###############################################################################
target_link_libraries(test_image PRIVATE hal -lpthread)
target_link_libraries(test_insgps PRIVATE hal -lpthread)
target_link_libraries(test_lidar PRIVATE hal -lpthread -lpcap)

###############################################################################
## Debug Install ##############################################################
###############################################################################

# Binaries
install(TARGETS test_image DESTINATION install/bin)
install(TARGETS test_insgps DESTINATION install/bin)
install(TARGETS test_lidar DESTINATION install/bin)

# Libraries 
install(TARGETS hal LIBRARY DESTINATION install/lib)

# Head files
install(DIRECTORY ./inc/uil/ DESTINATION install/inc
	    FILES_MATCHING PATTERN "*.h")

# Config
install(DIRECTORY ./config DESTINATION install
	    FILES_MATCHING PATTERN "*.json")
install(DIRECTORY ./config DESTINATION install
		FILES_MATCHING PATTERN "*.conf")

# TestCase
install(DIRECTORY ./test DESTINATION install
		FILES_MATCHING PATTERN "*.c")

###############################################################################
## Release Install ##############################################################
###############################################################################

# Binaries
install(TARGETS test_image DESTINATION ../hal_release/bin)
install(TARGETS test_insgps DESTINATION ../hal_release/bin)
install(TARGETS test_lidar DESTINATION ../hal_release/bin)

# Libraries 
install(TARGETS hal LIBRARY DESTINATION ../hal_release/lib)

# Head files
install(DIRECTORY ./inc/uil/ DESTINATION ../hal_release/inc
	    FILES_MATCHING PATTERN "*.h")

# Config
install(DIRECTORY ./config DESTINATION ../hal_release
	    FILES_MATCHING PATTERN "*.json")
install(DIRECTORY ./config DESTINATION ../hal_release
		FILES_MATCHING PATTERN "*.conf")

# TestCase
install(DIRECTORY ./test DESTINATION ../hal_release
		FILES_MATCHING PATTERN "*.c")

# Now comes everything we need, to create a package
# there are a lot more variables you can set, and some
# you need to set for some package types, but we want to
# be minimal here.
set(CPACK_PACKAGE_NAME "hal_pack")
set(CPACK_PACKAGE_VERSION "1.0.0")

# We don't want to split our program up into several incomplete pieces.
set(CPACK_MONOLITHIC_INSTALL 1)

# This must be last
include(CPack)



