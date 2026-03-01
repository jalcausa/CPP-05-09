#ifndef INTERN_HPP
#define INTERN_HPP

#include "AForm.hpp"
#include <string>

class Intern
{
  private:
	typedef AForm *(Intern::*FormCreator)(const std::string &target) const;

	AForm	*createShrubberyForm(const std::string &target) const;
	AForm	*createRobotomyForm(const std::string &target) const;
	AForm	*createPresidentialForm(const std::string &target) const;

  public:
	Intern();
	Intern(const Intern &other);
	Intern &operator=(const Intern &other);
	~Intern();

	AForm	*makeForm(const std::string &name, const std::string &target) const;
};

#endif
