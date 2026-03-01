#include "Bureaucrat.hpp"
#include "PresidentialPardonForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "ShrubberyCreationForm.hpp"

int	main()
{
	Bureaucrat				boss("Boss", 1);
	Bureaucrat				worker("Worker", 140);
	ShrubberyCreationForm	shrub("home");
	RobotomyRequestForm		robot("Bender");
	PresidentialPardonForm	pardon("Arthur Dent");

	worker.signForm(shrub);
	worker.executeForm(shrub);
	boss.executeForm(shrub);

	boss.signForm(robot);
	boss.executeForm(robot);

	boss.signForm(pardon);
	boss.executeForm(pardon);
	return (0);
}
