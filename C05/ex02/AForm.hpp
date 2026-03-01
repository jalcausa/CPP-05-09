#ifndef AFORM_HPP
#define AFORM_HPP

#include <exception>
#include <iostream>
#include <string>

class Bureaucrat;

class AForm
{
  private:
	const std::string	_name;
	bool				_isSigned;
	const int			_gradeToSign;
	const int			_gradeToExecute;

	void				validateGrade(int grade) const;

  protected:
	void				checkExecutable(const Bureaucrat &executor) const;

  public:
	AForm();
	AForm(const std::string &name, int gradeToSign, int gradeToExecute);
	AForm(const AForm &other);
	AForm &operator=(const AForm &other);
	virtual ~AForm();

	const std::string	&getName() const;
	bool				getIsSigned() const;
	int					getGradeToSign() const;
	int					getGradeToExecute() const;

	void				beSigned(const Bureaucrat &bureaucrat);
	virtual void		execute(const Bureaucrat &executor) const = 0;

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

	class FormNotSignedException : public std::exception
	{
	  public:
		const char	*what() const throw();
	};
};

std::ostream &operator<<(std::ostream &out, const AForm &form);

#endif
