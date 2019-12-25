#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/geometric.hpp>
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
#include "model/moon.hpp"
#include "model/sun.hpp"
#include "input/timer.hpp"
#include "render/shader_manager.hpp"
#include "math/elliptic_orbit.hpp"

namespace CGTask::model
{
    class model_manager_task2 : public model_manager
    {
    public:
        model_manager_task2(camera::free_camera const&camera)
            : camera(camera),
              axis_mesh(add(meshes, axis_2d<>(1000))), 
              ellipse_mesh(add(meshes, ellipse_2d(earth_orbit_major_axis))),
              axis_object(add(objects, object(axis_mesh))),
              ellipse_object(add(objects, object(ellipse_mesh)))
        {
            get_transform(ellipse_object).rotate(glm::radians(90.0f), glm::vec3(1, 0, 0));

            sun_mesh = add(meshes, make_sun());
            sun_object = add(objects ,object(sun_mesh));
            get_transform(sun_object).translate(glm::vec3(focus_x, 0, 0));
            get_transform(sun_object).scale(glm::vec3(0.05, 0.05, 0.05));

            moon_mesh = add(meshes, make_moon());
            moon_object = add(objects, object(moon_mesh));
            get_transform(moon_object).scale(glm::vec3(1, 1, 1));

            earth_model earth = make_earth();
            earth_mesh = add(meshes, std::move(earth.earth));
            earth_object = add(objects, object(earth_mesh));
            get_transform(earth_object).scale(glm::vec3(1, 1, 1));
            get_transform(earth_object).rotate(glm::pi<float>() / 2, glm::vec3(1.0f, 0.0f, 0.0f));

            earth_clouds_mesh = add(meshes, std::move(earth.clouds));
            earth_clouds_object = add(objects, object(earth_clouds_mesh));
            get_transform(earth_clouds_object).scale(glm::vec3(1.02, 1.02, 1.02));
            get_transform(earth_clouds_object).rotate(glm::pi<float>() / 2, glm::vec3(1.0f, 0.0f, 0.0f));
        }

        void update() override
        {
            timer.update();
            double t = timer.current();
            update_earth_pos(t);
            update_earth_rotation(t);
            update_moon_transform(t);
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
        void set_revolution_period(double period)
        {
            earth_revolution_period = period;
            rotation_period = earth_revolution_period / 365.25;
            moon_period = earth_revolution_period * 27.32 / 365.25;
        }
        void toggle_suspend()
        {
            timer.toggle_suspend();
        }
    private:
        float moon_orbit_radius = 10;
        float earth_orbit_major_axis = 200;
        //float earth_orbit_eccentricity = 0.0167086;
        float earth_orbit_eccentricity = std::sqrt(7) / 4;

        float focus_x = earth_orbit_major_axis * earth_orbit_eccentricity;

        double earth_revolution_period = 60;
        double rotation_period = earth_revolution_period / 365.25;
        //double rotation_period = earth_revolution_period / 50;
        double moon_period = earth_revolution_period * 27.32 / 365.25;

        math::elliptic_orbit orbit{earth_orbit_eccentricity, earth_orbit_major_axis};

        camera::free_camera const &camera;
        light main_light =
        {
            {focus_x, 0.0f, 0.0f},
            {0.1f, 0.1f, 0.1f, 1.0f},
            {1.0f, 1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f, 1.0f},
            1.0f, 0.0f, 0.0f
        };
        input::delta_timer timer;
        std::vector<drawable> meshes;
        std::size_t axis_mesh, ellipse_mesh, sun_mesh, moon_mesh, earth_mesh, earth_clouds_mesh;
        std::vector<object> objects;
        std::size_t axis_object, ellipse_object, sun_object, moon_object, earth_object, earth_clouds_object;

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
        void update_earth_pos(double t)
        {
            int circle = t / earth_revolution_period;
            t -= circle * earth_revolution_period;
            math::point p = orbit.position_on_elliptic_orbit(t, earth_revolution_period);
            glm::vec3 pos(p.x, p.y, 0);
            get_transform(earth_object).position(pos);
            get_transform(earth_clouds_object).position(pos);
        }
        void update_earth_rotation(double time)
        {
            glm::mat4 rot = glm::rotate(glm::pi<float>() / 2, glm::vec3(1.0f, 0.0f, 0.0f));
            rot = glm::rotate(float(time / rotation_period * 2 * glm::pi<float>()), glm::vec3(0, 0, 1)) * rot;
            get_transform(earth_object).rotation(rot);
            get_transform(earth_clouds_object).rotation(rot);
        } 
        void update_moon_transform(double time)
        {
            glm::mat4 rot = glm::rotate(glm::radians(125.0f), glm::vec3(0, 0, 1));
            rot = glm::rotate(glm::radians(-23.0f), glm::vec3(1, 0, 0)) * rot;
            rot = glm::rotate(glm::radians(150.0f), glm::vec3(0, 1, 0)) * rot;
            rot = glm::rotate(glm::radians(float(time / moon_period * 360)), glm::vec3(0, 0, 1)) * rot;
            glm::mat4 revolution = glm::rotate(
                    float(time / moon_period * 2 *glm::pi<float>()), glm::normalize(glm::vec3(1, 0, 10)));
            glm::vec3 pos(0, moon_orbit_radius, 0);
            pos = revolution * glm::vec4(pos, 1);
            pos += get_transform(earth_object).position();
            auto &moon_transform = get_transform(moon_object);
            moon_transform.position(pos);
            moon_transform.rotation(rot);
        }
    };
}
