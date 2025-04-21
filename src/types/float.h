#pragma once

class FloatType {
public:
    template<typename ...Args>
    FloatType(Args&& ...);
};