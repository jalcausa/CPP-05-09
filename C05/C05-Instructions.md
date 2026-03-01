#C++ - Module 05: Repetition and Exceptions 

**Summary:** This document contains the exercises of Module 05 from the C++ modules.
**Version:** 11.1 

---

#Contents 

| Chapter | Title | Page |
| --- | --- | --- |
| I | Introduction 

 | 2 |
| II | General rules 

 | 3 |
| III | AI Instructions 

 | 6 |
| IV | Exercise 00: Mommy, when I grow up, I want to be a bureaucrat! 

 | 8 |
| V | Exercise 01: Form up, maggots! 

 | 10 |
| VI | Exercise 02: No, you need form 28B, not 28C... 

 | 12 |
| VII | Exercise 03: At least this beats coffee-making 

 | 14 |
| VIII | Submission and Peer Evaluation 

 | 16 |

---

#Chapter I: Introduction 

C++ is a general-purpose programming language created by Bjarne Stroustrup as an extension of the C programming language, or "C with Classes". The goal of these modules is to introduce you to Object-Oriented Programming.

This will be the starting point of your C++ journey. Many languages are recommended for learning OOP. We have chosen C++ since it is derived from your old friend, C. Because this is a complex language, and in order to keep things simple, your code will comply with the C++98 standard.

We are aware that modern C++ is significantly different in many aspects. So, if you want to become a proficient C++ developer, it is up to you to go further after the 42 Common Core!

---

#Chapter II: General rules 

Compiling 

* Compile your code with `c++` and the flags `-Wall -Wextra -Werror`.


* Your code should still compile if you add the flag `-std=c++98`.



Formatting and naming conventions 

* The exercise directories will be named this way: `ex00`, `ex01`, ..., `exn`.


* Name your files, classes, functions, member functions and attributes as required in the guidelines.


* Write class names in **Upper Camel Case** format.


* Files containing class code will always be named according to the class name. For instance: `ClassName.hpp/ClassName.h`, `ClassName.cpp`, or `ClassName.tpp`.


* Every output message must end with a newline character and be displayed to the standard output, unless specified otherwise.


* No coding style is enforced; you can follow your favorite one, but keep code clean and readable for peer evaluators.



Allowed/Forbidden 

* You are allowed to use almost everything from the standard library.


* Use the "C++-ish" versions of C functions as much as possible.


* You cannot use any other external library (C++11, Boost, etc. are forbidden).


* The functions `*printf()`, `*alloc()` and `free()` are strictly forbidden; using them results in a grade of 0.


* The `using namespace <ns_name>` and `friend` keywords are forbidden unless explicitly stated otherwise; failure to comply results in a grade of -42.


* 
**STL Usage:** You are allowed to use the STL only in Modules 08 and 09. This means no Containers (vector/list/map, etc.) and no Algorithms until then. Failure results in a -42 grade.



Design requirements 

* Avoid memory leaks when using the `new` keyword.


* From Module 02 to 09, classes must be designed in **Orthodox Canonical Form**.


* Any function implementation in a header file (except for templates) results in a 0 for the exercise.


* Headers must be independent, include all necessary dependencies, and use include guards to avoid double inclusion.

#Chapter III: Exercise 00: Mommy, when I grow up, I want to be a bureaucrat! 

|  |  |
| --- | --- |
| **Directory** | <br>`ex00/` 

 |
| **Files to Submit** | <br>`Makefile`, `main.cpp`, `Bureaucrat.{h, hpp}`, `Bureaucrat.cpp` 

 |
| **Forbidden** | None 

 |

**Note:** Exception classes do not have to be in Orthodox Canonical Form, but every other class must.

Bureaucrat Class 

* **Attributes:**
* A constant `name`.


* A `grade` ranging from 1 (highest) to 150 (lowest).




* 
**Exceptions:** Throw `Bureaucrat::GradeTooHighException` or `Bureaucrat::GradeTooLowException` if an invalid grade is provided during instantiation or modification.


* **Member Functions:**
* 
`getName()` and `getGrade()` getters.


* Functions to increment or decrement the grade.


* 
*Note:* Incrementing a grade 3 results in grade 2.




* 
**Operator Overload:** Overload the insertion (`<<`) operator to print: `<name>, bureaucrat grade <grade>.`.



---

#Chapter IV: Exercise 01: Form up, maggots! 

|  |  |
| --- | --- |
| **Directory** | <br>`ex01/` 

 |
| **Files to Submit** | Files from ex00 + `Form.{h, hpp}`, `Form.cpp` 

 |
| **Forbidden** | None 

 |

Form Class 

* **Attributes (Private):**
* A constant `name`.


* A boolean for `signed` status (false at construction).


* A constant grade required to sign it.


* A constant grade required to execute it.




* 
**Exceptions:** `Form::GradeTooHighException` and `Form::GradeTooLowException`.


* **Member Functions:**
* Getters for all attributes.


* `beSigned()`: Takes a `Bureaucrat`. Signs the form if the grade is high enough, otherwise throws `Form::GradeTooLowException`.




* 
**Operator Overload:** Overload `<<` to print all form information.



Bureaucrat Update 

* `signForm()`: Calls `Form::beSigned()`. Prints success: `<bureaucrat> signed <form>`. Prints failure: `<bureaucrat> couldn't sign <form> because <reason>.`.



---

#Chapter V: Exercise 02: No, you need form 28B, not 28C... 

|  |  |
| --- | --- |
| **Directory** | <br>`ex02/` 

 |
| **Files to Submit** | Files from ex01 (renaming Form to AForm) + `ShrubberyCreationForm`, `RobotomyRequestForm`, `PresidentialPardonForm` 

 |

AForm (Abstract Class) 

* The base class must be abstract and renamed `AForm`.


* Attributes remain private in the base class.


* Add `execute(Bureaucrat const & executor) const`.



Concrete Classes 

All constructors take only one parameter: the **target** of the form.

1. **ShrubberyCreationForm:** (Sign 145, Exec 137). Creates `<target>_shrubbery` file with ASCII trees.


2. **RobotomyRequestForm:** (Sign 72, Exec 45). Makes drilling noises; succeeds 50% of the time informing `<target>` is robotomized.


3. **PresidentialPardonForm:** (Sign 25, Exec 5). Informs `<target>` has been pardoned by Zaphod Beeblebrox.



Execution 

* Check that the form is signed and the executor's grade is high enough; otherwise throw an exception.


* **Bureaucrat Update:** `executeForm(AForm const & form)`: Attempts execution. Prints success: `<bureaucrat> executed <form>` or an error message on failure.



---

#Chapter VI: Exercise 03: At least this beats coffee-making 

|  |  |
| --- | --- |
| **Directory** | <br>`ex03/` 

 |
| **Files to Submit** | Previous files + `Intern.{h, hpp}`, `Intern.cpp` 

 |

Intern Class 

* Has no name, grade, or characteristics.


* 
**Member Function:** `makeForm(string name, string target)`.


* Returns a pointer to the requested `AForm` object.


* Prints: `Intern creates <form>`.


* Prints an error if the form name doesn't exist.




* 
**Constraint:** You must avoid messy $if/else-if/else$ structures; excessive use will not be accepted.



---

