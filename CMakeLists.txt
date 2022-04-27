cmake_minimum_required(VERSION 3.13)  # CMake version check

project(sockets-tcp-http)              # Create project

set(CMAKE_CXX_STANDARD 14)            # Enable c++14 standard

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCE_FILES_SOCKETS sockets.cpp)

SET(GCC_COVERAGE_COMPILE_FLAGS "-static ")
SET(GCC_COVERAGE_LINK_FLAGS    "")

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(sockets ${SOURCE_FILES_SOCKETS})

# Link ws2_32 socket libraries to the executible
TARGET_LINK_LIBRARIES(sockets ws2_32)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
