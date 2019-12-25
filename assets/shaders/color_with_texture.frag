#version 330 core
in vec3 fragcolor;
in vec2 texture_coord;

out vec4 FragColor;

uniform sampler2D the_texture;
void main()
{
    FragColor = texture(the_texture, texture_coord) * vec4(fragcolor, 1.0);
}
