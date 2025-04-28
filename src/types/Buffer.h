#pragma once

#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator> 
#include <stdexcept>

namespace buffer 
{
    using type = std::vector<std::byte>;
    using iter = type::const_iterator;

    /// <summary>
    /// «аписывает в буфер байты любого типа данных.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="buf"></param>
    /// <param name="v"></param>
    template<typename T>
    inline void writeLE(type& buf, const T& v) 
    {
        auto ptr = reinterpret_cast<const std::byte*>(&v);
        buf.insert(buf.end(), ptr, ptr + sizeof(T));
    }

    /// <summary>
    /// —читывает XType из буфера и возвращает значение в XType типе.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="it"></param>
    /// <param name="end"></param>
    /// <returns></returns>
    template<typename T, typename = std::enable_if_t<is_serializable<T>::value>>
    T read(buffer::iter& it, buffer::iter& end)
    {
        using Decayed = std::decay_t<T>;
        auto raw = buffer::_readLE<typename Decayed::value_type>(it, end);
        return Decayed(std::move(raw));
    }

    /// <summary>
    /// —читывает тривиальные типы, которые не €вл€ютс€ XType 
    /// и возвращает тривиальный тип
    /// </summary>
    /// <typeparam name="U"></typeparam>
    /// <param name="it"></param>
    /// <param name="end"></param>
    /// <returns></returns>
    template<typename U>
    inline U _readLE(iter& it, iter& end) 
    {
        auto available = std::distance(it, end);
        auto size = ptrdiff_t(sizeof(U));
        if (available < size)
            throw std::runtime_error("_readLE: недостаточно данных");

        U value = U();
        std::copy_n(it, sizeof(U), reinterpret_cast<std::byte*>(&value));
        std::advance(it, sizeof(U));
        return value;
    }
}
