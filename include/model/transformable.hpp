#pragma once
#include <memory>
#include <type_traits>
#include "model/transform.hpp"

namespace CGTask::model
{
    struct transformable
    {
    public:
        template <typename Value,
                 typename = std::enable_if_t<
                     std::is_convertible_v<decltype(std::declval<Value>().transform()), struct transform const&>>>
        transformable(Value &&value)
            : eraser(std::make_unique<object_concrete<
                     std::remove_cv_t<std::remove_reference_t<Value>>>>(std::forward<Value>(value)))
        {}
        struct transform &transform()
        {
            return eraser->transform();
        }
        struct transform const &transform() const
        {
            return eraser->transform();
        }
    private:
        class object_base;

        std::unique_ptr<object_base> eraser;

        class object_base 
        {
        public:
            virtual struct transform &transform() = 0;
            virtual struct transform const &transform() const = 0;
        };
        template <typename Value>
        class object_concrete : public object_base
        {
        public:
            template <typename U, typename = std::enable_if<std::is_constructible_v<Value, U>>>
            object_concrete(U &&value)
                : value(std::forward<U>(value))
            {}
            struct transform &transform() override
            {
                return value.transform();
            }
            struct transform const &transform() const override
            {
                return value.transform();
            }
        private:
            Value value;
        };
    };

    struct transform_value
    {
        transform_value(struct transform const &trans = model::transform{})
            : trans(trans)
        {
        }
        struct transform &transform()
        {
            return trans;
        }
        struct transform const &transform() const
        {
            return trans;
        }
    private:
        struct transform trans;
    };
}
