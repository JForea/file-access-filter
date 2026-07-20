cmake_minimum_required(VERSION 3.10.0)
project(file-access-driver VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(
    faf
    user_application/src/main.cpp
    user_application/src/Helpers.cpp
    user_application/src/IoctlHelpers.cpp
    user_application/src/FileHelpers.cpp
)

target_include_directories(
    faf
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

include(CTest)
enable_testing()
