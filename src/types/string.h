#pragma once
#include <type_traits>
#include <string>
#include "SerializableValue.h"

class StringType: public SerializableValue<TypeId::String, std::string>
{
public:
    using Parent = SerializableValue<TypeId::String, value_type>;
    explicit StringType() = default;

    template<typename T, typename = std::enable_if_t<std::is_convertible_v<T, value_type>>>
    explicit StringType(T&& _value) : Parent(std::forward<T>(_value)) {};
};