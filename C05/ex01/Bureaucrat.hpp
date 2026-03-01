#ifndef BUREAUCRAT_HPP
#define BUREAUCRAT_HPP

#include <exception>
#include <iostream>
#include <string>

class Form;

class Bureaucrat
{
  private:
	const std::string	_name;
	int					_grade;

	void				validateGrade(int grade) const;

  public:
	Bureaucrat();
	Bureaucrat(const std::string &name, int grade);
	Bureaucrat(const Bureaucrat &other);
	Bureaucrat &operator=(const Bureaucrat &other);
	~Bureaucrat();

	const std::string	&getName() const;
	int					getGrade() const;

	void				incrementGrade();
	void				decrementGrade();
	void				signForm(Form &form) const;

	class GradeTooHighException : public std::exception
	{
	  public:
		const char	*what() const throw();
	};

	class GradeTooLowException : public std::exception
	{
	  public:
		const char	*what() const throw();
	};
};

std::ostream &operator<<(std::ostream &out, const Bureaucrat &bureaucrat);

#endif
