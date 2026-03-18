cmake_minimum_required(VERSION 3.13)

# Might change strictness later
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Pre-release version
project(Flit VERSION 0.1)

include(FetchContent)

find_package(CLI11 CONFIG QUIET)

if(NOT CLI11_FOUND)
    FetchContent_Declare(
        CLI11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
        GIT_TAG v2.6.1
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(CLI11)
endif()

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(Flit ${SOURCE_FILES})

target_include_directories(Flit PRIVATE src include)
target_link_libraries(Flit PRIVATE CLI11::CLI11)

target_compile_options(Flit PRIVATE -Wall)
