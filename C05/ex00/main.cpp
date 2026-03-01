#include "Bureaucrat.hpp"

int	main()
{
	try
	{
		Bureaucrat bob("Bob", 2);
		std::cout << bob << std::endl;
		bob.incrementGrade();
		std::cout << bob << std::endl;
		bob.incrementGrade();
	}
	catch (const std::exception &e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}

	try
	{
		Bureaucrat invalidHigh("InvalidHigh", 0);
		std::cout << invalidHigh << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}

	try
	{
		Bureaucrat invalidLow("InvalidLow", 151);
		std::cout << invalidLow << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}
	return (0);
}
