#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <cmath>
#include "render/shader_manager.hpp"
#include "model/buffer_manager.hpp"

namespace CGTask::model
{
    class ellipse_2d
    {
    public:
        ellipse_2d(float x_axis)
            : vao(make_vao())
        {
            float z_axis = x_axis / 4 * 3;
            float x_end = x_axis / 5 * 4;
            float z_end = z_axis / 5 * 3;
            float square_x_axis = x_axis * x_axis;
            float square_z_axis = z_axis * z_axis;
            std::vector<float> vertices;
            vertices.insert(vertices.end(),
                    {
                        0, 0, (float)z_axis, 
                        0, 0, -(float)z_axis
                    });
            int x = 0, z = z_axis;
            double d = 1 / ((double)square_x_axis) +
                0.25 / ((double)square_z_axis) - 1 / (double)z_axis;
            while (x < x_end)
            {
                double d_increment_x = (2 * x + 3) / ((double)square_x_axis);
                if (d < 0)
                {
                    d += d_increment_x;
                    x += 1;
                }
                else
                {
                    d += d_increment_x + (-2 * z + 2) / ((double)square_z_axis);
                    x += 1;
                    z -= 1;
                }
                insert_symmetry(vertices, x, z);
            }
            insert_symmetry(vertices, x_end, z_end);
            x = x_axis;
            z = 0;
            d = 1 / ((double)square_z_axis) +
                0.25 / ((double)square_x_axis) - 1 / (double)x_axis;
            while (z < z_end)
            {
                double d_increment_z = (2 * z + 3) / ((double)square_z_axis);
                if (d < 0)
                {
                    d += d_increment_z;
                    z += 1;
                }
                else
                {
                    d += d_increment_z + (-2 * x + 2) / ((double)square_x_axis);
                    z += 1;
                    x -= 1;
                }
                insert_symmetry(vertices, x, z);
            }
            size = vertices.size();
            make_buffer(GL_ARRAY_BUFFER, size * sizeof(float), &vertices[0], GL_STATIC_DRAW).detach();
            set_vertex_attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        }
        glm::vec4 color() const
        {
            return glm::vec4(0.2f, 0.5f, 1.0f, 1.0f);
        }
        void draw(render::shader_view const &shader) const
        {
            shader.set("fragcolor", color());
            vao.bind_vertex_array();
            glDrawArrays(GL_POINTS, 0, size / 3);
        }
    private:
        void insert_symmetry(std::vector<float> &vec, float x, float z)
        {
            vec.insert(vec.end(),
                {
                    (float)x, 0, (float)z,
                    (float)x, 0, -(float)z,
                    -(float)x, 0, (float)z,
                    -(float)x, 0, -(float)z
                });
        }
        vao_manager vao;
        std::size_t size;
    };
}
