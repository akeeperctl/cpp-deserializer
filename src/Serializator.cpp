#include "types/Buffer.h"
#include "types/SerializableValue.h"
#include "types/Any.h"
#include "Serializator.h"
#include <typeinfo>

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

    auto size = buffer::_readLE<Id>(it, end);
    result.reserve(size);

    for (uint64_t i = 0; i < size; ++i)
    {
        auto typeId = static_cast<TypeId>(buffer::_readLE<Id>(it, end));
        Any value;

        switch (typeId) 
        {
        case TypeId::Uint:
            value = Any(buffer::read<IntegerType>(it, end));
            break;
        case TypeId::Float:
            value = Any(buffer::read<FloatType>(it, end));
            break;
        case TypeId::String:
            value = Any(buffer::read<StringType>(it, end));
            break;
        case TypeId::Vector:
            value = Any(buffer::read<VectorType>(it, end));
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
