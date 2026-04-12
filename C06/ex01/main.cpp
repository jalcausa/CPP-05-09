#include "Serializer.hpp"
#include "Data.hpp"
#include <iostream>

int main(void)
{
	Data original;
	original.name  = "Ford Prefect";
	original.id    = 42;
	original.score = 9.8;

	std::cout << "=== Serialization Test ===" << std::endl;
	std::cout << "Original pointer : " << &original << std::endl;
	std::cout << "Data content     : name=" << original.name
			  << ", id=" << original.id
			  << ", score=" << original.score << std::endl;

	uintptr_t raw = Serializer::serialize(&original);
	std::cout << std::endl << "Serialized value : " << raw << std::endl;

	Data *result = Serializer::deserialize(raw);
	std::cout << std::endl << "Deserialized ptr : " << result << std::endl;
	std::cout << "Pointers match   : " << (&original == result ? "yes" : "no") << std::endl;
	std::cout << "Data content     : name=" << result->name
			  << ", id=" << result->id
			  << ", score=" << result->score << std::endl;

	return 0;
}
