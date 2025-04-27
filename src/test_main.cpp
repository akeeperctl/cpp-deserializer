#include <iostream>
#include "types/Any.h"

int main()
{
    ////Any
    Any my_any = Any();
    auto b = my_any.getPayloadTypeId();

    //// IntegerType
    IntegerType i1(42);
    uint64_t i2val = 123;
    IntegerType i2(i2val);
    const uint64_t i3val = 999;
    IntegerType i3(i3val);

    // FloatType
    FloatType f1(3.14);
    double f2val = 2.718;
    FloatType f2(f2val);
    const double f3val = 1.618;
    FloatType f3(f3val);

    // StringType
    StringType s1("hello");
    std::string s2val = "world";
    StringType s2(s2val);
    const std::string s3val = "mech";
    StringType s3(s3val);
    StringType s4(std::string("bibaboba"));

    // VectorType — из ссылок
    VectorType v1(i1, f1, s1);

    // VectorType — из временных объектов
    VectorType v2(IntegerType(1), FloatType(2.2), StringType("test"));

    //// VectorType — вложенность
    VectorType inner(f2, s2);
    VectorType v3(i3, inner);

    //// push_back
    VectorType v4;
    v4.push_back(i1);
    v4.push_back(FloatType(9.81));
    v4.push_back(StringType("gravity"));
    v4.push_back(VectorType(i2, f3));

    //std::vector<FloatType> v5 = {};
    //VectorType(v5);

    // VectorType v7 = std::vector<Any>{1, 4, 6};

    setlocale(0, "RU");
    std::cout << "Инициализации всех типов прошли успешно\n";

    return 0;
}