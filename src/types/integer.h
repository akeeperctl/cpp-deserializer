#pragma once

class IntegerType {
public:
    template<typename ...Args>
    IntegerType(Args&& ...);
};
