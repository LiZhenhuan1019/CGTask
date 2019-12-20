#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include <vector>
#include "render/buffer_manager.hpp"


namespace CGTask::render
{
    template <typename Float = float>
    class axis_2d
    {
        struct objects
        {
            vao_manager vao;
            buffer_manager vbo;
            buffer_manager ebo;
        };
    public:
        axis_2d(std::size_t width, std::size_t height)
            : width(width), height(height), obj(make())
        {}
        void draw()
        {
            obj.vao.bind_vertex_array();
            glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, nullptr);
        }
        glm::mat4 model() const
        {
            return glm::mat4(1.0f);
        }
        void set_size(std::size_t width, std::size_t height)
        {
            this->width = width;
            this->height = height;
            fill_vertex_buffer(obj.vbo.id());
        }
    private:
        std::size_t width, height;
        objects obj;
        objects make()
        {
            unsigned int indices[] = 
            {
                0, 1,
                0, 2,
                0, 3,
                0, 4
            };
          
            vao_manager vao = make_vao();
            buffer_manager vbo = make_buffer(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);
            fill_vertex_buffer(vbo.id());
            buffer_manager ebo = make_buffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
            set_vertex_attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(Float), nullptr);
            return objects{std::move(vao), std::move(vbo), std::move(ebo)};
        }
        void fill_vertex_buffer(GLuint vbo)
        {
            Float points[] =
            {
                0, 0, 0,
                (Float)width/2, 0, 0,
                0, (Float)height/2, 0,
                -(Float)width/2, 0, 0,
                0, -(Float)height/2, 0
            };
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER , sizeof(points), points, GL_STATIC_DRAW);
        }
    };
}
