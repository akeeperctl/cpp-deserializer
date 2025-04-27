#pragma once
#include "types/common.h"
#include "types/SerializableValue.h"
#include "types/any.h"

class Serializator 
{
public:
    template<typename Arg, typename = std::enable_if_t<EnableIfAny<Arg>>>
    void push(Arg&& _val)
    {
        m_storage.push_back(Any(std::forward<Arg>(_val)));
    };

    buffer::type serialize() const;

    static std::vector<Any> deserialize(const buffer::type& _val);

    const std::vector<Any>& getStorage() const;

private:
    std::vector<Any> m_storage;
};