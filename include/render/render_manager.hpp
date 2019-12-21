#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <optional>
#include "render/shader_manager.hpp"
#include "model/buffer_manager.hpp"
#include "model/model_manager.hpp"
#include "model/axis_2d.hpp"
#include "model/rectangle_2d.hpp"
#include "model/cube.hpp"
#include "model/ellipse_2d.hpp"
#include "camera/free_camera.hpp"

namespace CGTask::render
{
    class render_manager
    {
    public:
        render_manager(camera::free_camera const&camera, model::model_manager const&model)
            : camera(camera), model(model)
        {
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
        }
        void render()
        {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            {
                glUseProgram(uniform_color_program.id());
                load_view_projection_matrix(uniform_color_program.id());
                int model_location = glGetUniformLocation(uniform_color_program.id(), "model");
                int color_location = glGetUniformLocation(uniform_color_program.id(), "fragcolor");

                glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model.get_ellipse().transform()));
                glUniform4fv(color_location, 1, glm::value_ptr(model.get_ellipse().color()));
                model.get_ellipse().draw();
            }
            {
                glUseProgram(vertex_color_program.id());
                load_view_projection_matrix(vertex_color_program.id());
                int model_location = glGetUniformLocation(vertex_color_program.id(), "model");
                glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model.get_axis().transform()));
                model.get_axis().draw();
                
                glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model.get_cube().transform()));
                model.get_cube().draw();
            }
        }
        
    private:
        void load_view_projection_matrix(GLuint program)
        {
            int view_location = glGetUniformLocation(program, "view");
            glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(camera.view()));
            int projection_location = glGetUniformLocation(program, "projection");
            glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(camera.projection()));
        }


        shader_program_handler uniform_color_program = make_program(
                make_shader(GL_VERTEX_SHADER, load_file("assets/shaders/2d_nocolor.vert")),
                make_shader(GL_FRAGMENT_SHADER, load_file("assets/shaders/2d_uniform_color.frag")));
        shader_program_handler vertex_color_program = make_program(
                make_shader(GL_VERTEX_SHADER, load_file("assets/shaders/2d_color.vert")),
                make_shader(GL_FRAGMENT_SHADER, load_file("assets/shaders/2d_vertex_color.frag")));
        camera::free_camera const&camera;
        model::model_manager const&model;
    };
}
