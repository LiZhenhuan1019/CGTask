#pragma once
#include <functional>
#include <memory>
#include <type_traits>
#include "model/transformable.hpp"
#include "render/shader_manager.hpp"

namespace CGTask::model
{
    struct drawable
    {
    public:
        template <typename Value>
        drawable(Value &&value, render::shader_type type)
            : eraser(std::make_unique<drawable_concrete<
                    std::remove_cv_t<std::remove_reference_t<Value>>>>(std::forward<Value>(value))),
              type(type)
        {}
        void draw(render::shader_view const&shader) const
        {
            eraser->draw(shader);
        }
        render::shader_type shader_type() const
        {
            return type;
        }
    private:
        class drawable_base;
        std::unique_ptr<drawable_base> eraser;
        render::shader_type type;

        class drawable_base
        {
        public:
            virtual ~drawable_base() = default;
            virtual void draw(render::shader_view const &shader) const = 0;
        };
        template <typename Value>
        class drawable_concrete : public drawable_base
        {
        public:
            template <typename U, typename = std::enable_if<std::is_constructible_v<Value, U>>>
            drawable_concrete(U &&value)
                : value(std::forward<U>(value))
            {}
            void draw(render::shader_view const &shader) const override 
            {
                value.draw(shader);
            }
        private:
            Value value;
        };
    };
}
