#include "shared/log.h"
#include "shared/shared.h"
#include "shared/boolean.h"
#include <stdarg.h>

#ifdef _WIN32
#include <windows.h>

void VID_Printf(int print_level, const char *fmt, ...)
{
	va_list argptr;
	char msg[MAXPRINTMSG];
	static qboolean inupdate;

	va_start(argptr, fmt);
	vsprintf(msg, fmt, argptr);
	va_end(argptr);

	if(print_level == PRINT_ALL)
	{
		Com_Printf("%s", msg);
	}
	else if(print_level == PRINT_DEVELOPER)
	{
		Com_DPrintf("%s", msg);
	}
	else if(print_level == PRINT_ALERT)
	{
		MessageBox(0, msg, "PRINT_ALERT", MB_ICONWARNING);
		OutputDebugString(msg);
	}
}

void VID_Error(int err_level, const char *fmt, ...)
{
	va_list argptr;
	char msg[MAXPRINTMSG];
	static qboolean inupdate;

	va_start(argptr, fmt);
	vsprintf(msg, fmt, argptr);
	va_end(argptr);

	Com_Error(err_level, "%s", msg);
}

#endif // #ifdef _WIN32