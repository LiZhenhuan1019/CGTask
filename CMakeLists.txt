cmake_minimum_required(VERSION 3.16)
project(CGTask)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

configure_file("assets/shaders/2d_demo.vert" "assets/shaders/2d_demo.vert" COPYONLY)
configure_file("assets/shaders/2d_demo.frag" "assets/shaders/2d_demo.frag" COPYONLY)

set(SOURCES 
    src/glad.c
    src/main.cpp)
set(HEADERS 
    include/glad/glad.h
    include/KHR/khrplatform.h
    include/input/input_manager.hpp
    include/model/2d/ellipse.hpp
    include/render/render_manager.hpp
    include/render/shader_manager.hpp)

add_executable(task1 src/main.cpp src/glad.c include/glad/glad.h include/KHR/khrplatform.h)
target_include_directories(task1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include PRIVATE ${GLFW_INCLUDE_DIRS})
target_link_libraries(task1 PRIVATE ${GLFW_LIBRARIES} dl)
#target_compile_options(task1 PRIVATE -Werror -Wall -Wextra)
