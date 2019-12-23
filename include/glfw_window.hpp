#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cxxabi.h>
#include "input/input_manager.hpp"
#include "render/render_manager.hpp"
#include "camera/free_camera.hpp"

namespace CGTask
{
    GLFWwindow *make_window(std::size_t width, std::size_t height)
    {
        glfwSetErrorCallback([](int error_code, char const *description)
                {
                    std::cout << "error code: " << std::hex << error_code << 
                            " with description: '" << description << "'" << std::endl;
                });
        if (!glfwInit())
        {
            std::cout << "Failed to initialize GLFW" <<std::endl;
            return nullptr;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow *window = glfwCreateWindow(width, height, "CGTask", nullptr, nullptr);
        if (!window)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return nullptr;
        }
        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialized GLAD" << std::endl;
            return nullptr;
        }
        glViewport(0, 0, width, height);
        return window;
    }
    inline CGTask::camera::free_camera *camera_ptr;
    inline CGTask::render::render_manager *render_ptr;
    inline CGTask::input::input_manager *input_ptr;

    void set_callback(GLFWwindow *window,
            CGTask::camera::free_camera &camera,
            CGTask::render::render_manager &render,
            CGTask::input::input_manager &input)
    {
        CGTask::camera_ptr = &camera;
        CGTask::render_ptr = &render;
        CGTask::input_ptr = &input;
        glfwSetFramebufferSizeCallback(window,
                [](GLFWwindow *, int new_width, int new_height)
                {
                    glViewport(0, 0, new_width, new_height);
                    if (CGTask::camera_ptr)
                        CGTask::camera_ptr->set_size(new_width, new_height);
                });
        glfwSetKeyCallback(window,
                [](GLFWwindow *window, int key, int scancode, int action ,int mods)
                {
                    if (CGTask::input_ptr)
                        CGTask::input_ptr->input_callback(window, key, scancode, action, mods);
                });
        glfwSetCursorPosCallback(window,
                [](GLFWwindow *window, double xpos, double ypos)
                {
                    if (CGTask::input_ptr)
                        CGTask::input_ptr->mouse_callback(window, xpos, ypos);
                });
        glfwSetScrollCallback(window, 
                [](GLFWwindow *window, double xoffset, double yoffset)
                {
                    if (CGTask::input_ptr)
                        CGTask::input_ptr->scroll_callback(window, xoffset, yoffset);
                });

    }
}

