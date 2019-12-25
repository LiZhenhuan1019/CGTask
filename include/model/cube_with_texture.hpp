#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "render/shader_manager.hpp"
#include "model/buffer_manager.hpp"
#include "render/texture.hpp"

namespace CGTask::model
{
    class cube_with_texture
    {
    public:
        cube_with_texture(float length, float width, float height)
            : vao(make(length, width, height))
        {}
        void draw(render::shader_view const &shader) const
        {
            render::texture_view texture_view = texture.view();
            shader.set("material.diffuse", 1);
            shader.set("material.shininess", 32.0f);
            glActiveTexture(GL_TEXTURE1);
            texture_view.use();
            vao.bind_vertex_array();
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        }
        render::shader_type shader_type() const
        {
            return render::shader_type::texture_lighting_shader;
        }
    private:
        vao_manager vao;
        render::texture_handler texture = render::make_texture("assets/texture/container.jpg", "diffuse");

        vao_manager make(float length, float width, float height)
        {
            float points[] = 
            {
                // position               // normal          // texture coords
                -length, width, -height,  0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
                length, width, -height,   0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
                length, -width, -height,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
                -length, -width, -height, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,

                length, width, height,    1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
                length, -width, height,   1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
                length, -width, -height,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
                length, width, -height,   1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

                length, width, height,    0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
                -length, width, height,   0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
                -length, -width, height,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
                length, -width, height,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

                -length, -width, height,  -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                -length, width, height,   -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                -length, width, -height,  -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                -length, -width, -height, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

                length, -width, height,   0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
                -length, -width, height,  0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
                -length, -width, -height, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                length, -width, -height,  0.0f, -1.0f, 0.0f, 1.0f, 0.0f,

                -length, width, height,   0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
                length, width, height,    0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
                length, width, -height,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
                -length, width, -height,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
            };
            unsigned int indices[] = 
            {
                0, 1, 2, 0, 2, 3,
                4, 5, 6, 4, 6, 7,
                8, 9, 10, 8, 10, 11,
                12, 13, 14, 12, 14, 15,
                16, 17, 18, 16, 18, 19,
                20, 21, 22, 20, 22, 23
            };
            vao_manager vao = make_vao();
            make_buffer(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW).detach();
            make_buffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW).detach();
            set_vertex_attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),nullptr);
            set_vertex_attribute_pointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            set_vertex_attribute_pointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            return vao;
        }
    };
}
