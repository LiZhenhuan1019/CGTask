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
        drawable(Value &&value)
            : object(std::make_unique<drawable_object<std::remove_cv_t<std::remove_reference_t<Value>>>>(std::forward<Value>(value)))
        {}
        void draw(render::shader_view const&shader) const
        {
            object->draw(shader);
        }
    private:
        class drawable_base
        {
        public:
            virtual ~drawable_base() = default;
            virtual void draw(render::shader_view const &shader) const = 0;
        };
        template <typename Value>
        class drawable_object : public drawable_base
        {
        public:
            template <typename U>
            drawable_object(U &&value)
                : value(std::forward<U>(value))
            {}
            void draw(render::shader_view const &shader) const override 
            {
                value.draw(shader);
            }
        private:
            Value value;
        };
        std::unique_ptr<drawable_base> object;
    };
    using drawable_ref = std::reference_wrapper<drawable>;
}
