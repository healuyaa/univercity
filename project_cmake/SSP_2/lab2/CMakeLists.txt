cmake_minimum_required(VERSION 3.5.0)
project(giis_lab2 VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

set(DXSDK_INCLUDE_DIR "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include")
set(DXSDK_LIB_DIR "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64")
set(SQLite_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sqlite)
set(VCardSQLite_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/HospitalSQLite)

file(GLOB SRC_FILES
    main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vCard/src/*.cpp
)


add_library(sqlite3 STATIC ${SQLite_SOURCE_DIR}/sqlite3.c)

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

include_directories(${VCardSQLite_SOURCE_DIR})


target_include_directories(sqlite3 PUBLIC ${SQLite_SOURCE_DIR})
target_include_directories(imgui PUBLIC imgui)
target_include_directories(imgui_dx9 PUBLIC imgui)


add_executable(giis_lab2 ${SRC_FILES})


target_link_libraries(${PROJECT_NAME} 
    PRIVATE sqlite3
    PRIVATE imgui
    PRIVATE imgui_dx9
    PUBLIC d3d9.lib
    PUBLIC d3dx9.lib
)



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
