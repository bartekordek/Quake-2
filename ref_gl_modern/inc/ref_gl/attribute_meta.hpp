#pragma once

#include <string>

namespace Q2
{
struct AttributeMeta
{
	std::string name;
	int			index	   = 0;
	int			size	   = 0;
	unsigned	type	   = 0;
	bool		normalized = false;
	int			stride	   = 0;
	void	   *pointer	   = nullptr;

	AttributeMeta (const std::string &inName, int inIndex, int inSize, int inType, bool inNormalized, int inStride, void *inPointer)
		: name (inName), index (inIndex), size (inSize), type (inType), normalized (inNormalized), stride (inStride), pointer (inPointer)
	{
	}
};
}