#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <utility>
#include <optional>

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
                update_diff();
            suspended = !suspended;
        }
        // to solve precision problem when speed is to large.
        void set_period(double p)
        {
            period = p;
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
            return glfwGetTime() + diff;
        }
        void update_diff()
        {
            diff = current_time_ - glfwGetTime();
        }
        double delta_time_ = 0;
        double current_time_ = get_time();
        bool suspended = false;
        double diff = 0;
        std::optional<double> period;
    };
}
