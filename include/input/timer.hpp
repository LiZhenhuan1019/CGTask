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
            if (wrap_time())
                return;
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
        void speed(double new_speed)
        {
            speed_ = new_speed;
            update_diff();
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
            return speed_ * glfwGetTime() + diff;
        }
        void update_diff()
        {
            if (wrap_time())
                return;
            diff = current_time_ - speed_ * glfwGetTime();
        }
        bool wrap_time()
        {
            double time = glfwGetTime();
            if (period && speed_ * time > *period)
            {
                int circle = speed_ * time / *period;
                double change = circle * *period;
                double new_time = time - change / speed_;
                glfwSetTime(new_time);
                circle = current_time_ / *period;
                current_time_ -= circle * *period;
                diff = current_time_ - speed_ * new_time;
                return true;
            }
            return false;
        }
        double delta_time_ = 0;
        double current_time_ = get_time();
        bool suspended = false;
        double diff = 0;
        double speed_ = 1;
        std::optional<double> period;
    };
}
