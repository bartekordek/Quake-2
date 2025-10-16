#ifndef __shared_files_h__
#define __shared_files_h__

#include "shared/config.h"
#include "shared/cvar.h"

EXTERNC void FS_Link_f(void);
EXTERNC cvar_t* fs_gamedirvar;
EXTERNC int	Developer_searchpath(int who);
EXTERNC char** FS_ListFiles(char* findname, int* numfiles, unsigned musthave, unsigned canthave);
EXTERNC int file_from_pak;

#endif // __shared_files_h__