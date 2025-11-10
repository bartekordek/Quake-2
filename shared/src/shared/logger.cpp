#include "shared/logger.h"

#include <cstdarg>
#include <cstdint>
#include <cstddef>>
#include <cstring>
#include <cstdio>

#if Q2_WINDOWS
#include <Windows.h>

void log_str (const char *msg...)
{
	va_list args;
	va_start (args, msg);
	constexpr std::size_t bufferSize{1024};
	char				  buffer[bufferSize];
	vsnprintf (buffer, bufferSize, msg, args);
	va_end (args);

	char bufferOut[bufferSize];
	snprintf (bufferOut, bufferSize, "%s\n", buffer);

	OutputDebugString (bufferOut);
}

#endif	// #if Q2_WINDOWS