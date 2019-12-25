cmake_minimum_required(VERSION 3.16)
project(CGTask)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
pkg_search_module(ASSIMP REQUIRED assimp)

configure_file("assets/shaders/uniform_color.vert" "assets/shaders/uniform_color.vert" COPYONLY)
configure_file("assets/shaders/uniform_color.frag" "assets/shaders/uniform_color.frag" COPYONLY)
configure_file("assets/shaders/vertex_color.vert" "assets/shaders/vertex_color.vert" COPYONLY)
configure_file("assets/shaders/vertex_color.frag" "assets/shaders/vertex_color.frag" COPYONLY)
configure_file("assets/shaders/color_with_texture.vert" "assets/shaders/color_with_texture.vert" COPYONLY)
configure_file("assets/shaders/color_with_texture.frag" "assets/shaders/color_with_texture.frag" COPYONLY)
configure_file("assets/shaders/texture_lighting.vert" "assets/shaders/texture_lighting.vert" COPYONLY)
configure_file("assets/shaders/texture_lighting.frag" "assets/shaders/texture_lighting.frag" COPYONLY)


configure_file("assets/texture/container.jpg" "assets/texture/container.jpg" COPYONLY)

configure_file("assets/model/earth/Earth 2K.obj" "assets/model/earth/Earth 2K.obj" COPYONLY)
configure_file("assets/model/earth/Textures/Diffuse_2K.png" "assets/model/earth/Textures/Diffuse_2K.png" COPYONLY)
configure_file("assets/model/earth/Textures/Clouds_2K.png" "assets/model/earth/Textures/Clouds_2K.png" COPYONLY)

add_executable(task1 src/task1.cpp src/glad.c )
target_include_directories(task1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include PRIVATE ${GLFW_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIRS})
target_link_libraries(task1 PRIVATE ${GLFW_LIBRARIES} dl ${ASSIMP_LIBRARIES})
#target_compile_options(task1 PRIVATE -Werror -Wall -Wextra)

add_executable(task2 src/task2.cpp src/glad.c )
target_include_directories(task2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include PRIVATE ${GLFW_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIRS})
target_link_libraries(task2 PRIVATE ${GLFW_LIBRARIES} dl ${ASSIMP_LIBRARIES})

