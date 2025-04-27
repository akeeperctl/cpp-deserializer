#pragma once
#include <vector>

using Id = uint64_t;

enum class TypeId : Id 
{
    Uint,
    Float,
    String,
    Vector,
};