#include "AForm.hpp"
#include "Bureaucrat.hpp"

AForm::AForm() : _name("default"), _isSigned(false), _gradeToSign(150), _gradeToExecute(150)
{
}

AForm::AForm(const std::string &name, int gradeToSign, int gradeToExecute)
	: _name(name), _isSigned(false), _gradeToSign(gradeToSign), _gradeToExecute(gradeToExecute)
{
	validateGrade(_gradeToSign);
	validateGrade(_gradeToExecute);
}

AForm::AForm(const AForm &other)
	: _name(other._name),
	  _isSigned(other._isSigned),
	  _gradeToSign(other._gradeToSign),
	  _gradeToExecute(other._gradeToExecute)
{
}

AForm &AForm::operator=(const AForm &other)
{
	if (this != &other)
		_isSigned = other._isSigned;
	return (*this);
}

AForm::~AForm()
{
}

const std::string &AForm::getName() const
{
	return (_name);
}

bool AForm::getIsSigned() const
{
	return (_isSigned);
}

int AForm::getGradeToSign() const
{
	return (_gradeToSign);
}

int AForm::getGradeToExecute() const
{
	return (_gradeToExecute);
}

void AForm::beSigned(const Bureaucrat &bureaucrat)
{
	if (bureaucrat.getGrade() > _gradeToSign)
		throw GradeTooLowException();
	_isSigned = true;
}

void AForm::checkExecutable(const Bureaucrat &executor) const
{
	if (!_isSigned)
		throw FormNotSignedException();
	if (executor.getGrade() > _gradeToExecute)
		throw GradeTooLowException();
}

void AForm::validateGrade(int grade) const
{
	if (grade < 1)
		throw GradeTooHighException();
	if (grade > 150)
		throw GradeTooLowException();
}

const char *AForm::GradeTooHighException::what() const throw()
{
	return ("form grade is too high");
}

const char *AForm::GradeTooLowException::what() const throw()
{
	return ("form grade is too low");
}

const char *AForm::FormNotSignedException::what() const throw()
{
	return ("form is not signed");
}

std::ostream &operator<<(std::ostream &out, const AForm &form)
{
	out << form.getName() << ", signed: " << (form.getIsSigned() ? "yes" : "no")
		<< ", grade to sign: " << form.getGradeToSign()
		<< ", grade to execute: " << form.getGradeToExecute() << ".";
	return (out);
}
