#pragma once
#include <glm/glm.hpp>
#include <string>
#include <iterator>
#include <vector>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "model/buffer_manager.hpp"
#include "render/shader_manager.hpp"
#include "render/texture.hpp"

namespace CGTask::model
{
    struct mesh_exception : public std::logic_error
    {
        using logic_error::logic_error;
    };
    struct load_mesh_failed : public mesh_exception
    {
        using mesh_exception::mesh_exception;
    };
    struct vertex_struct
    {
        glm::vec3 positon;
        glm::vec3 normal;
        glm::vec2 texture_coord;
    };
    class mesh_handler
    {
    public:
        mesh_handler(std::string name, std::vector<vertex_struct> vertices, std::vector<unsigned int> indices, std::vector<render::texture_handler> textures)
            : name_(std::move(name)),
              vao(make_vao()),
              vbo(make_buffer(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex_struct), &vertices[0], GL_STATIC_DRAW)),
              ebo(make_buffer(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW)),
              indices_number(indices.size()),
              textures(std::move(textures))
        {
            set_vertex_attribute_pointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_struct), nullptr);
            set_vertex_attribute_pointer(1, 3, GL_FLOAT, GL_FALSE, 
                    sizeof(vertex_struct), (void *)offsetof(vertex_struct, normal));
            set_vertex_attribute_pointer(2, 2, GL_FLOAT, GL_FALSE, 
                    sizeof(vertex_struct), (void *)offsetof(vertex_struct, texture_coord));
        }
        std::string_view name() const
        {
            return name_;
        }
        void draw(render::shader_view const &shader) const
        {
            using namespace std::literals;
            for (unsigned int i = 0; i < textures.size(); ++i)
            {
                std::string_view name = textures[i].type();
                glActiveTexture(GL_TEXTURE0 + i);
                shader.set("material."s + name.data(), (int)i);
                textures[i].view().use();
            }

            vao.bind_vertex_array();
            glDrawElements(GL_TRIANGLES, indices_number, GL_UNSIGNED_INT, 0);
        }
        void add_texture(render::texture_handler new_textures)
        {
            textures.push_back(std::move(new_textures));
        }
    private:
        std::string name_;
        vao_manager vao;
        buffer_manager vbo;
        buffer_manager ebo;
        std::size_t indices_number;
        std::vector<render::texture_handler> textures;
    };
    class mesh_tree
    {
    public:
        mesh_tree(char const *path)
        {
            load_model(path);
        }
        template <typename Func>
        void foreach(Func func) const
        {
            for (mesh_handler const &mesh : meshes)
                func(mesh);
        }
        template <typename Func>
        void foreach(Func func)
        {
            for (mesh_handler &mesh : meshes)
                func(mesh);
        }
        std::vector<mesh_handler> detach()
        {
            return std::move(meshes);
        }
    private:
        std::vector<mesh_handler> meshes;
        std::string directory;

        void load_model(std::string path)
        {
            Assimp::Importer importer;
            const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                using namespace std::literals;
                throw load_mesh_failed("load mesh by assimp failed. error string: '"s + importer.GetErrorString() + "'");
            }
            directory = path.substr(0, path.find_last_of('/'));
            process_node(scene->mRootNode, scene);
        }
        void process_node(aiNode *node, aiScene const *scene)
        {
            for (std::size_t i = 0; i < node->mNumMeshes; ++i)
            {
                aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back(process_mesh(mesh, scene));
            }
            for (std::size_t i = 0; i < node->mNumChildren; ++i)
                process_node(node->mChildren[i], scene);
        }
        mesh_handler process_mesh(aiMesh *mesh, aiScene const *scene)
        {
            std::cout << "Processing mesh with name '" << mesh->mName.C_Str() << "'" << std::endl;
            std::vector<vertex_struct> vertices;
            std::vector<unsigned int> indices;
            std::vector<render::texture_handler> textures;
            for (std::size_t i = 0; i < mesh->mNumVertices; ++i)
            {
                aiVector3D const &current_vertex = mesh->mVertices[i];
                aiVector3D const &current_normal = mesh->mNormals[i];
                vertex_struct v;
                v.positon.x = current_vertex.x;
                v.positon.y = current_vertex.y;
                v.positon.z = current_vertex.z;
                v.normal.x = current_normal.x;
                v.normal.y = current_normal.y;
                v.normal.z = current_normal.z;
                if (mesh->mTextureCoords[0])
                {
                    aiVector3D const &current_tex_coord = mesh->mTextureCoords[0][i];
                    v.texture_coord.x = current_tex_coord.x;
                    v.texture_coord.y = current_tex_coord.y;
                }
                else
                {
                    std::cerr << "Warnning: no texture coordinate found in mesh";
                    v.texture_coord = glm::vec2(0.0f, 0.0f);
                }
                vertices.push_back(v);
            }
            for (std::size_t i = 0; i < mesh->mNumFaces; ++i)
            {
                aiFace const &face = mesh->mFaces[i];
                for (std::size_t j = 0; j < face.mNumIndices; ++j)
                    indices.push_back(face.mIndices[j]);
            }
            if (mesh->mMaterialIndex >= 0)
            {
                aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
                std::vector<render::texture_handler> diffuse_maps = load_matieral_textures(material, aiTextureType_DIFFUSE, "diffuse");
                textures.insert(textures.end(), std::move_iterator(diffuse_maps.begin()), std::move_iterator(diffuse_maps.end()));
            }
            return mesh_handler(mesh->mName.C_Str(), std::move(vertices), std::move(indices), std::move(textures));
        }
        std::vector<render::texture_handler> load_matieral_textures(aiMaterial *material, aiTextureType type, std::string_view type_name)
        {
            std::vector<render::texture_handler> textures;
            std::size_t material_count = material->GetTextureCount(type);
            for (std::size_t i = 0; i < material_count; ++i)
            {
                aiString str;
                material->GetTexture(type, i, &str);
                render::texture_handler texture = render::make_texture(str.C_Str(), type_name);
                textures.push_back(std::move(texture));
            }
            return textures;
        }
    };
}
