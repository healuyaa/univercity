cmake_minimum_required(VERSION 3.5.0)
project(remove-noisy)


set(GLFW_DIR ${CMAKE_SOURCE_DIR}/glfw)
set(STB_DIR ${CMAKE_SOURCE_DIR}/stb_images.h)


find_package(OpenGL REQUIRED)
find_package(OpenCV REQUIRED)


add_library(imgui
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp
)

add_library(imgui_opengl_impl
    imgui/backends/imgui_impl_opengl3.cpp
    imgui/backends/imgui_impl_glfw.cpp
)


include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(${STB_DIR})

include_directories(${GLFW_DIR}/include)
link_directories(${GLFW_DIR}/libs)


target_include_directories(imgui PUBLIC imgui)
target_include_directories(imgui_opengl_impl PUBLIC imgui)


add_executable(remove-noisy main.cpp)


target_link_libraries(remove-noisy
    PUBLIC ${OpenCV_LIBS}
    PUBLIC imgui
    PUBLIC imgui_opengl_impl
    PUBLIC glfw3
    PUBLIC ${OPENGL_LIBRARIES}
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
