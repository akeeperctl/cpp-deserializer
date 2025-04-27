#pragma once
#include <type_traits>
#include "SerializableValue.h"


class IntegerType: public SerializableValue<TypeId::Uint, uint64_t>
{
public:
    using Parent = SerializableValue<TypeId::Uint, value_type>;

    explicit IntegerType() = default;

    template<typename T, typename = std::enable_if_t<std::is_convertible_v<T, value_type>>>
    explicit IntegerType(T&& _value): Parent(std::forward<T>(_value)) {};
};