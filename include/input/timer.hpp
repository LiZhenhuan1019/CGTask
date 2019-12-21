#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace CGTask::input
{
    class delta_timer
    {
    public:
        delta_timer()
        {
            last_time = glfwGetTime();
        }
        void update()
        {
            double current_time = glfwGetTime();
            delta_time_ = current_time - last_time;
            last_time = current_time;
        }
        double delta_time()
        {
            return delta_time_;
        }
    private:
        double last_time;
        double delta_time_;
    };
}
