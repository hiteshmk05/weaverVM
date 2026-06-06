cmake_minimum_required(VERSION 3.20)

project(weaver-vm VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(raylib CONFIG REQUIRED)

add_executable(weaver-vm
    src/main.cpp
    src/chip8/chip8.cpp
    src/display/display.cpp
)

target_include_directories(weaver-vm
    PRIVATE
        src
)

target_link_libraries(weaver-vm
    PRIVATE
        raylib
)