#include <iostream>
#include <string>
#include "Array.hpp"

int main(void)
{
    // --- Default constructor: empty array ---
    Array<int> empty;
    std::cout << "Empty array size: " << empty.size() << std::endl;

    std::cout << std::endl;

    // --- Constructor with n: value-initialized elements ---
    Array<int> intArr(5);
    std::cout << "Array<int>(5), default values (should be 0):" << std::endl;
    for (unsigned int i = 0; i < intArr.size(); ++i)
        std::cout << "  intArr[" << i << "] = " << intArr[i] << std::endl;

    std::cout << std::endl;

    // Fill with values
    for (unsigned int i = 0; i < intArr.size(); ++i)
        intArr[i] = static_cast<int>(i * 10);

    // --- Copy constructor: deep copy ---
    Array<int> copy(intArr);
    copy[0] = 999;

    std::cout << "Original after copy[0] = 999:" << std::endl;
    for (unsigned int i = 0; i < intArr.size(); ++i)
        std::cout << "  intArr[" << i << "] = " << intArr[i] << std::endl;

    std::cout << "Copy:" << std::endl;
    for (unsigned int i = 0; i < copy.size(); ++i)
        std::cout << "  copy[" << i << "] = " << copy[i] << std::endl;

    std::cout << std::endl;

    // --- Assignment operator: deep copy ---
    Array<int> assigned;
    assigned = intArr;
    assigned[1] = 777;

    std::cout << "Original intArr[1] after assigned[1]=777 (must stay 10): "
              << intArr[1] << std::endl;

    std::cout << std::endl;

    // --- Out-of-bounds exception ---
    try
    {
        std::cout << intArr[10] << std::endl;
    }
    catch (std::exception &e)
    {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }

    std::cout << std::endl;

    // --- Array of strings ---
    Array<std::string> strArr(3);
    strArr[0] = "hello";
    strArr[1] = "world";
    strArr[2] = "42";

    std::cout << "String array:" << std::endl;
    for (unsigned int i = 0; i < strArr.size(); ++i)
        std::cout << "  strArr[" << i << "] = " << strArr[i] << std::endl;

    return 0;
}
