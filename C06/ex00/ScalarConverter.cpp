#include "ScalarConverter.hpp"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <climits>
#include <cfloat>
#include <iomanip>
#include <cctype>

// ─────────────────────────────────────────────────────────────────────────────
//  Helper: identify literal type
// ─────────────────────────────────────────────────────────────────────────────

static bool isPseudo(const std::string &s)
{
	return (s == "-inff" || s == "+inff" || s == "nanf"
		 || s == "-inf"  || s == "+inf"  || s == "nan");
}

static bool isCharLiteral(const std::string &s)
{
	// 'c' format (length 3: quote, char, quote)
	if (s.length() == 3 && s[0] == '\'' && s[2] == '\'')
		return true;
	// Single non-digit character
	if (s.length() == 1 && !isdigit(static_cast<unsigned char>(s[0])))
		return true;
	return false;
}

// ─────────────────────────────────────────────────────────────────────────────
//  Printing helpers
// ─────────────────────────────────────────────────────────────────────────────

static void printPseudo(const std::string &s)
{
	std::cout << "char: impossible" << std::endl;
	std::cout << "int: impossible" << std::endl;
	if (s == "nan" || s == "nanf")
	{
		std::cout << "float: nanf" << std::endl;
		std::cout << "double: nan" << std::endl;
	}
	else if (s == "-inf" || s == "-inff")
	{
		std::cout << "float: -inff" << std::endl;
		std::cout << "double: -inf" << std::endl;
	}
	else
	{
		std::cout << "float: +inff" << std::endl;
		std::cout << "double: +inf" << std::endl;
	}
}

static void printResults(double value)
{
	// ── char ──────────────────────────────────────────────────────────────
	if (isnan(value) || isinf(value) || value < 0.0 || value > 127.0)
		std::cout << "char: impossible" << std::endl;
	else if (static_cast<int>(value) < 32 || static_cast<int>(value) == 127)
		std::cout << "char: Non displayable" << std::endl;
	else
		std::cout << "char: '" << static_cast<char>(value) << "'" << std::endl;

	// ── int ───────────────────────────────────────────────────────────────
	if (isnan(value) || isinf(value)
		|| value > static_cast<double>(INT_MAX)
		|| value < static_cast<double>(INT_MIN))
		std::cout << "int: impossible" << std::endl;
	else
		std::cout << "int: " << static_cast<int>(value) << std::endl;

	// ── float ─────────────────────────────────────────────────────────────
	std::cout << std::fixed << std::setprecision(1);
	float fval = static_cast<float>(value);
	if (isnan(fval))
		std::cout << "float: nanf" << std::endl;
	else if (isinf(fval))
		std::cout << "float: " << (fval > 0.0f ? "+" : "-") << "inff" << std::endl;
	else
		std::cout << "float: " << fval << "f" << std::endl;

	// ── double ────────────────────────────────────────────────────────────
	if (isnan(value))
		std::cout << "double: nan" << std::endl;
	else if (isinf(value))
		std::cout << "double: " << (value > 0.0 ? "+" : "-") << "inf" << std::endl;
	else
		std::cout << "double: " << value << std::endl;
}

// ─────────────────────────────────────────────────────────────────────────────
//  Main entry point
// ─────────────────────────────────────────────────────────────────────────────

void ScalarConverter::convert(const std::string &literal)
{
	if (literal.empty())
	{
		std::cout << "char: impossible" << std::endl;
		std::cout << "int: impossible" << std::endl;
		std::cout << "float: impossible" << std::endl;
		std::cout << "double: impossible" << std::endl;
		return;
	}

	// Pseudo-literals are handled entirely before any numeric parsing
	if (isPseudo(literal))
	{
		printPseudo(literal);
		return;
	}

	double value;

	// Char literal: extract character and static_cast to double
	if (isCharLiteral(literal))
	{
		char c = (literal.length() == 1) ? literal[0] : literal[1];
		value = static_cast<double>(c);
	}
	else
	{
		// Strip trailing 'f' for float literals before strtod
		std::string toParse = literal;
		if (!toParse.empty() && toParse[toParse.length() - 1] == 'f')
			toParse = toParse.substr(0, toParse.length() - 1);

		char *end;
		value = strtod(toParse.c_str(), &end);

		// If strtod did not consume the entire string, the literal is invalid
		if (*end != '\0')
		{
			std::cout << "char: impossible" << std::endl;
			std::cout << "int: impossible" << std::endl;
			std::cout << "float: impossible" << std::endl;
			std::cout << "double: impossible" << std::endl;
			return;
		}
	}

	printResults(value);
}
