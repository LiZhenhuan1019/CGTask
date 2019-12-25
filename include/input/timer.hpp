#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <utility>

namespace CGTask::input
{
    class delta_timer
    {
    public:
        void update()
        {
            if (!suspended)
            {
                double new_time = get_time(); 
                delta_time_ = new_time - std::exchange(current_time_, new_time);
            }
        }
        void toggle_suspend()
        {
            if (suspended)
                suspend_amount = glfwGetTime() - current_time_;
            suspended = !suspended;
        }
        double delta_time() const
        {
            return delta_time_;
        }
        double current() const
        {
            return current_time_;
        }
    private:
        double get_time()
        {
            return glfwGetTime() - suspend_amount;
        }
        bool suspended = false;
        double suspend_amount = 0;
        double delta_time_ = 0;
        double current_time_ = get_time();
    };
}
