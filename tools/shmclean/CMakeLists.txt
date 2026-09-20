cmake_minimum_required(VERSION 3.16)

project(shmclean LANGUAGES CXX)

add_executable(shmclean main.cpp)
target_link_libraries(shmclean PRIVATE fengine)
