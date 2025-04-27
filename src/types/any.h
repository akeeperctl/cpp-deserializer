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
    Any(const Any&) = default;
    Any(Any&&) = default;

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

    buffer::iter deserialize(buffer::iter _begin, buffer::iter _end)
    {
        auto visitor = [&_begin, &_end](auto& payload)
            {
                return payload.deserialize(_begin, _end);
            };
        return std::visit(visitor, m_payload);
    }

    TypeId getPayloadTypeId() const
    {
        return static_cast<TypeId>(m_payload.index());
    }

    bool operator == (const Any& _o) const
    {
        return m_payload == _o.m_payload;
    }

private:
    static auto resolve(int x) { return IntegerType(x); }
    static auto resolve(uint64_t x) { return IntegerType(x); }
    static auto resolve(double x) { return FloatType(x); }
    static auto resolve(const char* x) { return StringType(x); }
    static auto resolve(const std::string& x) { return StringType(x); }
    static auto resolve(std::string&& x) { return StringType(std::move(x)); }
    //static auto resolve(std::vector<Any>&& x) { return VectorType(std::move(x)); }

    static auto resolve(IntegerType& x) { return x; }
    static auto resolve(FloatType& x) { return x; }
    static auto resolve(StringType& x) { return x; }
    static auto resolve(VectorType& x) { return x; }
    static auto resolve(VectorType&& x) { return std::move(x); }

    static auto resolve(const Any& a) { return a; }
    static auto resolve(Any&& a) { return std::move(a); }

    Payload m_payload;
};

namespace buffer
{
    /// <summary>
    /// «аписывает байты переменной в буфер.
    /// </summary>
    /// <param name="_buff вектор, куда идет запись байтов."></param>
    /// <param name="_v вектор, который нужно записать в буфер"></param>
    template<>
    static void buffer::writeLE<std::vector<Any>>(buffer::type& _buff, const std::vector<Any>& _v)
    {
        writeLE<Id>(_buff, _v.size());
        for (auto& element : _v)
            element.serialize(_buff);
    }

    template<>
    static std::vector<Any> readLE<std::vector<Any>>(buffer::iter& _begin, buffer::iter& _end)
    {
        auto size = readLE<Id>(_begin, _end);
        std::vector<Any> v;
        v.resize(size);

        for (size_t i = 0; i < size; i++)
            _begin = v[i].deserialize(_begin, _end);

        return v;
    }
}
