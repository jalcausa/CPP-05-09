#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>

class PmergeMe
{
public:
    PmergeMe();
    PmergeMe(const PmergeMe &other);
    PmergeMe &operator=(const PmergeMe &other);
    ~PmergeMe();

    void sortAndDisplay(int argc, char **argv);

    void binaryInsertVec(std::vector<int> &seq, int val, size_t end);
    void binaryInsertDeq(std::deque<int> &seq, int val, size_t end);

private:
    std::vector<int> _vec;
    std::deque<int>  _deq;

    void sortVector();
    void sortDeque();

    void fordJohnsonVec(std::vector<int> &seq);
    void fordJohnsonDeq(std::deque<int> &seq);
};

#endif
