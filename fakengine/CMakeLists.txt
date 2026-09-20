cmake_minimum_required(VERSION 3.16)

project(FENGINE LANGUAGES C CXX)

add_library(fengine INTERFACE)

target_include_directories(fengine INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

if(TARGET ZLIB::ZLIB)
    target_link_libraries(fengine INTERFACE ZLIB::ZLIB)
endif()

if(WIN32)
    target_link_libraries(fengine INTERFACE ws2_32 shlwapi)
else()
    target_link_libraries(fengine INTERFACE
        dl
        pthread
        rt
    )
endif()