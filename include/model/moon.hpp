#pragma once

#include <stdexcept>
#include <glm/glm.hpp>
#include "model/mesh.hpp"
#include "render/texture.hpp"

namespace CGTask::model
{
    struct moon_exception : std::logic_error
    {
        using logic_error::logic_error;
    };
    struct moon_model_not_match : moon_exception
    {
        using moon_exception::moon_exception;
    };
    class moon_mesh
    {
    public:
        moon_mesh(mesh_handler mesh)
            : mesh(std::move(mesh))
        {
        }
        void draw(render::shader_view const &shader) const
        {
            shader.set("material.ambient", 1.0f);
            shader.set("material.shininess", 1.0f);
            mesh.draw(shader);
        }
        render::shader_type shader_type() const
        {
            return render::shader_type::texture_lighting_shader;
        }
    private:
        mesh_handler mesh;
    };
    inline moon_mesh make_moon()
    {
        mesh_tree tree("assets/model/moon/Moon 2K.obj");
        std::vector<mesh_handler> meshes = tree.detach();
        if (meshes.size() != 1)
            throw moon_model_not_match("moon mesh number not match");
        render::texture_handler texture_moon = render::make_texture("assets/model/moon/Textures/Diffuse_2K.png", "diffuse");
        meshes[0].add_texture(std::move(texture_moon));
        return moon_mesh(std::move(meshes[0]));
    }
}
