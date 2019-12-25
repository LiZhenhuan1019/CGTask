#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include "manager_base.hpp"

namespace CGTask::model
{
    struct buffer_exception : public std::logic_error 
    {
        using logic_error::logic_error;
    };
    struct invalid_buffer : buffer_exception
    {
        using buffer_exception::buffer_exception;
    };
    struct invalid_vao : buffer_exception
    {
        using buffer_exception::buffer_exception;
    };

    inline auto buffer_deleter = [](GLuint const &i)
    {
        glDeleteBuffers(1, &i);
    };
    class buffer_manager : public manager_base<GLuint>
    {
    public:
        explicit buffer_manager(GLuint buffer)
            :manager_base(!glIsBuffer(buffer) ?
                    throw invalid_buffer("buffer_manager::buffer_manager: not a buffer") :
                    buffer, buffer_deleter)
        {}
    };

    buffer_manager make_buffer(GLenum buffer_target, std::size_t size, void const *begin, GLenum usage)
    {
        GLuint buffer_name;
        glGenBuffers(1, &buffer_name);
        glBindBuffer(buffer_target, buffer_name);
        buffer_manager result(buffer_name);
        glBufferData(buffer_target, size, begin, usage);
        return result;
    }

    void set_vertex_attribute_pointer(GLuint index, GLint vertex_size, GLenum type,
            GLboolean normalized, GLsizei stride, void const *pointer)
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, vertex_size, type, normalized, stride, pointer);
    }

    inline auto vao_deleter = [](GLuint const&i)
    {
        glDeleteVertexArrays(1, &i);
    };
    class vao_manager : public manager_base<GLuint>
    {
    public:
        explicit vao_manager(GLuint vao)
            : manager_base(!glIsVertexArray(vao) ?
                    throw invalid_vao("vao_manager::vao_manager: not a vao") :
                    vao, vao_deleter)
        {}
        void bind_vertex_array() const
        {
            glBindVertexArray(id());
        }
    };
    vao_manager make_vao()
    {
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        vao_manager result(vao);
        return result;
    }
}
