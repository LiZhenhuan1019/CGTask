#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include "model/buffer_manager.hpp"
#include "model/transform.hpp"

namespace CGTask::model
{
    template <typename Float = float>
    class rectangle_2d : public transformable
    {
    public:
        rectangle_2d(float height)
            : vao(make_vao())
        {
            float vertices[] =
            {
                50.0f,  50.0f,  height,  // top right
                -50.0f, 50.0f,  height,   // top left
                -50.0f, -50.0f, height,  // bottom left
                50.0f,  -50.0f, height,  // bottom right
            };

            static constexpr unsigned int indices[] =
            {
                0, 1, 2, 0, 2, 3,
                0, 2, 1, 0, 3, 2

            };
            make_buffer(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW).detach();
            make_buffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW).detach();
            set_vertex_attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        }
        glm::vec4 color()
        {
            return glm::vec4(1.0f, 0.5f, 0.2f, 1.0f);
        }
        void draw()
        {
            vao.bind_vertex_array();
            glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
        }
    private:
        vao_manager vao;
    };
}
