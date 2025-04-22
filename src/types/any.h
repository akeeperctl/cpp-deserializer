#pragma once
#include "common.h"

class Any {
public:
    template<typename ...Args>
    Any(Args&& ... args);

    bool operator == (const Any& _o) const;
};