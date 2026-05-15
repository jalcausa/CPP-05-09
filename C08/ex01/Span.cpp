#include "Span.hpp"
#include <algorithm>

// --- Constructors / Destructor ---

Span::Span(unsigned int n) : _maxSize(n)
{
	_numbers.reserve(n);
}

Span::Span(const Span& other) : _maxSize(other._maxSize), _numbers(other._numbers)
{
}

Span& Span::operator=(const Span& other)
{
	if (this != &other)
	{
		_maxSize = other._maxSize;
		_numbers = other._numbers;
	}
	return *this;
}

Span::~Span()
{
}

// --- Member functions ---

void Span::addNumber(int n)
{
	if (_numbers.size() >= _maxSize)
		throw FullContainerException();
	_numbers.push_back(n);
}

int Span::shortestSpan() const
{
	if (_numbers.size() < 2)
		throw NotEnoughNumbersException();

	std::vector<int> sorted(_numbers);
	std::sort(sorted.begin(), sorted.end());

	int shortest = sorted[1] - sorted[0];
	for (std::vector<int>::size_type i = 2; i < sorted.size(); ++i)
	{
		int diff = sorted[i] - sorted[i - 1];
		if (diff < shortest)
			shortest = diff;
	}
	return shortest;
}

int Span::longestSpan() const
{
	if (_numbers.size() < 2)
		throw NotEnoughNumbersException();

	int minVal = *std::min_element(_numbers.begin(), _numbers.end());
	int maxVal = *std::max_element(_numbers.begin(), _numbers.end());
	return maxVal - minVal;
}

// --- Exception messages ---

const char* Span::FullContainerException::what() const throw()
{
	return "Span: container is full, cannot add more numbers";
}

const char* Span::NotEnoughNumbersException::what() const throw()
{
	return "Span: need at least 2 numbers to compute a span";
}
