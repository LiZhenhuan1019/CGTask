#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <bitset>
#include <utility>
#include "input_manager.hpp"
#include "model/model_manager_task2.hpp"
#include "render/render_manager.hpp"
#include "camera/free_camera.hpp"
#include "input/timer.hpp"

namespace CGTask::input
{
    class input_manager_task2 : public input_manager
    {
    public:
        input_manager_task2(GLFWwindow *window, render::render_manager &render, camera::free_camera &camera, model::model_manager_task2 &model, input::delta_timer &timer)
            :render(render), camera(camera), model(model), timer(timer)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            if (glfwRawMouseMotionSupported())
                glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
            glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
        }
        void process_input(GLFWwindow *window) override
        {
            timer.update();
            
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);

            std::bitset<6> input;
            bool accelerate = false;
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
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                accelerate = true;
            float speed = accelerate ? 200 : 10;
            if (input.any())
                camera.move(input, timer.delta_time() * speed);
        }
        void input_callback(GLFWwindow * /*window*/, int key, int /*cancode*/, int action, int /*mods*/) override
        {
            if (action == GLFW_PRESS)
            {
                switch (key)
                {
                case GLFW_KEY_0:
                    // 1:1 speed
                    model.set_speed(1.0 / (24 * 60 * 60));
                    break;
                case GLFW_KEY_1:
                    // 1 second corresponds to 1 minute
                    model.set_speed(1.0 / (24 * 60));
                    break;
                case GLFW_KEY_2:
                    // 1 second corresponds to 1 hour
                    model.set_speed(1.0 / 24);
                    break;
                case GLFW_KEY_3:
                    // 1 second to 6 hours (1/4 day)
                    model.set_speed(1.0 / 4);
                    break;
                case GLFW_KEY_4:
                    // 1 second to 1 day
                    model.set_speed(1);
                    break;
                case GLFW_KEY_5:
                    // 1 second to 1 week
                    model.set_speed(7);
                    break;
                case GLFW_KEY_6:
                    // 1 second to 30 days
                    model.set_speed(30);
                    break;
                case GLFW_KEY_7:
                    // 1 second to 90 days
                    model.set_speed(90);
                    break;
                case GLFW_KEY_8:
                    // 1 second to 1 year
                    model.set_speed(365.25);
                    break;
                case GLFW_KEY_9:
                    // 1 second to 5 years
                    model.set_speed(365.25 * 5);
                    break;
                case GLFW_KEY_SPACE:
                    model.toggle_suspend();
                    break;
                case GLFW_KEY_P:
                    camera.toggle_projection();
                    break;
                }
            }
        }
        void mouse_callback(GLFWwindow * /*window*/, double xpos, double ypos) override
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
        void scroll_callback(GLFWwindow *, double /*xoffset*/, double yoffset) override
        {
            yoffset /= 5;
            double factor = 0;
            if (yoffset >= 0)
                factor = yoffset + 1;
            else
                factor = 1 / (-yoffset + 1);
            scale = std::clamp(scale * factor, 0.0, 1000.0);
            camera.orthographic_scale(scale);
        }
    private:
        delta_timer &timer;
        [[maybe_unused]] render::render_manager &render;
        camera::free_camera &camera;
        model::model_manager_task2 &model;

        bool first_mouse = true;
        double last_x = 0, last_y = 0;
        double scale = 1;
    };
}
