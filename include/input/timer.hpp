#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <utility>

namespace CGTask::input
{
    class delta_timer
    {
        // current_time_ = speed_ * glfwGetTime() + diff;
        // y = s*x + d;
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
                diff = current_time_ - speed_ * glfwGetTime();
            suspended = !suspended;
        }
        void speed(double new_speed)
        {
            speed_ = new_speed;
            diff = current_time_ - speed_ * glfwGetTime();
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
            return speed_ * glfwGetTime() + diff;
        }
        bool suspended = false;
        double diff = 0;
        double delta_time_ = 0;
        double speed_ = 1;
        double current_time_ = get_time();
    };
}
