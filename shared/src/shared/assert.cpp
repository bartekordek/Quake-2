#include "shared/assert.h"
#include <cassert>
#include <cinttypes>
#include <cstring>
#include <cstdarg>
#include <cstdio>

#define assertm(exp, msg) assert ((void (msg), exp))

void q2_assert (int value, const char *msg...)
{
	if (value > 0)
	{
		return;
	}


	va_list args;
	va_start (args, msg);

	constexpr std::uint32_t bufferSize{512};
	char					buffer[bufferSize];
	snprintf (buffer, bufferSize, msg, args);

	va_end (args);

	assertm (0, buffer);
}