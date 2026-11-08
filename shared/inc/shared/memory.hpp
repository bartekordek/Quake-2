#pragma once

#include <cstddef>

template<typename T>
auto Malloc(std::size_t inElementCount)
{
	return static_cast<T *> (malloc (inElementCount * sizeof (T)));
}