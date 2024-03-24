cmake_minimum_required(VERSION 3.22)
project(perspective-projection LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Armadillo REQUIRED)
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable(perspective-projection
    camera.hpp camera.cpp
    polygon.hpp polygon.cpp
    polygon_projection.hpp polygon_projection.cpp
    keyboard_controls.hpp keyboard_controls.cpp
    mouse_controls.hpp mouse_controls.cpp
    main.cpp)
target_link_libraries(perspective-projection PRIVATE 
    sfml-graphics 
    ${ARMADILLO_LIBRARIES})