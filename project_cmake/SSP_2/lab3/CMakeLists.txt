cmake_minimum_required(VERSION 3.5.0)
project(bicycle-rider VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sfml/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

set(LOAD_TEXTURES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/LoadTextures")

add_executable(bicycle-rider bicycle-rider.cpp)

target_link_libraries(bicycle-rider sfml-graphics sfml-window sfml-system)

include_directories(${LOAD_TEXTURES_DIR})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
