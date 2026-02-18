#include "shared/assert.h"
#include <cassert>
#include <cinttypes>
#include <cstring>
#include <cstdarg>
#include <cstdio>

#if Q2_WINDOWS 
#include <windows.h>
#endif // Q2_WINDOWS

#define assertm(exp, msg) assert ((void (msg), exp))

void Q2_Assert (int value, const char *msg...)
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

#if Q2_WINDOWS
	OutputDebugStringA (buffer);
#endif	// #if Q2_WINDOWS

	assertm (0, buffer);
}