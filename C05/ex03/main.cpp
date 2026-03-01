#include "Bureaucrat.hpp"
#include "Intern.hpp"

int	main()
{
	Intern		intern;
	Bureaucrat	boss("Boss", 1);
	AForm		*form;

	form = intern.makeForm("robotomy request", "Bender");
	if (form != NULL)
	{
		boss.signForm(*form);
		boss.executeForm(*form);
		delete form;
	}

	form = intern.makeForm("presidential pardon", "Arthur Dent");
	if (form != NULL)
	{
		boss.signForm(*form);
		boss.executeForm(*form);
		delete form;
	}

	form = intern.makeForm("unknown form", "Nobody");
	if (form != NULL)
		delete form;
	return (0);
}
