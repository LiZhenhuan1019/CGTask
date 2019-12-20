#pragma once
#include <type_traits>
#include <utility>
#include <functional>

namespace CGTask
{
    template <typename Id>
    class manager_base
    {
    public:
        explicit manager_base(Id id, std::function<void(Id const&)> deleter)
            :id_(std::move(id)), deleter(deleter)
        {}
        manager_base(manager_base const&) = delete;
        manager_base(manager_base &&src) noexcept
            : id_(move_helper(src.id_)), deleter(src.deleter)
        {}
        manager_base &operator=(manager_base const&) = delete;
        manager_base &operator=(manager_base &&rhs) noexcept
        {
            if constexpr (std::is_trivially_move_assignable_v<Id>)
                std::swap(id_, rhs.id_);
            else
                id_ = std::move(rhs.id_);
            deleter = rhs.deleter;
            return *this;
        }

        Id const &id() const
        {
            return id_;
        }
        void detach()
        {
            id_ = Id();
        }
        ~manager_base()
        {
            deleter(id());
        }
    private:
        static Id move_helper(Id &id)
        {
            if constexpr (std::is_trivially_move_constructible_v<Id>)
                return std::exchange(id, Id());
            else
                return std::move(id);
        }
        Id id_;
        std::function<void(Id const&)> deleter;
    };
}
