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
        input_manager(GLFWwindow *window, render::render_manager &render, camera::free_camera &camera)
            :render(render), camera(camera)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
        }
        void process_input(GLFWwindow *window)
        {
            timer.update();
            
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);

            std::bitset<6> input;
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                input.set(camera::input_enum::forward);
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                input.set(camera::input_enum::backward);
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                input.set(camera::input_enum::left);
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                input.set(camera::input_enum::right);
            if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
                input.set(camera::input_enum::up);
            if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
                input.set(camera::input_enum::down);
            if (input.any())
                camera.move(input, timer.delta_time() * 100);
        }
        void input_callback(GLFWwindow * /*window*/, int key, int /*cancode*/, int action, int /*mods*/)
        {
            if (key == GLFW_KEY_P && action == GLFW_PRESS)
            {
                camera.toggle_projection();
            }
        }
        void mouse_callback(GLFWwindow * /*window*/, double xpos, double ypos)
        {
            if (first_mouse)
            {
                first_mouse = false;
                last_x = xpos;
                last_y = ypos;
            }
            double x_offset = xpos - std::exchange(last_x, xpos);
            double y_offset = -(ypos - std::exchange(last_y, ypos));
            double sensitivity = 0.05f;
            x_offset *= sensitivity;
            y_offset *= sensitivity;
            camera.turn(x_offset, y_offset);
        }
    private:
        delta_timer timer;
        [[maybe_unused]] render::render_manager &render;
        camera::free_camera &camera;

        bool first_mouse = true;
        double last_x = 0, last_y = 0;
    };
}
