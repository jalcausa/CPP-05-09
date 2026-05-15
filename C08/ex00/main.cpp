#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include "easyfind.hpp"

int main(void)
{
	std::cout << "=== easyfind tests ===" << std::endl;

	// --- vector tests ---
	std::cout << "\n-- std::vector<int> --" << std::endl;
	std::vector<int> vec;
	vec.push_back(1);
	vec.push_back(5);
	vec.push_back(3);
	vec.push_back(42);
	vec.push_back(7);

	try {
		std::vector<int>::iterator it = easyfind(vec, 42);
		std::cout << "Found 42 at index " << (it - vec.begin()) << std::endl;
	} catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	try {
		std::vector<int>::iterator it = easyfind(vec, 1);
		std::cout << "Found 1 at index " << (it - vec.begin()) << std::endl;
	} catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	try {
		easyfind(vec, 99);
		std::cout << "Found 99 (should not reach here)" << std::endl;
	} catch (const std::exception& e) {
		std::cerr << "Exception (expected): " << e.what() << std::endl;
	}

	// --- list tests ---
	std::cout << "\n-- std::list<int> --" << std::endl;
	std::list<int> lst;
	lst.push_back(10);
	lst.push_back(20);
	lst.push_back(30);

	try {
		std::list<int>::iterator it = easyfind(lst, 20);
		std::cout << "Found 20 in list" << std::endl;
		(void)it;
	} catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	try {
		easyfind(lst, 999);
		std::cout << "Found 999 (should not reach here)" << std::endl;
	} catch (const std::exception& e) {
		std::cerr << "Exception (expected): " << e.what() << std::endl;
	}

	// --- deque tests ---
	std::cout << "\n-- std::deque<int> --" << std::endl;
	std::deque<int> deq;
	deq.push_back(100);
	deq.push_back(200);
	deq.push_back(300);

	try {
		std::deque<int>::iterator it = easyfind(deq, 100);
		std::cout << "Found 100 at index " << (it - deq.begin()) << std::endl;
	} catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	try {
		easyfind(deq, 0);
		std::cout << "Found 0 (should not reach here)" << std::endl;
	} catch (const std::exception& e) {
		std::cerr << "Exception (expected): " << e.what() << std::endl;
	}

	// --- const container test ---
	std::cout << "\n-- const std::vector<int> --" << std::endl;
	const std::vector<int> cvec(vec);
	try {
		std::vector<int>::const_iterator it = easyfind(cvec, 5);
		std::cout << "Found 5 at index " << (it - cvec.begin()) << std::endl;
	} catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	return 0;
}
