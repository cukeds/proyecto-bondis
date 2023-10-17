cmake_minimum_required(VERSION 3.23)
project(proyecto_bondi)

set(CMAKE_CXX_STANDARD 23)

include_directories(libs)

# add SFML-2.6.0
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "C:/SFML-2.6.0/lib/cmake/SFML")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)


add_executable(proyecto_bondi
        libs/Grafo.h
        libs/NodoGrafo.h
        src/main.cpp)


target_link_libraries(proyecto_bondi PUBLIC sfml-system sfml-graphics sfml-window)
