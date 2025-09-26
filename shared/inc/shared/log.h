#ifndef __shared_log_h__
#define __shared_log_h__

#include "shared/config.h"

EXTERNC void VID_Printf(int print_level, const char *fmt, ...);
EXTERNC void VID_Error(int err_level, const char *fmt, ...);

#endif	// __shared_log_h__