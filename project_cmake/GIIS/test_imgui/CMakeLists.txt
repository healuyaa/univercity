cmake_minimum_required(VERSION 3.5.0)
project(test_imgui_lib)


set(GLFW_DIR ${CMAKE_SOURCE_DIR}/glfw)

find_package(OpenGL REQUIRED)


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


include_directories(${GLFW_DIR}/include)
link_directories(${GLFW_DIR}/libs)


target_include_directories(imgui PUBLIC imgui)
target_include_directories(imgui_opengl_impl PRIVATE imgui)


add_executable(${PROJECT_NAME} main.cpp)


target_link_libraries(${PROJECT_NAME} PRIVATE imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui_opengl_impl)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw3)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})
