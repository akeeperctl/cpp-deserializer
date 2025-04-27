#pragma once

#include "SerializableValue.h"
#include "vector"

class Any;
class VectorType : public SerializableValue<TypeId::Vector, std::vector<Any>>
{
public:
    using Parent = SerializableValue<TypeId::Vector, value_type>;

    explicit VectorType(value_type&& _val)
        : Parent(std::move(_val))
    {
    }

    template<typename... Args>
    explicit VectorType(Args&&... args)
    {
        m_value.reserve(sizeof...(Args));
        (push_back(std::forward<Args>(args)), ...);
    }

    template<typename T>
    void push_back(T&& value)
    {
        if constexpr (std::is_same_v<std::decay_t<T>, Any>)
        {
            m_value.push_back(std::forward<T>(value));
        }
        else
        {
            m_value.push_back(Any(std::forward<T>(value)));
        }
    }

};
