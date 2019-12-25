#pragma once

#include "model/buffer_manager.hpp"
#include "render/texture.hpp"
#include "render/shader_manager.hpp"

namespace CGTask::model
{
    class mesh_with_texture
    {
    public:
        mesh_with_texture(vao_manager vao, buffer_manager vbo, buffer_manager ebo, std::size_t number_of_indices,
                render::texture_handler texture)
            : vao(std::move(vao)), vbo(std::move(vbo)), ebo(std::move(ebo)),
              number_of_indices(number_of_indices),
              texture(std::move(texture))
        {}
        void draw(render::shader_view const &shader) const
        {
            vao.bind_vertex_array();
            glDrawElements(GL_TRIANGLES, number_of_indices, GL_UNSIGNED_INT, nullptr);
        }
        render::shader_type shader_type() const
        {
            return render::shader_type::texture_lighting_shader;
        }
    private:
        vao_manager vao;
        buffer_manager vbo;
        buffer_manager ebo;
        std::size_t number_of_indices;
        render::texture_handler texture;
    };
}
