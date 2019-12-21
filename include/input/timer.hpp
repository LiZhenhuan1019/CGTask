#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <utility>

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
            current_time_ = glfwGetTime();
            delta_time_ = current_time_ - std::exchange(last_time, current_time_);
        }
        double delta_time()
        {
            return delta_time_;
        }
        double current()
        {
            return current_time_;
        }
    private:
        double last_time;
        double delta_time_;
        double current_time_;
    };
}
