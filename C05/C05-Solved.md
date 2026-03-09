# C++ Module 05: Repetition and Exceptions - Solved

## Overview of the Project
Welcome to Module 05! In this module, you are introduced to a powerful mechanism in C++ for handling errors: **Exceptions**. Prior to this, in C or early C++, you probably handled errors by returning specific error codes (like `-1` or `NULL`). Exceptions offer a much cleaner and more robust way to signal that something unexpected happened, separating the error-handling logic from the regular flow of your program.

This module is themed around a bureaucratic dystopia, where strict rules, grades, and forms rule the day. It is an excellent metaphor for strict input validation, where anything out of bounds throws an exception!

Let's go through the exercises one by one to understand what concepts they introduce, how the code is structured, and how we test it.

---

## Exercise 00: Mommy, when I grow up, I want to be a bureaucrat!

### Main Concepts
- **Custom Exceptions:** You will learn how to create your own exception classes by inheriting from `std::exception` and overriding the `what()` method.
- **Try-Catch Blocks:** You will learn how to use `try` to wrap code that might fail and `catch` to handle the resulting exceptions safely without crashing the program.
- **Strict Encapsulation:** Ensuring that an object’s internal state (like a Bureaucrat's grade) can never become invalid.

### Theoretical Explanation
**Why Exceptions?** In traditional C programming, functions often return error codes (e.g., `-1` for failure). This requires the programmer to constantly check return values, blending the "happy path" logic with error handling. Exceptions solve this by instantly pausing the normal execution flow and "jumping" to the nearest `catch` block that knows how to handle the error. This process is called *stack unwinding*.

**std::exception:** This is the base class for all standard C++ exceptions. By inheriting from it, we can create custom exceptions that seamlessly integrate with C++'s built-in exception handling. The key method is `virtual const char* what() const throw()`, which returns a C-string describing the error. The `throw()` specification (common in C++98) indicates that the `what()` method itself promises not to throw any exceptions.

### How the Code is Done and Why
In this exercise, we create the `Bureaucrat` class. A Bureaucrat has a constant `name` and a `grade` (from 1 to 150, where 1 is the highest). 

**The Exception Classes:**
Inside the `Bureaucrat` class declaration, we define two nested classes: `GradeTooHighException` and `GradeTooLowException`. They both inherit publicly from `std::exception`. We override the `what()` method to return a descriptive error message:
```cpp
class GradeTooHighException : public std::exception {
  public:
    const char *what() const throw();
};
```
*Note on `throw()`*: The `throw()` at the end of the method signature is an exception specification from older C++ (C++98) indicating that this method itself does not throw any exceptions.

**Input Validation:**
When constructing a `Bureaucrat` or modifying its grade (`incrementGrade` or `decrementGrade`), we check if the grade goes out of bounds. If it does, we literally "throw" our custom exception instead of letting the program continue with an invalid state:
```cpp
void Bureaucrat::validateGrade(int grade) const {
    if (grade < 1) throw GradeTooHighException();
    if (grade > 150) throw GradeTooLowException();
}
```

### The Tests (`main.cpp`)
The `main.cpp` file uses `try` and `catch` blocks to test three scenarios:
1. **Valid Creation & Modification:** Bob is created with grade 2. He is incremented to grade 1. Attempting to increment him again throws a `GradeTooHighException`.
2. **Invalid High Grade:** We attempt to create a bureaucrat with grade 0. A `GradeTooHighException` is caught immediately during construction.
3. **Invalid Low Grade:** We attempt to create a bureaucrat with grade 151. A `GradeTooLowException` is caught.
Using try-catch ensures that even though an error occurs, the program handles it gracefully and finishes normally.

---

## Exercise 01: Form up, maggots!

### Main Concepts
- **Inter-class dependencies:** The `Form` and `Bureaucrat` classes interact with each other and cross-reference.
- **Passing the buck:** Throwing exceptions based on the interaction between two objects (a Bureaucrat trying to sign a Form but lacking the required grade).

### Theoretical Explanation
**Forward Declarations vs. Includes:** When two classes interact, they need to know about each other. However, if `Bureaucrat.hpp` includes `Form.hpp` and `Form.hpp` includes `Bureaucrat.hpp`, it creates a circular dependency, leading to compilation errors. The theoretical solution is the *forward declaration* (e.g., `class Bureaucrat;` inside `Form.hpp`), which tells the compiler "this class exists, trust me." You only include the full header in the `.cpp` file where the class's methods are actually used.

**Exception Propagation:** An exception doesn't have to be caught in the function where it is thrown. If `Form::beSigned()` throws an exception because the Bureaucrat's grade is too low, it *propagates* up the call stack. The `Bureaucrat::signForm()` method catches this exact exception and handles it, demonstrating how exceptions gracefully pass error states across different interacting objects.

### How the Code is Done and Why
We introduce the `Form` class. A form has a name, a boolean indicating if it's signed, a grade required to sign it, and a grade required to execute it.

Just like the Bureaucrat, the `Form` has its own `GradeTooHighException` and `GradeTooLowException` for when it is constructed with invalid requirements.

The core logic lies in the `beSigned` method of the `Form`:
```cpp
void Form::beSigned(const Bureaucrat &bureaucrat) {
    if (bureaucrat.getGrade() > _gradeToSign)
        throw GradeTooLowException();
    _isSigned = true;
}
```
Notice how `beSigned` throws an exception if the Bureaucrat's grade is numerically higher (which means a lower rank) than the required grade.

On the `Bureaucrat` side, we add a `signForm` method. This method wraps the call to `form.beSigned(*this)` in a try-catch block. If it succeeds, it prints a success message; if it catches the exception thrown by the form, it prints a failure message explaining why.

### The Tests (`main.cpp`)
1. **Successful Signing:** A Bureaucrat (grade 42) attempts to sign a TaxForm (required grade 50). The signing succeeds.
2. **Failed Signing:** An Intern (grade 150) attempts to sign a TopSecret form (required grade 1). The `beSigned` method throws an exception, which the `Bureaucrat::signForm` method catches, outputting a clear error message.

---

## Exercise 02: No, you need form 28B, not 28C...

### Main Concepts
- **Abstract Classes (Polymorphism):** We rename `Form` to `AForm` (Abstract Form) and make it an abstract class by adding a **pure virtual function**.
- **File I/O:** Using `std::ofstream` to write to files.
- **Randomization:** Generating pseudo-random behavior in C++.

### Theoretical Explanation
**Abstract Classes and Pure Virtual Functions:** In OOP, sometimes a base class represents a generic concept that shouldn't be instantiated on its own—like a generic "Form." We enforce this in C++ using a *pure virtual function* (syntax: `virtual void myFunc() = 0;`). A class containing at least one pure virtual function becomes an *abstract class*. It forces derived classes (like `RobotomyRequestForm`) to provide their own specific implementation of that function, ensuring polymorphism.

**Polymorphism in Action:** By passing an `AForm` reference to `Bureaucrat::executeForm()`, the Bureaucrat doesn't need to know *which* specific form it is executing. At runtime, C++ uses dynamic dispatch (via the vtable) to call the correct derived `execute()` method.

### How the Code is Done and Why
We turn `AForm` into an abstract class by adding `= 0` to its `execute` method:
```cpp
virtual void execute(const Bureaucrat &executor) const = 0;
```
This means you can no longer instantiate an `AForm` directly. You can only instantiate classes that inherit from it and implement the `execute` method.

We create three concrete forms:
1. **ShrubberyCreationForm:** Takes a target. When executed, it uses `std::ofstream` to create a file named `<target>_shrubbery` and writes ASCII art trees into it.
2. **RobotomyRequestForm:** Takes a target. Uses `std::rand()` (seeded with `std::time()`) to create a 50/50 chance of successfully robotomizing the target or failing.
3. **PresidentialPardonForm:** Takes a target. Prints that the target has been pardoned.

The base `AForm` class also gets a protected method `checkExecutable` which verifies if the form is signed and if the executor's grade is high enough before the concrete form can actually do its job.

### The Tests (`main.cpp`)
The tests instantiate different bureaucrats (Boss, Worker) and the three different forms.
- The Worker (grade 140) can sign the Shrubbery form (requires 145) but cannot execute it (requires 137). The `executeForm` method catches this and displays an error.
- The Boss (grade 1) steps in to execute the Shrubbery form, which succeeds and generates the file.
- The Boss proceeds to sign and execute the Robotomy and Presidential Pardon forms successfully.

---

## Exercise 03: At least this beats coffee-making

### Main Concepts
- **Pointers to Member Functions:** A way to dynamically call different methods of a class without using large and messy `if/else if/else` blocks.
- **The Factory Pattern:** Creating objects dynamically based on a string identifier.

### Theoretical Explanation
**Pointers to Member Functions:** Regular function pointers point directly to a memory address of executable code. However, member functions of a class implicitly require a `this` pointer to know *which* object instance they are operating on. Therefore, pointers to member functions have a special syntax (`ReturnType (ClassName::*pointerName)(Args)`) and must be invoked alongside a specific object instance using the `.*` or `->*` operators (e.g., `(this->*pointer)(args)`).

**The Factory Pattern Concept:** This exercise serves as an introduction to the Factory Design Pattern. Instead of using `new RobotomyRequestForm()` all over your codebase, you centralize the creation logic inside an `Intern` (the Factory). This decouples the code that *uses* forms from the code that *creates* forms, making the system easier to scale and maintain.

### How the Code is Done and Why
The `Intern` class has one job: to make forms. The `makeForm` function takes a form name and a target, and returns a pointer to a newly allocated `AForm`.

The exercise strictly forbids messy if/else chains. To solve this elegantly, we use an array of strings representing the form names, and an array of **pointers to member functions** representing the creation logic:
```cpp
typedef AForm *(Intern::*FormCreator)(const std::string &target) const;
```
Inside `makeForm`:
```cpp
const std::string formNames[3] = {"shrubbery creation", "robotomy request", "presidential pardon"};
FormCreator formCreators[3] = {&Intern::createShrubberyForm, &Intern::createRobotomyForm, &Intern::createPresidentialForm};

for (int i = 0; i < 3; ++i) {
    if (name == formNames[i]) {
        std::cout << "Intern creates " << name << std::endl;
        return ((this->*formCreators[i])(target)); // Calls the specific creation method
    }
}
```
If the string matches, it dynamically calls the corresponding creator method (e.g., `createShrubberyForm`), which returns a `new` instance of that specific form. If no match is found, it prints an error and returns `NULL`.

### The Tests (`main.cpp`)
An Intern is tasked with creating forms dynamically using only strings:
1. It successfully creates a "robotomy request". The Boss signs and executes it. Since it was dynamically allocated (`new`), we must `delete` it afterwards to prevent memory leaks.
2. It successfully creates a "presidential pardon". The Boss processes it, and it's deleted.
3. It tries to create an "unknown form". The Intern gracefully reports it cannot create it, and returns `NULL`, preventing any invalid operations.

---
*Happy coding, and don't forget to get your C++-98 compliance form signed in triplicate!*