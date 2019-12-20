#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <optional>
#include "render/shader_manager.hpp"
#include "render/buffer_manager.hpp"
#include "render/axis_2d.hpp"
#include "render/rectangle_2d.hpp"

namespace CGTask::render
{
    class render_manager
    {
    public:
        render_manager(std::size_t width, std::size_t height)
            : axis(width, height)
        {
            set_projection(width, height);
            
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        void render()
        {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            glUseProgram(program.id());

            int view_location = glGetUniformLocation(program.id(), "view");
            glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
            int projection_location = glGetUniformLocation(program.id(), "projection");
            glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));
            int model_location = glGetUniformLocation(program.id(), "model");
            glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(axis.model()));
            axis.draw();
            glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(rectangle.model()));
            rectangle.draw();
        }
        void set_size(std::size_t width, std::size_t height)
        {
            set_projection(width, height);
            axis.set_size(width, height);
        }
    private:
        void set_projection(std::size_t width, std::size_t height)
        {
            projection = glm::ortho(-(float)width / 2, (float)width / 2,
                    -(float)height / 2, (float)height / 2, 0.0f, 100.f);
        }

        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection;
        shader_program_handler program = make_program(
                make_shader(GL_VERTEX_SHADER, load_file("assets/shaders/2d_demo.vert")),
                make_shader(GL_FRAGMENT_SHADER, load_file("assets/shaders/2d_demo.frag")));
        axis_2d<> axis;
        rectangle_2d<> rectangle;
    };
}
