#ifndef __shared_common_h__
#define __shared_common_h__

#include "shared/config.h"

EXTERNC Q2_EXPORT int Com_ServerState (void);
EXTERNC Q2_EXPORT void Com_SetServerState(int state);

#endif	// __shared_common_h__