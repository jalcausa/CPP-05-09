#include <iostream>
#include <vector>
#include <list>
#include <cstdlib>
#include "Span.hpp"

int main(void)
{
	std::cout << "=== Span tests ===" << std::endl;

	// --- Basic subject example ---
	std::cout << "\n-- Subject example --" << std::endl;
	{
		Span sp(5);
		sp.addNumber(6);
		sp.addNumber(3);
		sp.addNumber(17);
		sp.addNumber(9);
		sp.addNumber(11);
		std::cout << "shortestSpan() = " << sp.shortestSpan() << std::endl; // expected 2
		std::cout << "longestSpan()  = " << sp.longestSpan()  << std::endl; // expected 14
	}

	// --- Exception: container full ---
	std::cout << "\n-- addNumber on full Span --" << std::endl;
	{
		Span sp(2);
		sp.addNumber(1);
		sp.addNumber(2);
		try {
			sp.addNumber(3);
			std::cout << "No exception thrown (should not reach here)" << std::endl;
		} catch (const std::exception& e) {
			std::cerr << "Exception (expected): " << e.what() << std::endl;
		}
	}

	// --- Exception: not enough numbers ---
	std::cout << "\n-- Span with 0 or 1 numbers --" << std::endl;
	{
		Span sp(5);
		try {
			sp.shortestSpan();
		} catch (const std::exception& e) {
			std::cerr << "Exception (expected): " << e.what() << std::endl;
		}
		sp.addNumber(42);
		try {
			sp.longestSpan();
		} catch (const std::exception& e) {
			std::cerr << "Exception (expected): " << e.what() << std::endl;
		}
	}

	// --- addNumbers with a vector iterator ---
	std::cout << "\n-- addNumbers (vector range) --" << std::endl;
	{
		std::vector<int> v;
		v.push_back(10);
		v.push_back(50);
		v.push_back(30);
		v.push_back(20);
		v.push_back(40);

		Span sp(10);
		sp.addNumbers(v.begin(), v.end());
		std::cout << "shortestSpan() = " << sp.shortestSpan() << std::endl; // expected 10
		std::cout << "longestSpan()  = " << sp.longestSpan()  << std::endl; // expected 40
	}

	// --- addNumbers with a list iterator ---
	std::cout << "\n-- addNumbers (list range) --" << std::endl;
	{
		std::list<int> lst;
		lst.push_back(0);
		lst.push_back(100);
		lst.push_back(50);

		Span sp(5);
		sp.addNumbers(lst.begin(), lst.end());
		std::cout << "shortestSpan() = " << sp.shortestSpan() << std::endl; // expected 50
		std::cout << "longestSpan()  = " << sp.longestSpan()  << std::endl; // expected 100
	}

	// --- Large span test ---
	std::cout << "\n-- Large Span (10000 numbers) --" << std::endl;
	{
		Span sp(10000);
		std::vector<int> big;
		big.reserve(10000);
		for (int i = 0; i < 10000; ++i)
			big.push_back(i);
		sp.addNumbers(big.begin(), big.end());
		std::cout << "shortestSpan() = " << sp.shortestSpan() << std::endl; // expected 1
		std::cout << "longestSpan()  = " << sp.longestSpan()  << std::endl; // expected 9999
	}

	// --- Copy constructor test ---
	std::cout << "\n-- Copy constructor --" << std::endl;
	{
		Span sp1(5);
		sp1.addNumber(1);
		sp1.addNumber(10);
		sp1.addNumber(5);

		Span sp2(sp1);
		std::cout << "sp1 shortestSpan() = " << sp1.shortestSpan() << std::endl;
		std::cout << "sp2 shortestSpan() = " << sp2.shortestSpan() << std::endl;

		// Modifying sp2 should not affect sp1
		sp2.addNumber(20);
		std::cout << "sp1 longestSpan()  = " << sp1.longestSpan()  << std::endl;
		std::cout << "sp2 longestSpan()  = " << sp2.longestSpan()  << std::endl;
	}

	return 0;
}
