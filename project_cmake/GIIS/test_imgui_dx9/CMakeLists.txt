cmake_minimum_required(VERSION 3.5.0)
project(dx9_imgui_c VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(DXSDK_INCLUDE_DIR "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include")
set(DXSDK_LIB_DIR "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64")
set(STB_DIR ${CMAKE_SOURCE_DIR}/stb)
set(TIMAGE_DIR ${CMAKE_SOURCE_DIR}/TImage)


find_package(OpenCV REQUIRED)


add_library(imgui
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp
)

add_library(imgui_dx9
    imgui/backends/imgui_impl_dx9.cpp
    imgui/backends/imgui_impl_win32.cpp
)

include_directories(${DXSDK_INCLUDE_DIR})
link_directories(${DXSDK_LIB_DIR})

include_directories(${STB_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${TIMAGE_DIR})

target_include_directories(imgui PUBLIC imgui)
target_include_directories(imgui_dx9 PUBLIC imgui)


add_executable(dx9_imgui_c main.cpp)


target_link_libraries(dx9_imgui_c 
    PUBLIC ${OpenCV_LIBS}
    PUBLIC imgui
    PUBLIC imgui_dx9
    PUBLIC d3d9.lib
    PUBLIC d3dx9.lib
)






set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
