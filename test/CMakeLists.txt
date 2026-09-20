cmake_minimum_required(VERSION 3.16)

project(test_runner LANGUAGES CXX)

file(GLOB_RECURSE TEST_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(faketest ${TEST_SRCS})
set_target_properties(faketest PROPERTIES OUTPUT_NAME "test")
target_include_directories(faketest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(faketest PRIVATE fengine)

# Register CTest automated tests
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
    fakeapp
)

foreach(tc ${TEST_CASES})
    add_test(NAME "test_${tc}" COMMAND faketest ${tc} --once)
endforeach()
