#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <iostream>
#include "stb_image.h"
#include "manager_base.hpp"

namespace CGTask::render
{
    struct texture_exception : public std::logic_error
    {
        using logic_error::logic_error;
    };
    struct invalid_texture : public texture_exception
    {
        using texture_exception::texture_exception;
    };
    struct load_texture_failed : public texture_exception
    {
        using texture_exception::texture_exception;
    };
    auto texture_deleter = [](GLuint texture_id)
    {
        glDeleteTextures(1, &texture_id);
    };
    struct texture_view
    {
        texture_view(GLuint id, std::string_view type)
            : id_(id), type_(type)
        {}
        GLuint id() const
        {
            return id_;
        }
        void use(GLenum target = GL_TEXTURE_2D) const
        {
            glBindTexture(GL_TEXTURE_2D, id());
        }
        std::string_view type() const
        {
            return type_;
        }
    private:
        GLuint id_;
        std::string_view type_;
    };
    struct texture_handler : manager_base<GLuint>
    {
        explicit texture_handler(GLuint texture_id, std::string_view type)
            : manager_base(!glIsTexture(texture_id) ? 
                    throw invalid_texture("texture_handler::texture_handler: not a texture") :
                    texture_id, texture_deleter),
              texture_type(type)
        {}
        texture_view view() const
        {
            return texture_view(id(), type());
        }
        std::string_view type() const
        {
            return texture_type;
        }
    private:
        std::string texture_type;
    };

    texture_handler make_texture(char const *path, std::string_view type, bool change_alpha = false)
    {
        int width, height, nrChannels;
        //stbi_set_flip_vertically_on_load(true);
        std::unique_ptr<unsigned char, void(*)(void*)> data(stbi_load(path, &width, &height, &nrChannels, 0), stbi_image_free);
        if (!data)
            throw load_texture_failed("In make_texture function, load texture from file failed");
        unsigned int texture_id;
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        if (nrChannels == 4)
        {
            if (change_alpha)
            {
                for (std::size_t i = 0; i < width * height; ++i)
                {
                    data.get()[i * 4 + 3] = std::min((data.get()[i * 4] + data.get()[i * 4 + 1] + data.get()[i * 4 + 2]) / 3, 100);
                }
            }
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.get());
        }
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.get());
        glGenerateMipmap(GL_TEXTURE_2D);
        return texture_handler(texture_id, type);
    }
}
