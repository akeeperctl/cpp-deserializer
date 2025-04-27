#pragma once
#include <string>
#include <vector>

namespace buffer
{
    namespace
    {
        template<typename T>
        inline const std::byte* get_bytes(const T& _value)
        {
            return reinterpret_cast<const std::byte*>(&_value);
        };

        template<typename T>
        inline std::byte* get_bytes(T& _value)
        {
            return reinterpret_cast<std::byte*>(&_value);
        };
    }


    using type = std::vector<std::byte>;
    using iter = type::const_iterator;

    template<typename U>
    static void writeLE(buffer::type& _buff, const U& _value)
    {
        auto ptr = buffer::get_bytes(_value);
        _buff.insert(_buff.end(), ptr, ptr + sizeof(U));
    }

    /// <summary>
    /// ѕобайтово считывает буфер и сохран€ет байты в переменную с шаблонным типом.
    /// </summary>
    /// <typeparam name="U"></typeparam>
    /// <param name="_begin">итератор буфера, указывающий на его начало.</param>
    /// <param name="_end">итератор буфера, указывающий на его конец.</param>
    /// <returns>U переменна€, значение которой воссозданно из байтов.</returns>
    template<typename U>
    static U readLE(buffer::iter& _begin, buffer::iter& _end)
    {
        const auto data_size = std::distance(_begin, _end);
        const auto type_size = static_cast<ptrdiff_t>(sizeof(U));
        if (data_size < type_size)
            throw std::runtime_error("readLE: недостаточно данных буфера дл€ чтени€ типа");

        U value = 0;
        std::copy_n(_begin, type_size, buffer::get_bytes(value));
        std::advance(_begin, type_size);
        return value;
    }

    /// <summary>
    /// «аписывает байты переменной в буфер.
    /// </summary>
    /// <param name="_buff вектор, куда идет запись байтов."></param>
    /// <param name="_v вектор, который нужно записать в буфер"></param>
    template<>
    static void buffer::writeLE<std::string>(buffer::type& _buff, const std::string& _s)
    {
        const auto size = _s.size();
        const auto ptr = buffer::get_bytes(_s.data());
        writeLE<Id>(_buff, size);
        _buff.insert(_buff.cend(), ptr, ptr + size);
    }

    template<>
    static std::string readLE<std::string>(buffer::iter& _begin, buffer::iter& _end)
    {
        auto size = readLE<Id>(_begin, _end);
        std::string s(size, '\0');
        std::copy_n(_begin, size, reinterpret_cast<std::byte*>(&s[0]));
        std::advance(_begin, size);
        return s;
    }

}