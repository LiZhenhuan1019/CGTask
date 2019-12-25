#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
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
    struct shader_exception : public std::logic_error
    {
        using logic_error::logic_error;
    };
    struct invalid_shader : public shader_exception
    {
        using shader_exception::shader_exception;
    };
    struct compile_shader_failed : public shader_exception
    {
        using shader_exception::shader_exception;
    };
    struct invalid_program : public shader_exception
    {
        using shader_exception::shader_exception;
    };
    struct link_program_failed : public shader_exception
    {
        using shader_exception::shader_exception;
    };
    struct read_shader_file_failed : public shader_exception
    {
        using shader_exception::shader_exception;
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
    inline shader_handler make_shader(GLenum shader_type, std::string_view source)
    {
        return shader_handler(detail::compile_shader(shader_type, source));
    }

    inline auto shader_program_deleter = [](GLuint id)
    {
        glDeleteProgram(id);
    };

    class shader_view
    {
    public:
        explicit shader_view(GLuint id)
            : id_(id)
        {}
        GLuint id() const
        {
            return id_;
        }
        template <typename NameOrLoc>
        void set(NameOrLoc name_or_loc, int value) const
        {
            glUniform1i(get_location(name_or_loc), value);
        }
        template <typename NameOrLoc>
        void set(NameOrLoc name_or_loc, float value) const
        {
            glUniform1f(get_location(name_or_loc), value);
        }
        template <typename NameOrLoc>
        void set(NameOrLoc name_or_loc, glm::vec3 const &vec) const
        {
            glUniform3fv(get_location(name_or_loc), 1, glm::value_ptr(vec));
        }
        template <typename NameOrLoc>
        void set(NameOrLoc name_or_loc, glm::vec4 const &vec) const
        {
            glUniform4fv(get_location(name_or_loc), 1, glm::value_ptr(vec));
        }
        template <typename NameOrLoc>
        void set(NameOrLoc name_or_loc, glm::mat4 const &matrix) const
        {
            glUniformMatrix4fv(get_location(name_or_loc), 1, GL_FALSE, glm::value_ptr(matrix));
        }
        GLint location(std::string_view name) const
        {
            return glGetUniformLocation(id(), name.data());
        }
        void use() const
        {
            glUseProgram(id());
        }
    private:
        GLint get_location(std::string_view name) const
        {
            return location(name);
        }
        GLint get_location(GLint loc) const
        {
            return loc;
        }
        GLuint id_;
    };
    
    class shader_program_handler: public manager_base<GLuint>
    {
    public:
        explicit shader_program_handler(GLuint program_id)
            : manager_base(!glIsProgram(program_id) ?
                    throw invalid_program("shader_program_handler::shader_program_handler: not a program") :
                    program_id, shader_program_deleter)
        {}
        shader_view view() const
        {
            return shader_view(id());
        }
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

    enum class shader_type
    {
        uniform_color_shader,
        vertex_color_shader,
        color_with_texture_shader,
        texture_lighting_shader,

        shader_number
    };

    class shader_handlers
    {
    public:
        shader_handlers()
        {
            handlers.push_back(make_program(
                        make_shader(GL_VERTEX_SHADER, load_file("assets/shaders/uniform_color.vert")),
                        make_shader(GL_FRAGMENT_SHADER, load_file("assets/shaders/uniform_color.frag"))));
            handlers.push_back(make_program(
                        make_shader(GL_VERTEX_SHADER, load_file("assets/shaders/vertex_color.vert")),
                        make_shader(GL_FRAGMENT_SHADER, load_file("assets/shaders/vertex_color.frag"))));
            handlers.push_back(make_program(
                        make_shader(GL_VERTEX_SHADER, load_file("assets/shaders/color_with_texture.vert")),
                        make_shader(GL_FRAGMENT_SHADER, load_file("assets/shaders/color_with_texture.frag"))));
            handlers.push_back(make_program(
                        make_shader(GL_VERTEX_SHADER, load_file("assets/shaders/texture_lighting.vert")),
                        make_shader(GL_FRAGMENT_SHADER, load_file("assets/shaders/texture_lighting.frag"))));
        }
        std::vector<shader_program_handler> handlers;
    };
    class shader_selector
    {
    public:
        shader_selector(std::vector<shader_program_handler> const& handlers)
        {
            assert(!handlers.empty());
            views.reserve(handlers.size());
            for (auto const &handler : handlers)
            {
                views.push_back(handler.view());
            }
            views[current].use();
        }
        shader_view select(shader_type type)
        {
            assert(type < shader_type::shader_number);
            return select(std::size_t(type));
        }
        shader_view select(std::size_t shader_id)
        {
            assert(shader_id < views.size());
            if (shader_id != current)
            {
                current = shader_id;
                views[current].use();
            }
            return views[shader_id];
        }
        template <typename Func>
        void foreach(Func func)
        {
            for (std::size_t i = 0; i < views.size(); ++i)
            {
                auto shader = select(i);
                func(shader);
            }
        }
    private:
        std::vector<shader_view> views;
        std::size_t current = 0;
    };
}
