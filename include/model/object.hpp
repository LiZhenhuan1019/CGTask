#pragma once
#include <memory>
#include <type_traits>
#include "model/transformable.hpp"

namespace CGTask::model
{
    struct object
    {
    public:
        object(std::size_t mesh_id, transformable trans = transform_value())
            : transform(std::move(trans)), mesh_id(mesh_id)
        {}
        transformable transform;
        std::size_t mesh() const
        {
            return mesh_id;
        }
    private:
        std::size_t mesh_id;
    };
}
