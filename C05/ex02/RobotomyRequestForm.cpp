#include "RobotomyRequestForm.hpp"
#include <cstdlib>
#include <ctime>

RobotomyRequestForm::RobotomyRequestForm()
	: AForm("RobotomyRequestForm", 72, 45), _target("default")
{
}

RobotomyRequestForm::RobotomyRequestForm(const std::string &target)
	: AForm("RobotomyRequestForm", 72, 45), _target(target)
{
}

RobotomyRequestForm::RobotomyRequestForm(const RobotomyRequestForm &other)
	: AForm(other), _target(other._target)
{
}

RobotomyRequestForm &RobotomyRequestForm::operator=(const RobotomyRequestForm &other)
{
	if (this != &other)
	{
		AForm::operator=(other);
		_target = other._target;
	}
	return (*this);
}

RobotomyRequestForm::~RobotomyRequestForm()
{
}

const std::string &RobotomyRequestForm::getTarget() const
{
	return (_target);
}

void RobotomyRequestForm::execute(const Bureaucrat &executor) const
{
	static bool	seeded = false;

	checkExecutable(executor);
	if (!seeded)
	{
		std::srand(static_cast<unsigned int>(std::time(NULL)));
		seeded = true;
	}
	std::cout << "* drilling noises *" << std::endl;
	if (std::rand() % 2 == 0)
		std::cout << _target << " has been robotomized successfully." << std::endl;
	else
		std::cout << "Robotomy failed on " << _target << "." << std::endl;
}
