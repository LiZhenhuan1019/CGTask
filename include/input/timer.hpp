#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <utility>
#include <optional>
#include <chrono>

namespace CGTask::input
{
    class delta_timer
    {
        // current_time_ = speed_ * glfwGetTime() + diff;
        // y = s*x + d;
    public:
        using clock_t = std::chrono::high_resolution_clock;
        using time_point_t = clock_t::time_point;
        using duration_t = time_point_t::duration;
        using seconds_t = std::chrono::duration<double, std::ratio<1>>;
        void update()
        {
            update_raw_time();
            if (!suspended)
            {
                //if (wrap_time())
                 //   return;
                duration_t new_time = get_time(); 
                delta_time_ = new_time - std::exchange(current_time_, new_time);
            }
        }
        void toggle_suspend()
        {
            if (suspended)
                update_begin_time();
            suspended = !suspended;
        }
        void speed(double new_speed)
        {
            speed_ = new_speed;
            update_begin_time();
        }
        // to solve precision problem when speed is to large.
        template <typename Dur>
        void set_period(Dur p)
        {
            period = std::chrono::duration_cast<duration_t>(p);
        }
        duration_t delta() const
        {
            return delta_time_;
        }
        double delta_in_seconds() const
        {
            return delta_time_ / seconds_t(1);
        }
        duration_t current() const
        {
            return current_time_;
        }
        double current_in_seconds() const
        {
            return current_time_ / seconds_t(1);
        }
    private:
        duration_t raw_get_time() const
        {
            return clock_t::now().time_since_epoch() - begin_time_;
        }
        void update_raw_time()
        {
            raw_time_ = raw_get_time();
        }
        duration_t get_time()
        {
            return std::chrono::duration_cast<duration_t>(speed_ * raw_time_);
        }
        void update_begin_time()
        {
            begin_time_ = raw_time_ - std::chrono::duration_cast<duration_t>(current_time_ / speed_);
            wrap_time();
        }
        bool wrap_time()
        {
            if (period && current_time_.count() > 1000'00000000)
            {
                long long circle = current_time_ / *period;
                duration_t change_diff = std::chrono::duration_cast<duration_t>(circle * *period / speed_);
                begin_time_ += change_diff;
                update_raw_time();
                current_time_ = get_time();
                return true;
            }
            return false;
        }
        duration_t begin_time_ = clock_t::now().time_since_epoch();
        duration_t raw_time_ = raw_get_time();
        duration_t current_time_ = raw_time_;
        duration_t diff = duration_t::zero();
        duration_t delta_time_ = duration_t::zero();
        double speed_ = 1;

        bool suspended = false;

        std::optional<duration_t> period;
    };
}
