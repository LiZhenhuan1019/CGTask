#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include "model/axis_2d.hpp"
#include "model/rectangle_2d.hpp"
#include "model/cube.hpp"
#include "model/ellipse_2d.hpp"
#include "input/timer.hpp"

namespace CGTask::model
{
    class model_manager
    {
    public:
        model_manager()
            : axis_(1000), cube_(10, 10, 10), ellipse(200)
        {}

        void update()
        {
            timer.update();
            double t = timer.current();
            cube_.position(glm::vec3(200 * std::cos(t), 0, 150 * std::sin(t)));
        }
        
        axis_2d<> const &get_axis() const
        {
            return axis_;
        }
        cube const &get_cube() const
        {
            return cube_;
        }
        ellipse_2d const &get_ellipse() const
        {
            return ellipse;
        }
    private:
        input::delta_timer timer;
        axis_2d<> axis_;
        cube cube_;
        ellipse_2d ellipse;
    };
}
