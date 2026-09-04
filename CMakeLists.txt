cmake_minimum_required(VERSION 3.28)
project(SorcererShowdownV2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Sorcerer Showdown V2/source")
set(HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Sorcerer Showdown V2/header")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${SOURCE_DIR}/*.cpp"
)

add_executable(SorcererShowdownV2 ${SOURCES})

target_include_directories(SorcererShowdownV2 PRIVATE
    "${HEADER_DIR}"
)

if(MSVC)
    target_compile_options(SorcererShowdownV2 PRIVATE
        /EHsc /utf-8 /W4 /permissive-
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(SorcererShowdownV2 PRIVATE
        -Wall -Wextra -Wpedantic -Wshadow -Wno-unused-parameter -O2
    )
endif()