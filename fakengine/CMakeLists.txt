cmake_minimum_required(VERSION 3.16)

project(FENGINE LANGUAGES C CXX)

file(GLOB FENGINE_CORE_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(fengine STATIC ${FENGINE_CORE_SRCS})

target_include_directories(fengine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

if(TARGET ZLIB::ZLIB)
    target_link_libraries(fengine PUBLIC ZLIB::ZLIB)
endif()

if(WIN32)
    target_link_libraries(fengine PUBLIC ws2_32 shlwapi)
else()
    target_link_libraries(fengine PUBLIC
        dl
        pthread
        rt
    )
endif()