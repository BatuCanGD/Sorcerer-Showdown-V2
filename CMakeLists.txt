cmake_minimum_required(VERSION 3.28)
project(SorcererShowdownV2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(json GIT_REPOSITORY https://github.com/nlohmann/json.git GIT_TAG v3.11.3 GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(json)
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Sorcerer Showdown V2/source")
set(HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Sorcerer Showdown V2/header")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${SOURCE_DIR}/*.cpp")
add_executable(SorcererShowdownV2 ${SOURCES})
target_include_directories(SorcererShowdownV2 PRIVATE "${HEADER_DIR}")
target_link_libraries(SorcererShowdownV2 PRIVATE nlohmann_json::nlohmann_json)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/characters.json")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/characters.json" "${CMAKE_CURRENT_BINARY_DIR}/characters.json" COPYONLY)
endif()

if(MSVC)
    target_compile_options(SorcererShowdownV2 PRIVATE 
        /EHsc /utf-8 /W4 /permissive-
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(SorcererShowdownV2 PRIVATE
        -Wall -Wextra -Wpedantic -Wshadow -O3 -Wno-unused-parameter -Wno-missing-field-initializers
    )
endif()