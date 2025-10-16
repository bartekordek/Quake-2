#ifndef __shared_cvar_h__
#define __shared_cvar_h__

#include "shared/boolean.h"

/*
==========================================================

CVARS (console variables)

==========================================================
*/

#ifndef CVAR
#define CVAR

#define CVAR_ARCHIVE	1  // set to cause it to be saved to vars.rc
#define CVAR_USERINFO	2  // added to userinfo  when changed
#define CVAR_SERVERINFO 4  // added to serverinfo when changed
#define CVAR_NOSET		8  // don't allow change from console at all,
// but can be set from the command line
#define CVAR_LATCH 16  // save changes until server restart

// nothing outside the Cvar_*() functions should modify these fields!
typedef struct cvar_s
{
	char		  *name;
	char		  *string;
	char		  *latched_string;	// for CVAR_LATCH vars
	int			   flags;
	qboolean	   modified;  // set each time the cvar is changed
	float		   value;
	struct cvar_s *next;
} cvar_t;

#endif	// CVAR

#endif	// __shared_cvar_h__