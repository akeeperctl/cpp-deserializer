#pragma once
#include <type_traits>
#include <string>
#include "SerializableValue.h"

class StringType: public SerializableValue<TypeId::String, std::string>
{
public:
    using Parent = SerializableValue<TypeId::String, value_type>;

    explicit StringType() = default;

    explicit StringType(value_type&& _val)
        : Parent(std::move(_val))
    {
    }

    template<typename T, typename = std::enable_if_t<std::is_convertible_v<T, value_type>>>
    explicit StringType(T&& _value) : Parent(std::forward<T>(_value)) {};
};

namespace buffer
{
    // Перегрузка для std::string
    template<>
    inline void writeLE(type& buf, const std::string& s)
    {
        writeLE<uint64_t>(buf, s.size());
        auto ptr = reinterpret_cast<const std::byte*>(s.data());
        buf.insert(buf.end(), ptr, ptr + s.size());
    }

    template<>
    inline std::string _readLE(iter& it, iter& end)
    {
        auto sz = _readLE<uint64_t>(it, end);
        std::string s(sz, '\0');
        std::copy_n(it, sz, reinterpret_cast<std::byte*>(&s[0]));
        std::advance(it, sz);
        return s;
    }
}