#pragma once
#include <exception>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <initializer_list>
#include <string_view>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <fstream>

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

    class shader_handler
    {
    public:
        explicit shader_handler(GLuint shader_id)
            : shader_id(shader_id)
        {
            if (!glIsShader(shader_id))
                throw invalid_shader("shader_handler::shader_handler: not a shader");
        }
        shader_handler(shader_handler const&) = delete;
        shader_handler(shader_handler &&src) noexcept
            : shader_id(std::exchange(src.shader_id, 0))
        {}
        shader_handler &operator=(shader_handler const&) = delete;
        shader_handler &operator=(shader_handler &&rhs) noexcept
        {
            std::swap(shader_id, rhs.shader_id);
            return *this;
        }
        ~shader_handler()
        {
            glDeleteShader(shader_id);
        }

        GLuint id() const
        {
            return shader_id;
        }
    private:
        GLuint shader_id;
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
    class shader_program_handler
    {
    public:
        explicit shader_program_handler(GLuint program_id)
            : program_id(program_id)
        {
            if (!glIsProgram(program_id))
                throw invalid_program("shader_program_handler::shader_program_handler: not a program");
        }
        shader_program_handler(shader_program_handler const&) = delete;
        shader_program_handler(shader_program_handler &&src) noexcept
            :program_id(std::exchange(src.program_id, 0))
        {}
        shader_program_handler &operator=(shader_program_handler const&) = delete;
        shader_program_handler &operator=(shader_program_handler &&rhs) noexcept
        {
            std::swap(program_id, rhs.program_id);
            return *this;
        }
        ~shader_program_handler()
        {
            glDeleteProgram(program_id);
        }
        GLuint id() const
        {
            return program_id;
        }
    private:
        GLuint program_id;
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

    class shader_manager
    {
    public:
    };
}
