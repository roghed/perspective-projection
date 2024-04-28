find_package(Armadillo REQUIRED)
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable(perspective-projection
    camera.hpp camera.cpp
    polygon.hpp polygon.cpp
    polygon_projection.hpp polygon_projection.cpp
    keyboard_controls.hpp keyboard_controls.cpp
    mouse_controls.hpp mouse_controls.cpp
    bsp_tree.hpp bsp_tree.cpp
    object.hpp object.cpp
    obj_file_parser.hpp obj_file_parser.cpp
    scene.hpp scene.cpp
    vec.hpp vec.cpp
    main.cpp)
target_link_libraries(perspective-projection PRIVATE 
    sfml-graphics 
    ${ARMADILLO_LIBRARIES})
