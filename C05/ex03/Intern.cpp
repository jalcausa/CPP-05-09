#include "Intern.hpp"
#include "PresidentialPardonForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include <iostream>

Intern::Intern()
{
}

Intern::Intern(const Intern &other)
{
	(void)other;
}

Intern &Intern::operator=(const Intern &other)
{
	(void)other;
	return (*this);
}

Intern::~Intern()
{
}

AForm *Intern::createShrubberyForm(const std::string &target) const
{
	return (new ShrubberyCreationForm(target));
}

AForm *Intern::createRobotomyForm(const std::string &target) const
{
	return (new RobotomyRequestForm(target));
}

AForm *Intern::createPresidentialForm(const std::string &target) const
{
	return (new PresidentialPardonForm(target));
}

AForm *Intern::makeForm(const std::string &name, const std::string &target) const
{
	const std::string	formNames[3] = {
		"shrubbery creation", "robotomy request", "presidential pardon"};
	FormCreator			formCreators[3] = {
		&Intern::createShrubberyForm, &Intern::createRobotomyForm, &Intern::createPresidentialForm};

	for (int i = 0; i < 3; ++i)
	{
		if (name == formNames[i])
		{
			std::cout << "Intern creates " << name << std::endl;
			return ((this->*formCreators[i])(target));
		}
	}
	std::cout << "Intern couldn't create " << name << std::endl;
	return (NULL);
}
