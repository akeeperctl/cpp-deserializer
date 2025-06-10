#pragma once
#include <type_traits>
#include <string>
#include "SerializableValue.h"

class StringType: public SerializableValue<TypeId::String, std::string>
{
public:
    using Parent = SerializableValue<TypeId::String, value_type>;

    explicit StringType() = default;
    explicit StringType(value_type&& _val) : Parent(std::move(_val)) {};
    explicit StringType(size_t count, char ch) : Parent(count, ch) {};
    StringType(const char* _val) : Parent(_val) {};
    StringType(std::initializer_list<char>&& _val) : Parent(std::move(_val)) {};

    template<class InputIt>
    explicit StringType(InputIt&& first, InputIt&& last) : Parent(std::forward<InputIt>(first), std::forward<InputIt>(last)) {};
    explicit StringType(const char* s, size_t count) : Parent(s, count) {};
    explicit StringType(const value_type& s, size_t count) : Parent(s, count) {};
    explicit StringType(const value_type& s, size_t pos, size_t count) : Parent(s, pos, count) {};

    template<typename T, typename = std::enable_if_t<std::is_convertible_v<T, value_type>>>
    explicit StringType(T&& _value) : Parent(std::forward<T>(_value)) {};
};

namespace buffer
{
    /// <summary>
    /// Записывает в буфер байты только из строки
    /// </summary>
    /// <param name="buf"></param>
    /// <param name="s"></param>
    template<>
    inline void writeLE(type& buf, const std::string& s)
    {;
        writeLE<uint64_t>(buf, s.size());
        auto ptr = reinterpret_cast<const std::byte*>(s.data());
        buf.insert(buf.end(), ptr, ptr + s.size());
    }
    
    /// <summary>
    /// Считывает из буфера байты только в строку
    /// </summary>
    /// <param name="it"></param>
    /// <param name="end"></param>
    /// <returns></returns>
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