#ifndef SPAN_HPP
# define SPAN_HPP

# include <vector>
# include <exception>

class Span
{
public:
	Span(unsigned int n);
	Span(const Span& other);
	Span& operator=(const Span& other);
	~Span();

	void addNumber(int n);

	template <typename Iterator>
	void addNumbers(Iterator begin, Iterator end)
	{
		while (begin != end)
		{
			addNumber(*begin);
			++begin;
		}
	}

	int shortestSpan() const;
	int longestSpan() const;

	class FullContainerException : public std::exception
	{
	public:
		virtual const char* what() const throw();
	};

	class NotEnoughNumbersException : public std::exception
	{
	public:
		virtual const char* what() const throw();
	};

private:
	unsigned int	_maxSize;
	std::vector<int>	_numbers;
};

#endif
