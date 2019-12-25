#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "render/shader_manager.hpp"
#include "model/buffer_manager.hpp"

namespace CGTask::model
{
    class cube
    {
    public:
        cube(float length, float width, float height)
            : vao(make(length, width, height))
        {}
        void draw(render::shader_view const &view) const
        {
            vao.bind_vertex_array();
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        }
        render::shader_type shader_type() const
        {
            return render::shader_type::vertex_color_shader;
        }
    private:
        vao_manager vao;

        vao_manager make(float length, float width, float height)
        {
            float points[] = 
            {
                // position              // color
                length, width, - height, 1.0f, 1.0f, 0.0f,
                - length, width, - height, 0.0f, 1.0f, 0.0f,
                - length, - width, - height, 0.0f, 0.0f, 0.0f,
                length, - width, - height, 1.0f, 0.0f, 0.0f,
                length, width, height, 1.0f, 1.0f, 1.0f,
                - length, width, height, 0.0f, 1.0f, 1.0f,
                - length, - width, height, 0.0f, 0.0f, 1.0f,
                length, - width, height, 1.0f, 0.0f, 1.0f
            };
            unsigned int indices[] = 
            {
                0, 3, 2, 0, 2, 1,
                4, 5, 6, 4, 6, 7,
                0, 4, 7, 0, 7, 3,
                1, 2, 6, 1, 6, 5,
                2, 3, 7, 2, 7, 6,
                0, 1, 5, 0, 5, 4
            };
            vao_manager vao = make_vao();
            make_buffer(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW).detach();
            make_buffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW).detach();
            set_vertex_attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
            set_vertex_attribute_pointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
            return vao;
        }
    };
}
