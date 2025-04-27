#include "types/SerializableValue.h"
#include "types/Any.h"
#include "Serializator.h"
#include <typeinfo>

/// <summary>
/// Вспомогательная функция для считывания данных.
/// Считывает данные с буфера и возвращает нужный тип.
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="it"></param>
/// <param name="end"></param>
/// <returns></returns>
template<typename T>
T read(buffer::iter& it, buffer::iter end)
{
    using Decayed = std::decay_t<T>;
    auto raw = buffer::readLE<typename Decayed::value_type>(it, end);
    return Decayed(std::move(raw));
}

buffer::type Serializator::serialize() const 
{
    buffer::type result;
    buffer::writeLE<Id>(result, m_storage.size());
    for (const auto& val : m_storage)
        val.serialize(result);
    return result;
}

std::vector<Any> Serializator::deserialize(const buffer::type& _val) 
{
    std::vector<Any> result;
    auto it = _val.cbegin();
    auto end = _val.cend();

    auto size = buffer::readLE<Id>(it, end);
    result.reserve(size);

    for (uint64_t i = 0; i < size; ++i)
    {
        auto typeId = static_cast<TypeId>(buffer::readLE<Id>(it, end));
        Any value;

        switch (typeId) 
        {
        case TypeId::Uint:
            value = Any(read<IntegerType>(it, end));
            break;
        case TypeId::Float:
            value = Any(read<FloatType>(it, end));
            break;
        case TypeId::String:
            value = Any(read<StringType>(it, end));
            break;
        case TypeId::Vector:
            value = Any(read<VectorType>(it, end));
            break;
        default:
            throw std::runtime_error("Serializator: неизвестный TypeId");
        }
        result.push_back(std::move(value));
    }

    return result;
}

const std::vector<Any>& Serializator::getStorage() const
{
    return m_storage;
}
