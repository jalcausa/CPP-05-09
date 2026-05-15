#include <iostream>
#include <list>
#include "MutantStack.hpp"

int main(void)
{
	std::cout << "=== MutantStack tests ===" << std::endl;

	// --- Subject example ---
	std::cout << "\n-- Subject example --" << std::endl;
	{
		MutantStack<int> mstack;

		mstack.push(5);
		mstack.push(17);

		std::cout << "top() = " << mstack.top() << std::endl; // 17

		mstack.pop();

		std::cout << "size() = " << mstack.size() << std::endl; // 1

		mstack.push(3);
		mstack.push(5);
		mstack.push(737);
		mstack.push(0);

		MutantStack<int>::iterator it  = mstack.begin();
		MutantStack<int>::iterator ite = mstack.end();

		++it;
		--it;

		std::cout << "Iterating MutantStack:" << std::endl;
		while (it != ite)
		{
			std::cout << *it << std::endl;
			++it;
		}
		// Expected: 5, 3, 5, 737, 0
	}

	// --- Equivalent std::list comparison ---
	std::cout << "\n-- Equivalent std::list --" << std::endl;
	{
		std::list<int> mlist;

		mlist.push_back(5);
		mlist.push_back(17);

		std::cout << "back() = " << mlist.back() << std::endl;

		mlist.pop_back();

		std::cout << "size() = " << mlist.size() << std::endl;

		mlist.push_back(3);
		mlist.push_back(5);
		mlist.push_back(737);
		mlist.push_back(0);

		std::list<int>::iterator it  = mlist.begin();
		std::list<int>::iterator ite = mlist.end();

		++it;
		--it;

		std::cout << "Iterating std::list:" << std::endl;
		while (it != ite)
		{
			std::cout << *it << std::endl;
			++it;
		}
		// Should match MutantStack output: 5, 3, 5, 737, 0
	}

	// --- Const iterator test ---
	std::cout << "\n-- Const iterators --" << std::endl;
	{
		MutantStack<int> mstack;
		mstack.push(1);
		mstack.push(2);
		mstack.push(3);

		const MutantStack<int> cmstack(mstack);
		MutantStack<int>::const_iterator cit  = cmstack.begin();
		MutantStack<int>::const_iterator cite = cmstack.end();

		std::cout << "Const iterate:";
		while (cit != cite)
		{
			std::cout << " " << *cit;
			++cit;
		}
		std::cout << std::endl; // expected: 1 2 3
	}

	// --- Reverse iterator test ---
	std::cout << "\n-- Reverse iterators --" << std::endl;
	{
		MutantStack<int> mstack;
		mstack.push(10);
		mstack.push(20);
		mstack.push(30);

		MutantStack<int>::reverse_iterator rit  = mstack.rbegin();
		MutantStack<int>::reverse_iterator rite = mstack.rend();

		std::cout << "Reverse iterate:";
		while (rit != rite)
		{
			std::cout << " " << *rit;
			++rit;
		}
		std::cout << std::endl; // expected: 30 20 10
	}

	// --- Copy constructor test ---
	std::cout << "\n-- Copy constructor --" << std::endl;
	{
		MutantStack<int> ms1;
		ms1.push(100);
		ms1.push(200);

		MutantStack<int> ms2(ms1);
		ms2.push(300);

		std::cout << "ms1 size = " << ms1.size() << std::endl; // 2
		std::cout << "ms2 size = " << ms2.size() << std::endl; // 3

		std::cout << "ms2 contents:";
		for (MutantStack<int>::iterator it = ms2.begin(); it != ms2.end(); ++it)
			std::cout << " " << *it;
		std::cout << std::endl; // expected: 100 200 300
	}

	// --- Assignment operator test ---
	std::cout << "\n-- Assignment operator --" << std::endl;
	{
		MutantStack<int> ms1;
		ms1.push(7);
		ms1.push(8);

		MutantStack<int> ms2;
		ms2.push(99);
		ms2 = ms1;

		std::cout << "ms2 after assignment:";
		for (MutantStack<int>::iterator it = ms2.begin(); it != ms2.end(); ++it)
			std::cout << " " << *it;
		std::cout << std::endl; // expected: 7 8
	}

	// --- stack member functions ---
	std::cout << "\n-- Stack member functions --" << std::endl;
	{
		MutantStack<std::string> sstack;
		sstack.push("hello");
		sstack.push("world");
		std::cout << "top = " << sstack.top() << std::endl;
		std::cout << "empty = " << sstack.empty() << std::endl;
		sstack.pop();
		std::cout << "top after pop = " << sstack.top() << std::endl;
	}

	return 0;
}
