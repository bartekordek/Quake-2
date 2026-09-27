#pragma once

#include "shared/boolean.h"

inline bool to_bool (qboolean q)
{
	return q == e_true;
}

inline qboolean to_qboolean (bool b)
{
	return b ? e_true : e_false;
}

inline qboolean to_qboolean (float in_float)
{
	return in_float > 0.f ? e_true : e_false;
}