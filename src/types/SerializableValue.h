#pragma once
#include "Common.h"
#include "Buffer.h"
#include "type_traits"
#include "stdexcept"

class Any;

// forward declaration
template<TypeId id, typename T>
class SerializableValue;
// ~forward declaration

// template aliases
template<typename T>
using dec_t = std::decay_t<T>;

template<typename T>
using serializable_t = SerializableValue<dec_t<T>::type_id, typename dec_t<T>::value_type>;

// Возвращает true, если T принадлежит к SerializableValue
template<typename T>
struct is_serializable
{
    static constexpr bool value = std::is_base_of_v<serializable_t<T>, dec_t<T>>;
};

// Включает шаблон, если T принадлежит к SerializableValue
template<typename T>
using enable_if_is_ser_value = std::enable_if<is_serializable<T>::value>;
// ~template aliases

template<TypeId id, typename T>
class SerializableValue
{
public:
    using value_type = T;
    static const TypeId type_id = id;
    explicit SerializableValue() : m_typeId(static_cast<Id>(id)) {};

    template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
    explicit SerializableValue(U&& _value)
        : m_value(std::forward<U>(_value)),
        m_typeId(static_cast<Id>(id))
    {};

    void serialize(buffer::type& _buff) const 
    {
        buffer::writeLE<Id>(_buff, m_typeId);
        buffer::writeLE<T>(_buff, m_value);
    };

    buffer::iter deserialize(buffer::iter _begin, buffer::iter _end) 
    {
        m_typeId = buffer::readLE<Id>(_begin, _end);
        m_value = buffer::readLE<T>(_begin, _end);

        return _begin;
    };

    const T& getValue() const 
    { 
        return m_value; 
    };

    const TypeId getTypeId() const
    {
        return static_cast<TypeId>(m_typeId);
    }

    bool operator==(const SerializableValue& other) const 
    {
        return getValue() == other.getValue();
    }

protected:
    T m_value;
    Id m_typeId;
};
