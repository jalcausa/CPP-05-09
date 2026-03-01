#ifndef SHRUBBERYCREATIONFORM_HPP
#define SHRUBBERYCREATIONFORM_HPP

#include "AForm.hpp"

class ShrubberyCreationForm : public AForm
{
  private:
	std::string	_target;

  public:
	ShrubberyCreationForm();
	ShrubberyCreationForm(const std::string &target);
	ShrubberyCreationForm(const ShrubberyCreationForm &other);
	ShrubberyCreationForm &operator=(const ShrubberyCreationForm &other);
	~ShrubberyCreationForm();

	const std::string	&getTarget() const;

	void				execute(const Bureaucrat &executor) const;

	class FileOpenException : public std::exception
	{
	  public:
		const char	*what() const throw();
	};
};

#endif
