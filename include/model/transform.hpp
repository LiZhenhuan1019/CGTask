#pragma once
#include <glm/geometric.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace CGTask::model
{
    namespace space
    {
        constexpr struct self_t
        {
            explicit self_t() = default;
        } self;
        constexpr struct world_t
        {
        } world;
    }
    struct transform
    {
    public:
        transform(glm::vec3 position = glm::vec3(0))
            : pos(position), rot(1), scale_(1, 1, 1)
        {
            update();
        }
        glm::mat4 const &transform_matrix() const
        {
            return trans;
        }
        glm::vec3 const &position() const
        {
            return pos;
        }
        void position(glm::vec3 const &position)
        {
            pos = position;
            update();
        }
        void translate(glm::vec3 const &vec, space::self_t)
        {
            // resulting transform is equavalent to glm::translate(pos) * glm::translate(rot, vec)
            glm::vec4 result = rot * glm::vec4(vec, 1);
            pos += glm::vec3(result);
            update();
        }
        void translate(glm::vec3 const&vec, space::world_t = space::world)
        {
            pos += vec;
            update();
        }
        void rotate(float angle, glm::vec3 const &axis)
        {
            rot = glm::rotate(angle, glm::normalize(axis)) * rot;
            update();
        }
        glm::mat4 const &rotation() const
        {
            return rot;
        }
        void rotation(glm::mat4 const &mat)
        {
            rot = mat;
            update();
        }
        void scale(glm::vec3 const &vec)
        {
            scale_.x *= vec.x;
            scale_.y *= vec.y;
            scale_.z *= vec.z;
            update();
        }
    private:
        void update()
        {
            trans = glm::translate(pos) * glm::scale(rot, scale_);
        }
        glm::mat4 trans;
        glm::vec3 pos;
        glm::mat4 rot;
        glm::vec3 scale_;
    };
}
