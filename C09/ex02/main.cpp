#include "PmergeMe.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: PmergeMe <positive integers...>" << std::endl;
        return 1;
    }

    PmergeMe pm;
    pm.sortAndDisplay(argc, argv);

    return 0;
}
