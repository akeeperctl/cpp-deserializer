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
        Any value;
        value.deserialize(it, end);
        result.push_back(std::move(value));
    }

    return result;
}

const std::vector<Any>& Serializator::getStorage() const
{
    return m_storage;
}
