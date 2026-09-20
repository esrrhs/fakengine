cmake_minimum_required(VERSION 3.16)

project(test_runner LANGUAGES CXX)

# 1. Legacy test suite (mainapp based)
file(GLOB LEGACY_TEST_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*/*.cpp"
)
list(FILTER LEGACY_TEST_SRCS EXCLUDE REGEX ".*/unit/.*")

add_executable(faketest ${LEGACY_TEST_SRCS})
set_target_properties(faketest PROPERTIES OUTPUT_NAME "test")
target_include_directories(faketest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(faketest PRIVATE fengine)

set(TEST_CASES
    hashmapapp
    compressapp
    inifileapp
    ftrieapp
    fstringapp
    fpoolapp
    xmlapp
    sqliteapp
    fallocapp
    fkeybufferapp
    fmd5app
    fpointapp
    pcreapp
)

if(TARGET bundled_fkscript)
    target_compile_definitions(faketest PRIVATE HAVE_FAKESCRIPT)
    list(APPEND TEST_CASES fakeapp)
endif()

if(TARGET bundled_mysqlclient)
    target_compile_definitions(faketest PRIVATE HAVE_MYSQL)
endif()

foreach(tc ${TEST_CASES})
    add_test(NAME "legacy_${tc}" COMMAND faketest ${tc} --once)
endforeach()

# 2. Modern GoogleTest unit tests
find_package(GTest QUIET)
if(NOT GTest_FOUND)
    if(EXISTS "/usr/local/include/gtest/gtest.h" AND EXISTS "/usr/local/lib64/libgtest.a")
        add_library(GTest::gtest STATIC IMPORTED GLOBAL)
        set_target_properties(GTest::gtest PROPERTIES
            IMPORTED_LOCATION "/usr/local/lib64/libgtest.a"
            INTERFACE_INCLUDE_DIRECTORIES "/usr/local/include"
        )
        add_library(GTest::gtest_main STATIC IMPORTED GLOBAL)
        set_target_properties(GTest::gtest_main PROPERTIES
            IMPORTED_LOCATION "/usr/local/lib64/libgtest_main.a"
            INTERFACE_INCLUDE_DIRECTORIES "/usr/local/include"
        )
        set(GTest_FOUND TRUE)
    else()
        include(FetchContent)
        FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        )
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(googletest)
    endif()
endif()

if(TARGET GTest::gtest)
    file(GLOB UNIT_TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/unit/*.cpp")
    add_executable(unit_tests ${UNIT_TEST_SRCS})
    target_include_directories(unit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(unit_tests PRIVATE fengine GTest::gtest)
    add_test(NAME "modern_unit_tests" COMMAND unit_tests)
endif()
