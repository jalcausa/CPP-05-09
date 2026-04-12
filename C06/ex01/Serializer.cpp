#include "Serializer.hpp"

// reinterpret_cast converts the pointer bit-pattern to an integer type (uintptr_t)
// guaranteed to be wide enough to hold any pointer on the platform.
uintptr_t Serializer::serialize(Data *ptr)
{
	return reinterpret_cast<uintptr_t>(ptr);
}

// The inverse: take the raw integer and reinterpret its bits as a Data pointer.
Data *Serializer::deserialize(uintptr_t raw)
{
	return reinterpret_cast<Data *>(raw);
}
