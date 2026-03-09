# C++ - Module 05: Repetition and Exceptions 

**Summary:** This document contains the exercises of Module 05 from the C++ modules.  
**Version:** 11.1 

---

## Contents 

| Chapter | Title | Page |
| --- | --- | --- |
| I | Introduction | 2 |
| II | General rules | 3 |
| III | AI Instructions | 6 |
| IV | Exercise 00: Mommy, when I grow up, I want to be a bureaucrat! | 8 |
| V | Exercise 01: Form up, maggots! | 10 |
| VI | Exercise 02: No, you need form 28B, not 28C... | 12 |
| VII | Exercise 03: At least this beats coffee-making | 14 |
| VIII | Submission and Peer Evaluation | 16 |

---

## Chapter I: Introduction 

C++ is a general-purpose programming language created by Bjarne Stroustrup...

---

## Chapter II: General rules 

### Compiling 
* Compile your code with `c++` and the flags `-Wall -Wextra -Werror`.
* Your code should still compile if you add the flag `-std=c++98`.

### Formatting and naming conventions 
* The exercise directories will be named this way: `ex00`, `ex01`, ..., `exn`.
* Write class names in **Upper Camel Case** format.
* Files containing class code will always be named according to the class name. For instance: `ClassName.hpp`, `ClassName.cpp`.

> **STL Usage:** You are allowed to use the STL only in Modules 08 and 09. Failure results in a -42 grade.

---

## Chapter III: Exercise 00

| | |
| --- | --- |
| **Directory** | `ex00/` |
| **Files to Submit** | `Makefile`, `main.cpp`, `Bureaucrat.{h, hpp}`, `Bureaucrat.cpp` |
| **Forbidden** | None |

**Note:** Exception classes do not have to be in Orthodox Canonical Form, but every other class must.

### Bureaucrat Class 
* **Attributes:**
    * A constant `name`.
    * A `grade` ranging from 1 (highest) to 150 (lowest).
* **Exceptions:** Throw `Bureaucrat::GradeTooHighException` or `Bureaucrat::GradeTooLowException`.
* **Member Functions:**
    * `getName()` and `getGrade()` getters.
    * Functions to increment or decrement the grade.
* **Operator Overload:** Overload the insertion (`<<`) operator.