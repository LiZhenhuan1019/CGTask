cmake_minimum_required(VERSION 3.16)
project(CGTask)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

configure_file("assets/shaders/2d_nocolor.vert" "assets/shaders/2d_nocolor.vert" COPYONLY)
configure_file("assets/shaders/2d_uniform_color.frag" "assets/shaders/2d_uniform_color.frag" COPYONLY)
configure_file("assets/shaders/2d_color.vert" "assets/shaders/2d_color.vert" COPYONLY)
configure_file("assets/shaders/2d_vertex_color.frag" "assets/shaders/2d_vertex_color.frag" COPYONLY)

add_executable(task1 src/task1.cpp src/glad.c )
target_include_directories(task1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include PRIVATE ${GLFW_INCLUDE_DIRS})
target_link_libraries(task1 PRIVATE ${GLFW_LIBRARIES} dl)
#target_compile_options(task1 PRIVATE -Werror -Wall -Wextra)

add_executable(task2 src/task2.cpp src/glad.c )
target_include_directories(task2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include PRIVATE ${GLFW_INCLUDE_DIRS})
target_link_libraries(task2 PRIVATE ${GLFW_LIBRARIES} dl)

