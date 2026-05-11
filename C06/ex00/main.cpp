#include "ScalarConverter.hpp"
#include <iostream>

static void test(const std::string &literal)
{
	std::cout << "=== Input: \"" << literal << "\" ===" << std::endl;
	ScalarConverter::convert(literal);
	std::cout << std::endl;
}

int main(void)
{
	// Char literal
	test("'A'");       // single char with quotes
	test("a");         // single char without quotes

	// Integer
	test("0");
	test("42");
	test("-42");
	test("2147483647");    // INT_MAX
	test("2147483648");    // INT_MAX + 1 → int impossible

	// Float
	test("0.0f");
	test("3.14f");
	test("-3.14f");

	// Double
	test("0.0");
	test("3.14");
	test("-3.14");

	// Pseudo-literals
	test("nan");
	test("nanf");
	test("+inf");
	test("-inf");
	test("+inff");
	test("-inff");

	// Non-displayable char (ASCII 1)
	test("\x01");

	// Invalid input
	test("hello");
	test("12abc");
	test("");

	return 0;
}
