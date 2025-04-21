#pragma once

class StringType {
public:
    template<typename ...Args>
    StringType(Args&& ...);
};