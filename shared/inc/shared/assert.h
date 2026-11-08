#ifndef __shared_assert_h__
#define __shared_assert_h__

#include "shared/config.h"

EXTERNC void Q2_Assert (int value, const char *message, ...);

#ifndef Q2_ASSERTS_ENABLED
#if defined(NDEBUG) || defined(Q2_SHIPPING)
#define Q2_ASSERTS_ENABLED 0
#else
#define Q2_ASSERTS_ENABLED 1
#endif
#endif

// Usage: Q2_ASSERT (expr, "format", args...)
// The expression is not evaluated when asserts are disabled, so it must not have side effects.
#if Q2_ASSERTS_ENABLED
#define Q2_ASSERT(expr, ...) \
	do \
	{ \
		Q2_Assert ((expr) ? 1 : 0, __VA_ARGS__); \
	} while (0)
#else
#define Q2_ASSERT(expr, ...) ((void) sizeof ((expr) ? 1 : 0))
#endif


#endif	// __shared_assert_h__