#include "PmergeMe.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <sys/time.h>

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe &other)
    : _vec(other._vec), _deq(other._deq) {}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
    if (this != &other)
    {
        _vec = other._vec;
        _deq = other._deq;
    }
    return *this;
}

PmergeMe::~PmergeMe() {}

// ---------------------------------------------------------------------------
// Jacobsthal sequence: J(0)=0, J(1)=1, J(n)=J(n-1)+2*J(n-2)
// ---------------------------------------------------------------------------
static size_t jacobsthal(size_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    size_t prev2 = 0, prev1 = 1, cur = 0;
    for (size_t i = 2; i <= n; ++i)
    {
        cur = prev1 + 2 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}

// ---------------------------------------------------------------------------
// Binary insertion: insert val into chain in position [0..end)
// ---------------------------------------------------------------------------
void PmergeMe::binaryInsertVec(std::vector<int> &seq, int val, size_t end)
{
    if (end > seq.size())
        end = seq.size();
    size_t lo = 0, hi = end;
    while (lo < hi)
    {
        size_t mid = lo + (hi - lo) / 2;
        if (seq[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    seq.insert(seq.begin() + static_cast<std::vector<int>::difference_type>(lo), val);
}

void PmergeMe::binaryInsertDeq(std::deque<int> &seq, int val, size_t end)
{
    if (end > seq.size())
        end = seq.size();
    size_t lo = 0, hi = end;
    while (lo < hi)
    {
        size_t mid = lo + (hi - lo) / 2;
        if (seq[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    seq.insert(seq.begin() + static_cast<std::deque<int>::difference_type>(lo), val);
}

// ---------------------------------------------------------------------------
// Insert losers into a sorted chain using Jacobsthal order.
// pairs[] is sorted by first (winner). pairs[0].second is already in chain.
// We insert pairs[i].second for i >= 1.
// The upper search bound for pairs[i].second is the position of pairs[i].first
// in chain (the loser is guaranteed <= its paired winner).
// ---------------------------------------------------------------------------
static void insertLosersVec(std::vector<int> &chain,
                             const std::vector<std::pair<int,int> > &pairs,
                             PmergeMe &pm)
{
    size_t pendingSize = pairs.size() - 1; // pairs[0].second already in chain
    if (pendingSize == 0)
        return;

    std::vector<bool> inserted(pendingSize, false);

    // Jacobsthal groups: indices go jPrev..jCurr-1 inserted high-to-low
    size_t jacobIdx = 2;
    while (true)
    {
        size_t jCurr = jacobsthal(jacobIdx);
        size_t jPrev = jacobsthal(jacobIdx - 1);

        if (jPrev >= pendingSize)
            break;

        size_t hiIdx = (jCurr <= pendingSize) ? jCurr - 1 : pendingSize - 1;

        // Insert from hiIdx down to jPrev (inclusive)
        for (size_t idx = hiIdx + 1; idx > jPrev; --idx)
        {
            size_t i = idx - 1; // 0-based index into pending (pending[i] = pairs[i+1].second)
            if (i < pendingSize && !inserted[i])
            {
                int val          = pairs[i + 1].second;
                int pairedWinner = pairs[i + 1].first;

                // Find pairedWinner in chain — upper bound is its index (inclusive)
                size_t upperBound = chain.size();
                for (size_t k = 0; k < chain.size(); ++k)
                {
                    if (chain[k] == pairedWinner)
                    {
                        upperBound = k + 1; // search includes up to winner position
                        break;
                    }
                }
                pm.binaryInsertVec(chain, val, upperBound);
                inserted[i] = true;
            }
        }
        ++jacobIdx;
    }

    // Flush remaining (shouldn't happen if Jacobsthal covers all, but safety net)
    for (size_t i = 0; i < pendingSize; ++i)
    {
        if (!inserted[i])
        {
            int val          = pairs[i + 1].second;
            int pairedWinner = pairs[i + 1].first;
            size_t upperBound = chain.size();
            for (size_t k = 0; k < chain.size(); ++k)
            {
                if (chain[k] == pairedWinner)
                {
                    upperBound = k + 1;
                    break;
                }
            }
            pm.binaryInsertVec(chain, val, upperBound);
        }
    }
}

static void insertLosersDeq(std::deque<int> &chain,
                             const std::vector<std::pair<int,int> > &pairs,
                             PmergeMe &pm)
{
    size_t pendingSize = pairs.size() - 1;
    if (pendingSize == 0)
        return;

    std::vector<bool> inserted(pendingSize, false);

    size_t jacobIdx = 2;
    while (true)
    {
        size_t jCurr = jacobsthal(jacobIdx);
        size_t jPrev = jacobsthal(jacobIdx - 1);

        if (jPrev >= pendingSize)
            break;

        size_t hiIdx = (jCurr <= pendingSize) ? jCurr - 1 : pendingSize - 1;

        for (size_t idx = hiIdx + 1; idx > jPrev; --idx)
        {
            size_t i = idx - 1;
            if (i < pendingSize && !inserted[i])
            {
                int val          = pairs[i + 1].second;
                int pairedWinner = pairs[i + 1].first;

                size_t upperBound = chain.size();
                for (size_t k = 0; k < chain.size(); ++k)
                {
                    if (chain[k] == pairedWinner)
                    {
                        upperBound = k + 1;
                        break;
                    }
                }
                pm.binaryInsertDeq(chain, val, upperBound);
                inserted[i] = true;
            }
        }
        ++jacobIdx;
    }

    for (size_t i = 0; i < pendingSize; ++i)
    {
        if (!inserted[i])
        {
            int val          = pairs[i + 1].second;
            int pairedWinner = pairs[i + 1].first;
            size_t upperBound = chain.size();
            for (size_t k = 0; k < chain.size(); ++k)
            {
                if (chain[k] == pairedWinner)
                {
                    upperBound = k + 1;
                    break;
                }
            }
            pm.binaryInsertDeq(chain, val, upperBound);
        }
    }
}

// ---------------------------------------------------------------------------
// Ford-Johnson for std::vector
// ---------------------------------------------------------------------------
void PmergeMe::fordJohnsonVec(std::vector<int> &seq)
{
    size_t n = seq.size();
    if (n <= 1)
        return;

    if (n == 2)
    {
        if (seq[0] > seq[1])
            std::swap(seq[0], seq[1]);
        return;
    }

    bool hasStraggler = (n % 2 != 0);
    int  straggler    = 0;
    if (hasStraggler)
    {
        straggler = seq.back();
        seq.pop_back();
    }

    // Build (winner, loser) pairs — winner >= loser
    size_t numPairs = seq.size() / 2;
    std::vector<std::pair<int,int> > pairs(numPairs);
    for (size_t i = 0; i < numPairs; ++i)
    {
        int a = seq[2 * i];
        int b = seq[2 * i + 1];
        pairs[i] = (a >= b) ? std::make_pair(a, b) : std::make_pair(b, a);
    }

    // Recursively sort winners
    std::vector<int> winners;
    winners.reserve(numPairs);
    for (size_t i = 0; i < numPairs; ++i)
        winners.push_back(pairs[i].first);

    fordJohnsonVec(winners); // sorts in place

    // Rebuild pairs sorted by winner, preserving loser correspondence.
    // Match each winner (now sorted) to its original pair by value.
    // If there are duplicate winners, we match them in the order they appear
    // (stable match by first unmatched).
    std::vector<bool> matched(numPairs, false);
    std::vector<std::pair<int,int> > sortedPairs(numPairs);
    for (size_t i = 0; i < numPairs; ++i)
    {
        for (size_t j = 0; j < numPairs; ++j)
        {
            if (!matched[j] && pairs[j].first == winners[i])
            {
                sortedPairs[i] = pairs[j];
                matched[j] = true;
                break;
            }
        }
    }
    pairs = sortedPairs;

    // Build chain starting with all winners
    std::vector<int> chain;
    chain.reserve(2 * numPairs + 1);
    for (size_t i = 0; i < numPairs; ++i)
        chain.push_back(pairs[i].first);

    // Prepend pairs[0].second — it is <= pairs[0].first (the minimum winner)
    chain.insert(chain.begin(), pairs[0].second);

    // Insert remaining losers using Jacobsthal order
    insertLosersVec(chain, pairs, *this);

    // Insert straggler with binary search over full chain
    if (hasStraggler)
        binaryInsertVec(chain, straggler, chain.size());

    seq = chain;
}

// ---------------------------------------------------------------------------
// Ford-Johnson for std::deque (identical algorithm, different container)
// ---------------------------------------------------------------------------
void PmergeMe::fordJohnsonDeq(std::deque<int> &seq)
{
    size_t n = seq.size();
    if (n <= 1)
        return;

    if (n == 2)
    {
        if (seq[0] > seq[1])
            std::swap(seq[0], seq[1]);
        return;
    }

    bool hasStraggler = (n % 2 != 0);
    int  straggler    = 0;
    if (hasStraggler)
    {
        straggler = seq.back();
        seq.pop_back();
    }

    size_t numPairs = seq.size() / 2;
    std::vector<std::pair<int,int> > pairs(numPairs);
    for (size_t i = 0; i < numPairs; ++i)
    {
        int a = seq[2 * i];
        int b = seq[2 * i + 1];
        pairs[i] = (a >= b) ? std::make_pair(a, b) : std::make_pair(b, a);
    }

    std::deque<int> winners;
    for (size_t i = 0; i < numPairs; ++i)
        winners.push_back(pairs[i].first);

    fordJohnsonDeq(winners);

    std::vector<bool> matched(numPairs, false);
    std::vector<std::pair<int,int> > sortedPairs(numPairs);
    for (size_t i = 0; i < numPairs; ++i)
    {
        for (size_t j = 0; j < numPairs; ++j)
        {
            if (!matched[j] && pairs[j].first == winners[i])
            {
                sortedPairs[i] = pairs[j];
                matched[j] = true;
                break;
            }
        }
    }
    pairs = sortedPairs;

    std::deque<int> chain;
    for (size_t i = 0; i < numPairs; ++i)
        chain.push_back(pairs[i].first);

    chain.push_front(pairs[0].second);

    insertLosersDeq(chain, pairs, *this);

    if (hasStraggler)
        binaryInsertDeq(chain, straggler, chain.size());

    seq = chain;
}

// ---------------------------------------------------------------------------
// Public interface
// ---------------------------------------------------------------------------
void PmergeMe::sortVector()
{
    fordJohnsonVec(_vec);
}

void PmergeMe::sortDeque()
{
    fordJohnsonDeq(_deq);
}

void PmergeMe::sortAndDisplay(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i)
    {
        std::istringstream iss(argv[i]);
        long val;
        iss >> val;
        if (iss.fail() || !iss.eof() || val < 0)
        {
            std::cerr << "Error" << std::endl;
            return;
        }
        _vec.push_back(static_cast<int>(val));
        _deq.push_back(static_cast<int>(val));
    }

    std::cout << "Before:";
    for (size_t i = 0; i < _vec.size(); ++i)
        std::cout << " " << _vec[i];
    std::cout << std::endl;

    struct timeval tstart, tend;
    gettimeofday(&tstart, NULL);
    sortVector();
    gettimeofday(&tend, NULL);
    double vecTime = (tend.tv_sec - tstart.tv_sec) * 1e6
                   + (tend.tv_usec - tstart.tv_usec);

    gettimeofday(&tstart, NULL);
    sortDeque();
    gettimeofday(&tend, NULL);
    double deqTime = (tend.tv_sec - tstart.tv_sec) * 1e6
                   + (tend.tv_usec - tstart.tv_usec);

    std::cout << "After: ";
    for (size_t i = 0; i < _vec.size(); ++i)
        std::cout << " " << _vec[i];
    std::cout << std::endl;

    size_t n = _vec.size();
    std::cout << "Time to process a range of " << n
              << " elements with std::vector : " << vecTime << " us" << std::endl;
    std::cout << "Time to process a range of " << n
              << " elements with std::deque  : " << deqTime << " us" << std::endl;
}
