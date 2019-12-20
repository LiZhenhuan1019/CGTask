#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <initializer_list>
#include <string_view>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <fstream>
#include "manager_base.hpp"

namespace CGTask::render
{
    struct render_exception : public std::logic_error
    {
        using logic_error::logic_error;
    };
    struct invalid_shader : public render_exception
    {
        using render_exception::render_exception;
    };
    struct compile_shader_failed : public render_exception
    {
        using render_exception::render_exception;
    };
    struct invalid_program : public render_exception
    {
        using render_exception::render_exception;
    };
    struct link_program_failed : public render_exception
    {
        using render_exception::render_exception;
    };
    struct read_shader_file_failed : public render_exception
    {
        using render_exception::render_exception;
    };

    namespace detail
    {
        [[noreturn]] void throw_read_shader_file_failed(std::string_view path)
        {
            using namespace std::literals;
            throw read_shader_file_failed("file path: '"s + path.data() + "'");
        }
    }
    inline std::string load_file(std::string_view path)
    {
        std::string result;
        std::ifstream file(path.data());
        if (!file)
            detail::throw_read_shader_file_failed(path);
        file.seekg(0, std::ios_base::end);
        std::size_t count = file.tellg();
        file.seekg(0, std::ios_base::beg);
        result.resize(count);
        file.read(&result[0], count);
        if (!file && !file.eof())
            detail::throw_read_shader_file_failed(path);
        return result;
    }

    auto shader_deleter = [](GLuint shader_id)
    {
        glDeleteShader(shader_id);
    };
    class shader_handler : public manager_base<GLuint>
    {
    public:
        explicit shader_handler(GLuint shader_id)
            : manager_base(!glIsShader(shader_id) ? 
                    throw invalid_shader("shader_handler::shader_handler: not a shader") :
                    shader_id, shader_deleter)
        {}
    };
    namespace detail
    {
        inline GLuint compile_shader(GLenum shader_type, std::string_view source)
        {
            GLuint shader = glCreateShader(shader_type);
            if (shader == 0)
                throw invalid_shader("Create shader failed");
            auto source_string = source.data();
            glShaderSource(shader, 1, &source_string, nullptr);
            glCompileShader(shader);
            GLint success = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                using namespace std::literals;
                char infolog[1024];
                glGetShaderInfoLog(shader, 1024, nullptr, infolog);
                throw compile_shader_failed(
                    "ERROR::SHADER::COMPILATION_FAILED\nError info:\n"s +
                    infolog);
            }
            return shader;
        }
    }
    shader_handler make_shader(GLenum shader_type, std::string_view source)
    {
        return shader_handler(detail::compile_shader(shader_type, source));
    }

    inline auto shader_program_deleter = [](GLuint id)
    {
        glDeleteProgram(id);
    };
    class shader_program_handler: public manager_base<GLuint>
    {
    public:
        explicit shader_program_handler(GLuint program_id)
            : manager_base(!glIsProgram(program_id) ?
                    throw invalid_program("shader_program_handler::shader_program_handler: not a program") :
                    program_id, shader_program_deleter)
        {}
    };
    namespace detail
    {
        template <typename Shader, typename = std::enable_if_t<
            std::is_convertible_v<decltype(std::declval<Shader>().id()), GLuint>>>
        GLuint get_id(Shader const&shader)
        {
            return shader.id();
        }
        GLuint get_id(GLuint id)
        {
            return id;
        }
    }
    template <typename ...Shaders>
    shader_program_handler make_program(Shaders const &...shaders)
    {
        GLuint program_id = glCreateProgram();
        if (program_id == 0)
            throw invalid_program("Create shader program failed");
        (glAttachShader(program_id, detail::get_id(shaders)), ...);
        glLinkProgram(program_id);
        GLint success = 0;
        glGetProgramiv(program_id, GL_LINK_STATUS, &success);
        if (!success)
        {
            using namespace std::literals;
            char infolog[1024];
            glGetProgramInfoLog(program_id, 1024, nullptr, infolog);
            throw link_program_failed("ERROR::SHADER::LINK_PROGRAM_FAILED\nError info:\n"s + infolog);
        }
        return shader_program_handler(program_id);
    }
}
