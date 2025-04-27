#pragma once
#include <type_traits>
#include "SerializableValue.h"


class FloatType: public SerializableValue<TypeId::Float, double>
{
public:
    using Parent = SerializableValue<TypeId::Float, value_type>;

    explicit FloatType() = default;

    template<typename T, typename = std::enable_if_t<std::is_convertible_v<T, value_type>>>
    explicit FloatType(T&& _value) : Parent(std::forward<T>(_value)) {};
};