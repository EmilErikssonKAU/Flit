cmake_minimum_required(VERSION 3.13)

# Might change strictness later
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Pre-release version
project(Flit VERSION 0.1)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(Flit ${SOURCE_FILES})

target_include_directories(Flit PRIVATE src include)

target_compile_options(Flit PRIVATE -Wall)
