#version 330 core
in vec3 fragcolor;
out vec4 FragColor;
void main()
{
    FragColor = vec4(fragcolor, 1.0);
}
