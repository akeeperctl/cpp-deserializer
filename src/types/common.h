#pragma once
#include <vector>


using Id = uint64_t;
using Buffer = std::vector<std::byte>;
using BufIter = Buffer::const_iterator;

enum class TypeId : Id {
    Uint,
    Float,
    String,
    Vector
};
