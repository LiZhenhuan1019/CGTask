#pragma once
#include <glm/glm.hpp>
#include "render/shader_manager.hpp"
namespace CGTask::model
{
    struct light
    {
        glm::vec3 position;
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        float constant;
        float linear;
        float quadratic;
        void set_shader(render::shader_view const &shader) const
        {
            shader.set("light.position", position);
            shader.set("light.ambient", ambient);
            shader.set("light.diffuse", diffuse);
            shader.set("light.specular", specular);
            shader.set("light.constant", constant);
            shader.set("light.linear", linear);
            shader.set("light.quadratic", quadratic);
        }
    };
}
