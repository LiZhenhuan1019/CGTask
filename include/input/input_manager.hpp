#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace CGTask::input
{
    class input_manager
    {
    public:
        void process_input(GLFWwindow *window)
        {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);
        }
    };
}
