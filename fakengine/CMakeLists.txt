cmake_minimum_required(VERSION 3.16)

project(FENGINE LANGUAGES C CXX)

file(GLOB FENGINE_CORE_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

file(GLOB_RECURSE FENGINE_THIRDPARTY_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/lua/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/zlib/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lzo/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/expat/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/tinyxml/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/sqlite/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/pcre/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/pcre/*.cc"
)
list(FILTER FENGINE_THIRDPARTY_SRCS EXCLUDE REGEX ".*/sqlite/shell\\.c$")

add_library(fengine STATIC ${FENGINE_CORE_SRCS} ${FENGINE_THIRDPARTY_SRCS})

target_include_directories(fengine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_compile_definitions(fengine PRIVATE
    -DHAVE_EXPAT_CONFIG_H
    -DHAVE_CONFIG_H
)

if(TARGET bundled_fkscript AND TARGET bundled_mysqlclient)
    target_link_libraries(fengine PUBLIC
        bundled_fkscript
        bundled_mysqlclient
    )
endif()

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