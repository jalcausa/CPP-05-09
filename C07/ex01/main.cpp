#include <iostream>
#include <string>
#include "iter.hpp"

template <typename T>
void printElem(T const &elem)
{
    std::cout << elem << std::endl;
}

template <typename T>
void doubleElem(T &elem)
{
    elem *= 2;
}

int main(void)
{
    // --- Non-const integer array ---
    int intArr[] = {1, 2, 3, 4, 5};
    std::size_t intLen = 5;

    std::cout << "Integer array:" << std::endl;
    ::iter(intArr, intLen, printElem<int>);

    ::iter(intArr, intLen, doubleElem<int>);
    std::cout << "After doubling:" << std::endl;
    ::iter(intArr, intLen, printElem<int>);

    std::cout << std::endl;

    // --- Const string array ---
    const std::string strArr[] = {"hello", "world", "42"};
    std::cout << "Const string array:" << std::endl;
    ::iter(strArr, 3, printElem<std::string>);

    std::cout << std::endl;

    // --- Float array ---
    float floatArr[] = {1.1f, 2.2f, 3.3f};
    std::cout << "Float array:" << std::endl;
    ::iter(floatArr, 3, printElem<float>);

    return 0;
}
