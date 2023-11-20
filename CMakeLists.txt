cmake_minimum_required(VERSION 3.9)

project(ministl VERSION 0.1 LANGUAGES CXX)

include(util.cmake)

set(MINISTL_PUBLIC_INCLUDE_DIR "include")
set(EXE "ministl")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

green_message("BUILD LEVEL: ${BUILD_LEVEL}")

add_executable(${EXE} main.cc)

target_include_directories(${EXE} PUBLIC ${MINISTL_PUBLIC_INCLUDE_DIR})

if (BUILD_LEVEL STREQUAL "Debug")
    target_compile_options(${EXE} PRIVATE -g)
    add_definitions(-DDEBUG)
endif()
