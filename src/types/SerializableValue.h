#pragma once
#include "Common.h"
#include "type_traits"

template<
    TypeId id, 
    typename T
>
class SerializableValue
{
public:

    SerializableValue(T&& _value) : m_value(std::forward<T>(_value)) {};

    void serialize(Buffer& _buff) const {};
    BufIter deserialize(BufIter _begin, BufIter _end) {};

    //template<typename Type>
    //auto& getValue() const { return m_value; };

    //template<TypeId kId>
    auto& getValue() const { return m_value; };

    static constexpr TypeId typeId = id;

private:
    T m_value;
};