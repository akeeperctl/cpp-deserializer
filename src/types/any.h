#pragma once

#include <variant>
#include "Common.h"
#include "Buffer.h"
#include "Float.h"
#include "String.h"
#include "Integer.h"
#include "Vector.h"


using Payload = std::variant<IntegerType, FloatType, StringType, VectorType>;

template<typename T>
constexpr bool EnableIfAny = std::is_constructible_v<IntegerType, T>
    || std::is_constructible_v<FloatType, T>
    || std::is_constructible_v<StringType, T>
    || std::is_constructible_v<VectorType, T>;

class Any 
{
public:

    Any() : m_payload(IntegerType(0)) {};

    template<
        typename T,
        typename = std::enable_if_t<!std::is_same_v<std::decay_t<T>, Any> && EnableIfAny<T>>
    >    
    explicit Any(T&& value) : m_payload(resolve(std::forward<T>(value))) {}

    void serialize(buffer::type& _buff) const 
    {
        auto visitor = [&_buff](auto& payload)
            {
                payload.serialize(_buff);
            };
        std::visit(visitor, m_payload);
    }

    buffer::iter deserialize(buffer::iter& _begin, buffer::iter& _end)
    {
        TypeId typeId = static_cast<TypeId>(buffer::_readLE<uint64_t>(_begin, _end));
        switch (typeId)
        {
        case TypeId::Uint:
            m_payload = buffer::read<IntegerType>(_begin, _end);
            break;
        case TypeId::Float:
            m_payload = buffer::read<FloatType>(_begin, _end);
            break;
        case TypeId::String:
            m_payload = buffer::read<StringType>(_begin, _end);
            break;
        case TypeId::Vector:
            m_payload = buffer::read<VectorType>(_begin, _end);
            break;
        default:
            throw std::runtime_error("Any: неизвестный TypeId");
        }

        return _begin;
    }
    TypeId getPayloadTypeId() const
    {
        return static_cast<TypeId>(m_payload.index());
    }

    template<typename Type>
    auto& getValue() const
    {
        // Проверяем, что тип соответствует загруженному типу
        if (std::holds_alternative<std::decay_t<Type>>(m_payload))
        {
            // Возвращаем ссылку на значение
            return std::get<std::decay_t<Type>>(m_payload);
        }
        throw std::bad_variant_access();
    }

    template<TypeId kId>
    auto& getValue() const
    {
        // Проверяем, что тип соответствует загруженному типу по индексу
        if (m_payload.index() == static_cast<size_t>(kId))
        {
            // Возвращаем ссылку на значение
            return std::get<static_cast<size_t>(kId)>(m_payload);
        }

        throw std::bad_variant_access();
    }

    bool operator == (const Any& _o) const
    {
        return m_payload == _o.m_payload;
    }

private:
    static auto resolve(uint64_t&& x) { return IntegerType(std::move(x)); }
    static auto resolve(double&& x) { return FloatType(std::move(x)); }
    static auto resolve(const char* x) { return StringType(x); }
    static auto resolve(const std::string& x) { return StringType(x); }
    static auto resolve(std::string&& x) { return StringType(std::move(x)); }
    //static auto resolve(std::vector<Any>&& x) { return VectorType(std::move(x)); }

    static auto resolve(IntegerType& x) { return x; }
    static auto resolve(FloatType& x) { return x; }
    static auto resolve(StringType& x) { return x; }
    static auto resolve(VectorType& x) { return x; }
    static auto resolve(VectorType&& x) { return std::move(x); }

    //static auto resolve(const Any& a) { return a; }
    //static auto resolve(Any&& a) { return std::move(a); }

    Payload m_payload;
};

/// <summary>
/// Дополнение для buffer.
/// Включает в себя методы, где необходим полный класс Any
/// </summary>
namespace buffer 
{
    /// <summary>
    ///  Записывает в буфер байты из std::vector<Any>
    /// </summary>
    /// <param name="buf"></param>
    /// <param name="v"></param>
    template<>
    inline void writeLE(type& buf, const std::vector<Any>& v) 
    {
        // сначала количество элементов
        writeLE<uint64_t>(buf, v.size());

        // затем каждый Any сам себя сериализует
        for (auto const& a : v)
            a.serialize(buf);
    }

    /// <summary>
    /// Считывание только для std::vector<Any>
    /// </summary>
    /// <param name="it"></param>
    /// <param name="end"></param>
    /// <returns></returns>
    template<>
    inline std::vector<Any> _readLE(iter& it, iter& end) 
    {
        // читаем число элементов
        auto count = _readLE<uint64_t>(it, end);
        std::vector<Any> v;
        v.reserve(count);

        // для каждого элемента вызываем Any::deserialize
        for (uint64_t i = 0; i < count; ++i) 
        {
            Any a;
            a.deserialize(it, end);
            v.push_back(std::move(a));
        }
        return v;
    }
}
