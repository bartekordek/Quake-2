#pragma once

#include "shared/boolean.h"

inline bool to_bool (qboolean q)
{
	return q == e_true;
}

qboolean to_qboolean (bool b)
{
	return b ? e_true : e_false;
}