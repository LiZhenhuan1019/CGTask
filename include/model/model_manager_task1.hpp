#pragma once
#include <chrono>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include "model/model_manager.hpp"
#include "model/axis_2d.hpp"
#include "model/cube.hpp"
#include "model/ellipse_2d.hpp"
#include "model/drawable.hpp"
#include "model/object.hpp"
#include "input/timer.hpp"
#include "render/shader_manager.hpp"

namespace CGTask::model
{
    class model_manager_task1 : public model_manager
    {
    public:
        model_manager_task1()
            : axis_mesh(add(meshes, axis_2d<>(1000))), 
              cube_mesh(add(meshes, cube(10, 10, 10))),
              ellipse_mesh(add(meshes, ellipse_2d(120))),
              axis_object(add(objects, object(axis_mesh))),
              cube_object(add(objects, object(cube_mesh))),
              ellipse_object(add(objects, object(ellipse_mesh)))
        {}

        void update() override
        {
            timer.update();
            double t = timer.current_in_seconds();
            get_transform(cube_object).position(glm::vec3(120 * std::cos(t), 0, 90 * std::sin(t)));
        }
        void draw(render::shader_selector &selector) const override
        {
            for (auto const &each : objects)
            {
                drawable const& mesh = get_drawable(each.mesh());
                render::shader_view shader = selector.select(mesh.shader_type());
                shader.set("model", each.transform.transform().transform_matrix());
                mesh.draw(shader);
            }
        }
        
    private:
        transform &get_transform(std::size_t object_id)
        {
            return objects[object_id].transform.transform();
        }
        drawable const &get_drawable(std::size_t mesh_id) const
        {
            return meshes[mesh_id];
        }
        template <typename Vec, typename ...Args>
        std::size_t add(Vec &vec, Args &&...args)
        {
            std::size_t result = vec.size();
            vec.emplace_back(std::forward<Args>(args)...);
            return result;
        }

        input::delta_timer timer;
        std::vector<drawable> meshes;
        std::size_t axis_mesh, cube_mesh, ellipse_mesh;
        std::vector<object> objects;
        std::size_t axis_object, cube_object, ellipse_object;
    };
}
