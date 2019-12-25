#pragma once

#include <stdexcept>
#include "model/mesh.hpp"
#include "render/texture.hpp"

namespace CGTask::model
{
    struct earth_exception : std::logic_error
    {
        using logic_error::logic_error;
    };
    struct earth_model_not_match : earth_exception
    {
        using earth_exception::earth_exception;
    };
    class earth_mesh
    {
    public:
        earth_mesh(mesh_handler mesh)
            : mesh(std::move(mesh))
        {}
        void draw(render::shader_view const &shader) const
        {
            shader.set("material.ambient", 1.0f);
            shader.set("material.shininess", 32.0f);
            mesh.draw(shader);
        }
        render::shader_type shader_type() const
        {
            return render::shader_type::texture_lighting_shader;
        }
    private:
        mesh_handler mesh;
    };
    struct earth_model
    {
        earth_mesh earth;
        earth_mesh clouds;
    };
    inline earth_model make_earth()
    {
        mesh_tree tree("assets/model/earth/Earth 2K.obj");
        std::vector<mesh_handler> meshes = tree.detach();
        std::size_t earth_index = -1, clouds_index = -1;
        for (std::size_t i = 0; i < meshes.size(); ++i)
        {
            if (meshes[i].name() == "Earth_Cube.002")
                earth_index = i;
            else if (meshes[i].name() == "Clouds_Cube.000")
                clouds_index = i;
        }
        if (earth_index >= meshes.size() ||
                clouds_index >= meshes.size())
            throw earth_model_not_match("earth mesh name not match");
        render::texture_handler texture_earth_diffuse = render::make_texture("assets/model/earth/Textures/Diffuse_2K.png", "diffuse");
        render::texture_handler texture_clouds_diffuse = render::make_texture("assets/model/earth/Textures/Clouds_2K.png", "diffuse", true);
        meshes[earth_index].add_texture(std::move(texture_earth_diffuse));
        meshes[clouds_index].add_texture(std::move(texture_clouds_diffuse));
        earth_mesh earth(std::move(meshes[earth_index]));
        earth_mesh clouds(std::move(meshes[clouds_index]));
        return {std::move(earth), std::move(clouds)};
    }
}
