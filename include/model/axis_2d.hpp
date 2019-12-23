#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include <vector>
#include "model/buffer_manager.hpp"
#include "render/shader_manager.hpp"

namespace CGTask::model
{
    template <typename Float = float>
    class axis_2d
    {
        struct buffers
        {
            vao_manager vao;
            buffer_manager vbo;
            buffer_manager ebo;
        };
    public:
        axis_2d(std::size_t size)
            : size(size), obj(make())
        {}
        void draw(render::shader_view const &shader) const
        {
            obj.vao.bind_vertex_array();
            glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, nullptr);
        }
        
    private:
        std::size_t size;
        buffers obj;
        buffers make()
        {
            unsigned int indices[] = 
            {
                0, 1,
                2, 3,
                4, 5,
            };
          
            vao_manager vao = make_vao();
            buffer_manager vbo = make_buffer(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);
            fill_vertex_buffer(vbo.id());
            buffer_manager ebo = make_buffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
            set_vertex_attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(Float), nullptr);
            set_vertex_attribute_pointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(Float), (void *)(3 * sizeof(float)));
            return buffers{std::move(vao), std::move(vbo), std::move(ebo)};
        }
        void fill_vertex_buffer(GLuint vbo)
        {
            Float points[] =
            {
                // position             color
                (Float)size, 0, 0,  1.0f, 0.0f, 0.0f,
                -(Float)size, 0, 0, 1.0f, 0.0f, 0.0f,
                0, (Float)size, 0,  0.0f, 1.0f, 0.0f,
                0, -(Float)size, 0, 0.0f, 1.0f, 0.0f,
                0, 0, (Float)size,  0.0f,  0.0f, 1.0f,
                0, 0, -(Float)size, 0.0f,  0.0f, 1.0f
            };
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER , sizeof(points), points, GL_STATIC_DRAW);
        }
    };
}
