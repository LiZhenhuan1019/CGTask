#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <bitset>
#include <utility>
#include "render/render_manager.hpp"
#include "camera/free_camera.hpp"
#include "input/timer.hpp"

namespace CGTask::input
{
    class input_manager
    {
    public:
        virtual ~input_manager() = default;
        virtual void process_input(GLFWwindow *window) = 0;
        virtual void input_callback(GLFWwindow * /*window*/, int key, int /*cancode*/, int action, int /*mods*/) = 0;
        virtual void mouse_callback(GLFWwindow * /*window*/, double xpos, double ypos) = 0;
        virtual void scroll_callback(GLFWwindow *, double /*xoffset*/, double yoffset) = 0;
    };
}
