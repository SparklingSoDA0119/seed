cmake_minimum_required(VERSION 2.8)

project(seed)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)

message(Project Name : ${CMAKE_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_COMPILER "g++")


set(ROOT ..)

set(OUTPUT libseed)

ADD_COMPILE_OPTIONS(-g -Wall)
set(CMAKE_EXE_LINKER_FLAGS "-static -Wl, --gc-section")

file(GLOB COMMON_SOURCE "Common/*.h" "Common/*.cpp")
include_directories(.)
add_library(${OUTPUT} SHARED ${COMMON_SOURCE})
