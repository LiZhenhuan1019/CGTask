#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <optional>
#include <stdexcept>
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
    struct font_error : std::runtime_error
    {
        using runtime_error::runtime_error;
    };
    class render_manager
    {
    public:
        render_manager(camera::free_camera const&camera, model::model_manager const&model)
            : camera(camera), model(model)
        {
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        void render()
        {
            glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            selector.foreach([this](shader_view const &shader)
                    {
                        load_view_projection_matrix(shader.id());
                    });
            model.draw(selector);
        }
        
    private:
        void load_view_projection_matrix(GLuint program)
        {
            int view_location = glGetUniformLocation(program, "view");
            glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(camera.view()));
            int projection_location = glGetUniformLocation(program, "projection");
            glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(camera.projection()));
        }

        shader_handlers handlers;
        shader_selector selector = handlers.handlers;
        camera::free_camera const&camera;
        model::model_manager const&model;
    };
}
