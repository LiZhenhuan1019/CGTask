#version 330 core
in vec3 frag_position;
in vec3 normal;
in vec2 texture_coord;
out vec4 FragColor;

struct Light
{
    vec3 position;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    float constant;
    float linear;
    float quadratic;
};
uniform Light light;
uniform vec3 view_position;

struct Material
{
    float ambient;
    float night_ambient;
    sampler2D diffuse;
    sampler2D night_diffuse;
    float shininess;
};
uniform Material material;

vec4 calculate_point_light(Light light, vec3 normal, vec3 frag_position, vec3 view_position)
{
    vec3 light_direction = normalize(light.position - frag_position);
    vec3 norm = normalize(normal);
    float dot_norm_light = dot(norm, light_direction);
    float night_mix_value = smoothstep(0.0, 0.4, dot_norm_light);
    float texture_mix_value = smoothstep(-0.3, 0.0, dot_norm_light);

    vec4 night_diffuse_color = texture(material.night_diffuse, texture_coord);
    vec4 diffuse_color = texture(material.diffuse, texture_coord);
    vec4 texture_color = mix(night_diffuse_color, diffuse_color, texture_mix_value);

    float distance = length(light.position - frag_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
            light.quadratic * (distance * distance));

    vec4 ambient = light.ambient *
        mix(material.night_ambient * night_diffuse_color,
            material.ambient * diffuse_color, night_mix_value);

    float diff = max(dot_norm_light, 0.0);
    vec4 diffuse = light.diffuse * diff * texture_color;

    vec3 view_direction = normalize(view_position - frag_position);
    vec3 halfway_direction = normalize(light_direction + view_direction);
    float spec = pow(max(dot(normal, halfway_direction), 0.0), material.shininess);
    vec4 specular = light.specular * spec * texture_color * smoothstep(-0.1, 0.1, dot_norm_light);

    return (ambient + diffuse + specular) * attenuation;
}

void main()
{
    vec4 color = calculate_point_light(light, normal, frag_position, view_position);
    FragColor = color;
}
