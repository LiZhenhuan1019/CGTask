#include <exception>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cxxabi.h>

#include "input/input_manager.hpp"
#include "render/render_manager.hpp"
CGTask::render::render_manager *render;
int main()
{
    int width = 800, height = 600;
    glfwSetErrorCallback([](int error_code, char const *description)
            {
                std::cout << "error code: " << std::hex << error_code << 
                        " with description: '" << description << "'" << std::endl;
            });
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" <<std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(width, height, "CGTask", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialized GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, width, height);
    try
    {
        CGTask::input::input_manager input_manager;
        CGTask::render::render_manager render_manager(width, height);
        render = &render_manager;
        glfwSetFramebufferSizeCallback(window,
                [](GLFWwindow *, int new_width, int new_height)
                {
                    glViewport(0, 0, new_width, new_height);
                    if (render)
                        render->set_size(new_width, new_height);
                });


        while (!glfwWindowShouldClose(window))
        {
            input_manager.process_input(window);
            render_manager.render();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    catch(std::exception const &e)
    {
        std::cout << "Caught exception of type '" << 
            abi::__cxa_demangle(typeid(e).name(), nullptr, nullptr, nullptr) << "' in main.\n" <<
            "what(): `" << e.what() << "`" << std::endl;
    }
    glfwTerminate();
    return 0;
}
