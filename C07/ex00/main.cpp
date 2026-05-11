#include <iostream>
#include <string>
#include "whatever.hpp"

int main(void)
{
    // --- Integer tests ---
    int a = 2;
    int b = 3;

    ::swap(a, b);
    std::cout << "a = " << a << ", b = " << b << std::endl;
    std::cout << "min(a, b) = " << ::min(a, b) << std::endl;
    std::cout << "max(a, b) = " << ::max(a, b) << std::endl;

    std::cout << std::endl;

    // --- String tests ---
    std::string c = "chain1";
    std::string d = "chain2";

    ::swap(c, d);
    std::cout << "c = " << c << ", d = " << d << std::endl;
    std::cout << "min(c, d) = " << ::min(c, d) << std::endl;
    std::cout << "max(c, d) = " << ::max(c, d) << std::endl;

    std::cout << std::endl;

    // --- Equality edge cases (returns second parameter) ---
    int x = 5;
    int y = 5;
    std::cout << "min(5, 5) returns second: " << (&::min(x, y) == &y ? "yes" : "no") << std::endl;
    std::cout << "max(5, 5) returns second: " << (&::max(x, y) == &y ? "yes" : "no") << std::endl;

    return 0;
}
