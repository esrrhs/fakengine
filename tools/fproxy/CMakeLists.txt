cmake_minimum_required(VERSION 3.16)

project(fproxy LANGUAGES CXX)

file(GLOB FPROXY_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(fproxy ${FPROXY_SRCS})
target_include_directories(fproxy PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(fproxy PRIVATE fengine)
