#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include "model/axis_2d.hpp"
#include "model/rectangle_2d.hpp"
#include "model/cube.hpp"
#include "model/ellipse_2d.hpp"
#include "model/drawable.hpp"
#include "model/object.hpp"
#include "input/timer.hpp"
#include "render/shader_manager.hpp"

namespace CGTask::model
{
    class model_manager
    {
    public:
        virtual ~model_manager() = default;

        virtual void update() = 0;
        virtual void draw(render::shader_selector &selector) const = 0;
    };
}
