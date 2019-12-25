#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/gtc/type_ptr.hpp>

#include "camera/free_camera.hpp"
#include "model/model_manager.hpp"
#include "model/axis_2d.hpp"
#include "model/cube_with_texture.hpp"
#include "model/ellipse_2d.hpp"
#include "model/drawable.hpp"
#include "model/object.hpp"
#include "model/light.hpp"
#include "model/earth_model.hpp"
#include "input/timer.hpp"
#include "render/shader_manager.hpp"

namespace CGTask::model
{
    class model_manager_task2 : public model_manager
    {
    public:
        model_manager_task2(camera::free_camera const&camera)
            : camera(camera),
              axis_mesh(add(meshes, axis_2d<>(1000))), 
              cube_mesh(add(meshes, cube_with_texture(10, 10, 10))),
              ellipse_mesh(add(meshes, ellipse_2d(120))),
              axis_object(add(objects, object(axis_mesh))),
              cube_object(add(objects, object(cube_mesh))),
              ellipse_object(add(objects, object(ellipse_mesh)))
        {
            earth_model earth = make_earth();
            earth_mesh = add(meshes, std::move(earth.earth));
            earth_object = add(objects, object(earth_mesh));
            get_transform(earth_object).scale(glm::vec3(5, 5, 5));
            get_transform(earth_object).rotate(glm::pi<float>() / 2, glm::vec3(1.0f, 0.0f, 0.0f));

            earth_clouds_mesh = add(meshes, std::move(earth.clouds));
            earth_clouds_object = add(objects, object(earth_clouds_mesh));
            get_transform(earth_clouds_object).scale(glm::vec3(5.1, 5.1, 5.1));
            get_transform(earth_clouds_object).rotate(glm::pi<float>() / 2, glm::vec3(1.0f, 0.0f, 0.0f));
        }

        void update() override
        {
            timer.update();
            double t = timer.current();
            get_transform(cube_object).position(glm::vec3(120 * std::cos(t), 0, 90 * std::sin(t)));
        }
        void draw(render::shader_selector &selector) const override
        {
            for (auto const &each : objects)
            {
                drawable const& mesh = get_drawable(each.mesh());
                render::shader_view shader = selector.select(mesh.shader_type());
                if (mesh.shader_type() == render::shader_type::texture_lighting_shader)
                {
                    main_light.set_shader(shader);
                    shader.set("view_position", glm::vec3(camera.pos()));
                }
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

        camera::free_camera const &camera;
        light main_light =
        {
            {120.f, 50.0f, 0.0f},
            {0.1f, 0.1f, 0.1f, 1.0f},
            {1.0f, 1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f, 1.0f},
            1.0f, 0.0f, 0.0f
        };
        input::delta_timer timer;
        std::vector<drawable> meshes;
        std::size_t axis_mesh, cube_mesh, ellipse_mesh, earth_mesh, earth_clouds_mesh;
        std::vector<object> objects;
        std::size_t axis_object, cube_object, ellipse_object, earth_object, earth_clouds_object;
    };
}
