#include <iostream>
#include "types/Any.h"
#include "Serializator.h"
#include <vector>
#include <cstddef>
#include <iomanip> 
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
    StringType s5 = "bibaiba";

    //https://en.cppreference.com/w/cpp/string/basic_string/basic_string
    StringType sx_3(4, '=');

    char mutable_c_str[] = "another C-style string";
    StringType sx_4(std::begin(mutable_c_str) + 8, std::end(mutable_c_str) - 1);
    StringType sx_6("C-style string", 7);
    StringType sx_7("C-style\0 string");

    std::string const other11("Exemplar");
    StringType sx_11(other11);
    StringType sx_12(std::string("C++ by ") + std::string("example"));

    std::string const other15("Mutatis Mutandis");
    StringType sx_15(other15, 8);

    std::string const other17("Exemplary");
    StringType sx_17(other17, 0, other17.length() - 1);

    StringType sx_19 = { 'C', '-', 's', 't', 'y', 'l', 'e' };
    //~https://en.cppreference.com/w/cpp/string/basic_string/basic_string

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

    setlocale(0, "RU");
    std::cout << "Инициализации всех типов прошли успешно\n";

    Serializator s;
    s.push(VectorType(StringType("qwerty"), IntegerType(100500)));
    s.push(VectorType(IntegerType(100500), IntegerType(100500), IntegerType(100500)));
    s.push(VectorType(StringType("invector")));
    s.push(StringType("qwerty"));
    s.push(IntegerType(100500));
    auto buffer = s.serialize();
    auto result = Serializator::deserialize(buffer);

    //std::cout << "}" << std::endl;
    std::cout << bool(s.getStorage() == result);

    return 0;
}