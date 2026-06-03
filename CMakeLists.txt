cmake_minimum_required(VERSION 3.20)
project(retro-vm LANGUAGES CXX)

# if(DEFINED ENV{VCPKG_ROOT})
#     set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
# elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
#     set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
# else()
#     message(FATAL_ERROR "vcpkg not found. Either set VCPKG_ROOT or clone vcpkg into the project root.")
# endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_package(rasylib)

add_executable(retro-vm src/main.cpp src/chip8/chip8.cpp)

target_include_directories(retro-vm PRIVATE src/)
# target_link_libraries(retro-vm PRIVATE raylib)

