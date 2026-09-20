cmake_minimum_required(VERSION 3.16)

project(fakengine VERSION 1.177 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose build type: Debug Release RelWithDebInfo MinSizeRel" FORCE)
endif()

# Output directories matching original repository layout
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")

# Common compiler options
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(
        -m64
        -Wall
        -gdwarf-2
        -Wno-unused-parameter
        -Wno-unused-variable
        -Wno-unused-function
        -Wno-unused-but-set-variable
        -Wno-misleading-indentation
        -Wno-class-memaccess
        -Wno-stringop-truncation
    )
    add_compile_options("$<$<COMPILE_LANGUAGE:C>:-fwrapv>")
    add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:-fwrapv>")
endif()

add_compile_definitions(_USE_MATH_DEFINES)

if(WIN32)
    add_compile_definitions(WIN32 _WIN32 NOMINMAX)
endif()

# ZLIB support
find_package(ZLIB QUIET)
if(TARGET ZLIB::ZLIB)
    add_compile_definitions(HAVE_ZLIB)
endif()

# Subdirectories
add_subdirectory(fakengine)
add_subdirectory(tools)

# Testing
enable_testing()
add_subdirectory(test)
