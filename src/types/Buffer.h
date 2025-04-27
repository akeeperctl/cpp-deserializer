#pragma once

#include <vector>
#include <string>
#include <algorithm>  // for copy_n
#include <iterator>   // for advance
#include <stdexcept>

namespace buffer 
{
    using type = std::vector<std::byte>;
    using iter = type::const_iterator;

    // Для trivially-copyable
    template<typename U>
    inline void writeLE(type& buf, const U& v) 
    {
        auto ptr = reinterpret_cast<const std::byte*>(&v);
        buf.insert(buf.end(), ptr, ptr + sizeof(U));
    }

    template<typename U>
    inline U readLE(iter& it, iter& end) 
    {
        auto available = std::distance(it, end);
        if (available < ptrdiff_t(sizeof(U)))
            throw std::runtime_error("readLE: недостаточно данных");

        U value{};
        std::copy_n(it, sizeof(U),
            reinterpret_cast<std::byte*>(&value));
        std::advance(it, sizeof(U));
        return value;
    }

    // Перегрузка для std::string
    template<>
    inline void writeLE(type& buf, const std::string& s) 
    {
        writeLE<uint64_t>(buf, s.size());
        auto ptr = reinterpret_cast<const std::byte*>(s.data());
        buf.insert(buf.end(), ptr, ptr + s.size());
    }

    template<>
    inline std::string readLE(iter& it, iter& end) 
    {
        auto sz = readLE<uint64_t>(it, end);
        std::string s(sz, '\0');
        std::copy_n(it, sz,
            reinterpret_cast<std::byte*>(&s[0]));
        std::advance(it, sz);
        return s;
    }
}
