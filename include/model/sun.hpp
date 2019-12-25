#pragma once

#include <stdexcept>
#include <glm/glm.hpp>
#include "model/mesh.hpp"
#include "render/texture.hpp"

namespace CGTask::model
{
    struct sun_exception : std::logic_error
    {
        using logic_error::logic_error;
    };
    struct sun_model_not_match : sun_exception
    {
        using sun_exception::sun_exception;
    };
    class sun_mesh
    {
    public:
        sun_mesh(mesh_handler mesh)
            : mesh(std::move(mesh))
        {
        }
        void draw(render::shader_view const &shader) const
        {
            shader.set("light.ambient", glm::vec4(1, 1, 1, 1));
            shader.set("light.diffuse", glm::vec4(0, 0, 0, 0));
            shader.set("light.specular", glm::vec4(0, 0, 0, 0));
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
    inline sun_mesh make_sun()
    {
        mesh_tree tree("assets/model/sun/13913_Sun_v2_l3.obj");
        std::vector<mesh_handler> meshes = tree.detach();
        if (meshes.size() != 1)
            throw sun_model_not_match("sun mesh number not match");
        render::texture_handler texture_sun = render::make_texture("assets/model/sun/13913_Sun_diff.jpg", "diffuse");
        meshes[0].add_texture(std::move(texture_sun));
        return sun_mesh(std::move(meshes[0]));
    }
}
