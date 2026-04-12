#include "Base.hpp"
#include "A.hpp"
#include "B.hpp"
#include "C.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

// ─────────────────────────────────────────────────────────────────────────────
//  generate: randomly returns a new A, B, or C as a Base pointer
// ─────────────────────────────────────────────────────────────────────────────
Base *generate(void)
{
	int r = rand() % 3;
	if (r == 0)
		return new A();
	if (r == 1)
		return new B();
	return new C();
}

// ─────────────────────────────────────────────────────────────────────────────
//  identify (pointer): dynamic_cast to pointer — returns NULL on failure
// ─────────────────────────────────────────────────────────────────────────────
void identify(Base *p)
{
	if (dynamic_cast<A *>(p))
		std::cout << "A" << std::endl;
	else if (dynamic_cast<B *>(p))
		std::cout << "B" << std::endl;
	else if (dynamic_cast<C *>(p))
		std::cout << "C" << std::endl;
}

// ─────────────────────────────────────────────────────────────────────────────
//  identify (reference): dynamic_cast to reference — throws std::bad_cast on
//  failure (no pointer use allowed inside per the subject)
// ─────────────────────────────────────────────────────────────────────────────
void identify(Base &p)
{
	try
	{
		(void)dynamic_cast<A &>(p);
		std::cout << "A" << std::endl;
		return;
	}
	catch (std::bad_cast &) {}
	try
	{
		(void)dynamic_cast<B &>(p);
		std::cout << "B" << std::endl;
		return;
	}
	catch (std::bad_cast &) {}
	try
	{
		(void)dynamic_cast<C &>(p);
		std::cout << "C" << std::endl;
	}
	catch (std::bad_cast &) {}
}

// ─────────────────────────────────────────────────────────────────────────────
//  main
// ─────────────────────────────────────────────────────────────────────────────
int main(void)
{
	srand(static_cast<unsigned int>(time(NULL)));

	std::cout << "=== Real-Type Identification ===" << std::endl;
	for (int i = 0; i < 6; ++i)
	{
		Base *p = generate();

		std::cout << "Round " << i + 1 << "  |  ";
		std::cout << "pointer: ";
		identify(p);

		std::cout << "         |  reference: ";
		identify(*p);

		delete p;
	}
	return 0;
}
