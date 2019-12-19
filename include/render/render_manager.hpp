#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "render/shader_manager.hpp"
namespace CGTask::render
{
    class render_manager
    {
    public:
        render_manager()
        {
            float vertices[] =
            {
                0.5f,  0.5f,  0.0f,  // top right
                0.5f,  -0.5f, 0.0f,  // bottom right
                -0.5f, -0.5f, 0.0f,  // bottom left
                -0.5f, 0.5f,  0.0f   // top left
            };
            unsigned int indices[] =
            {
                0, 1, 3,
                2, 3, 1
            };
            GLuint VBO;
            GLuint EBO;
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),(void*)0);
            glEnableVertexAttribArray(0);
            glBindVertexArray(0);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        void render()
        {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            glUseProgram(program.id());
            glBindVertexArray(VAO);
            glDrawElements(GL_POINTS, 4, GL_UNSIGNED_INT, 0);
        }
    private:
        GLuint VAO;
        shader_program_handler program = make_program(
                make_shader(GL_VERTEX_SHADER, load_file("assets/shaders/2d_demo.vert")),
                make_shader(GL_FRAGMENT_SHADER, load_file("assets/shaders/2d_demo.frag")));
    };
}
