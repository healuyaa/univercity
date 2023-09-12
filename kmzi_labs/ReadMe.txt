CMakeLists.txt - файл настроек для cmake проектов

Минимальный набор джентельмена для простого проекта:

cmake_minimum_required(VERSION 3.5.0)
project("имя_проекта")
add_executable(${PROJECT_NAME} main.cpp) <- ${PROJECT_NAME} - тоже самое что и "имя_проекта", просто удобнее такую конструкцию писать и не парится

Мой cmake:

cmake_minimum_required(VERSION 3.5.0)
project(kmzi_labs)

add_executable(${PROJECT_NAME} "./lab1/kmzi_lab1.cpp") <- здесь указан путь к cpp-шнику, который запакуется в 1 исходный exe-шник, у него имя всегда будет kmzi_labs.exe
kmzi_labs.exe <- всегда находиться в папке build/Debug

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} ntl)
Это три таргета для подключения NTL