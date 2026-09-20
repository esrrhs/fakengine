cmake_minimum_required(VERSION 3.16)

project(cmdcontroller LANGUAGES CXX)

add_executable(cmdcontroller main.cpp)
target_link_libraries(cmdcontroller PRIVATE fengine)
