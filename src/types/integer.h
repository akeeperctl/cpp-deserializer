#pragma once
#include "type_traits"
#include "SerializableValue.h"

class IntegerType: public SerializableValue<TypeId::Uint, uint64_t> {
public:
    using Parent = SerializableValue<TypeId::Uint, uint64_t>;
    
    template<
        typename T1,
        typename = std::enable_if_t<std::is_integral_v<T1>>
    >
    IntegerType(T1 _value): Parent(_value) {};

    using Parent::serialize;
    using Parent::deserialize;
    using Parent::getValue;
};