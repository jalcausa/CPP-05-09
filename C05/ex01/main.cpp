#include "Bureaucrat.hpp"
#include "Form.hpp"

int	main()
{
	try
	{
		Bureaucrat alice("Alice", 42);
		Form taxForm("TaxForm", 50, 20);
		std::cout << alice << std::endl;
		std::cout << taxForm << std::endl;
		alice.signForm(taxForm);
		std::cout << taxForm << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}

	try
	{
		Bureaucrat intern("Intern", 150);
		Form topSecret("TopSecret", 1, 1);
		intern.signForm(topSecret);
	}
	catch (const std::exception &e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}
	return (0);
}
