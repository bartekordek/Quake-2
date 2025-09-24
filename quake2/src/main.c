#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "shared/boolean.h"
#include "shared/cvar.h"

extern	cvar_t* dedicated;

qboolean	Minimized;

#ifdef _WIN32
#include <windows.h>

#define	MAX_NUM_ARGVS	128
int			argc;
char* argv[MAX_NUM_ARGVS];


HINSTANCE	global_hInstance;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG				msg;
	int				time, oldtime, newtime;
	char* cddir;
	ParseCommandLine(lpCmdLine);

	// if we find the CD, add a +set cddir xxx command line
	cddir = Sys_ScanForCD();
	if (cddir && argc < MAX_NUM_ARGVS - 3)
	{
		int		i;

		// don't override a cddir on the command line
		for (i = 0; i < argc; i++)
			if (!strcmp(argv[i], "cddir"))
				break;
		if (i == argc)
		{
			argv[argc++] = "+set";
			argv[argc++] = "cddir";
			argv[argc++] = cddir;
		}
	}

	Qcommon_Init(argc, argv);
	oldtime = Sys_Milliseconds();

	/* main window message loop */
	while(1)
	{
		// if at a full screen console, don't update unless needed
		if(Minimized || (dedicated && dedicated->value))
		{
			Sleep(1);
		}
	}

}
#endif // _WIN32