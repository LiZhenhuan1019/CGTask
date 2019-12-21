#include <exception>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cxxabi.h>
#include <sched.h>

#include "input/input_manager.hpp"
#include "render/render_manager.hpp"
#include "camera/free_camera.hpp"

CGTask::render::render_manager *render;
CGTask::input::input_manager *input;
CGTask::camera::free_camera *camera;
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
    glfwWindowHint(GLFW_DEPTH_BITS, 32);
    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LEQUAL);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glViewport(0, 0, width, height);
    try
    {
        CGTask::camera::free_camera camera(glm::vec3(200.f, 100.0f, 100.f), glm::vec3(-1.0f, -1.0f, -1.0f), width, height);
        CGTask::render::render_manager render_manager(1000, camera);
        CGTask::input::input_manager input_manager(window, render_manager, camera);
        ::camera = &camera;
        ::render = &render_manager;
        ::input = &input_manager;
        glfwSetFramebufferSizeCallback(window,
                [](GLFWwindow *, int new_width, int new_height)
                {
                    glViewport(0, 0, new_width, new_height);
                    if (::render)
                        ::render->set_size(new_width, new_height);
                    if (::camera)
                        ::camera->set_size(new_width, new_height);
                });
        glfwSetKeyCallback(window,
                [](GLFWwindow *window, int key, int scancode, int action ,int mods)
                {
                    if (::input)
                        input->input_callback(window, key, scancode, action, mods);
                });
        glfwSetCursorPosCallback(window,
                [](GLFWwindow *window, double xpos, double ypos)
                {
                    if (input)
                        input->mouse_callback(window, xpos, ypos);
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
