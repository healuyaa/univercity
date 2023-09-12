cmake_minimum_required(VERSION 3.5.0)
project(kmzi_labs)

add_executable(${PROJECT_NAME} "main.cpp")
# ./lab1/kmzi_lab1.cpp

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} ntl)