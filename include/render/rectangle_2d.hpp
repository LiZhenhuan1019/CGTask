#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "render/buffer_manager.hpp"

namespace CGTask::render
{
    template <typename Float = float>
    class rectangle_2d
    {
        static constexpr float vertices[] =
        {
            50.0f,  50.0f,  0.0f,  // top right
            50.0f,  -50.0f, 0.0f,  // bottom right
            -50.0f, -50.0f, 0.0f,  // bottom left
            -50.0f, 50.0f,  0.0f   // top left
        };
        static constexpr unsigned int indices[] =
        {
            0, 1, 3, 2
        };
    public:
        rectangle_2d()
            : vao(make_vao())
        {
            make_buffer(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW).detach();
            make_buffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW).detach();
            set_vertex_attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        }
        glm::mat4 model()
        {
            return glm::mat4(1.0f);
        }
        void draw()
        {
            vao.bind_vertex_array();
            glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
        }
    private:
        vao_manager vao;
    };
}
