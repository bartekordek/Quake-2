#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>

#include "../linux/glob.h"

#include "../qcommon/qcommon.h"

//===============================================================================

byte *membase;
int maxhunksize;
int curhunksize;

void *Hunk_Begin (int maxsize)
{
    maxhunksize = maxsize + sizeof(int);
    curhunksize = 0;
/*     membase = mmap(0, maxhunksize, PROT_READ|PROT_WRITE,  */
/*         MAP_PRIVATE, -1, 0); */
/*     if ((membase == NULL) || (membase == MAP_FAILED)) */
    membase = malloc(maxhunksize);
    if (membase == NULL)
        Com_Error(ERR_FATAL, "unable to virtual allocate %d bytes", maxsize);

    *((int *)membase) = curhunksize;

    return membase + sizeof(int);
}

void *Hunk_Alloc (int size)
{
    byte *buf;

    // round to cacheline
    size = (size+31)&~31;
    if (curhunksize + size > maxhunksize)
        Com_Error(ERR_FATAL, "Hunk_Alloc overflow");
    buf = membase + sizeof(int) + curhunksize;
    curhunksize += size;
    return buf;
}

int Hunk_End (void)
{
    return curhunksize;
}

void Hunk_Free (void *base)
{
    byte *m;

    if (base) {
        m = ((byte *)base) - sizeof(int);
        free(m);
    }
}

//===============================================================================


/*
================
Sys_Milliseconds
================
*/
int curtime;
int Sys_MillisecondsxD (void)
{
    struct timeval tp;
    struct timezone tzp;
    static int        secbase;

    gettimeofday(&tp, &tzp);

    if (!secbase)
    {
        secbase = tp.tv_sec;
        return tp.tv_usec/1000;
    }

    curtime = (tp.tv_sec - secbase)*1000 + tp.tv_usec/1000;

    return curtime;
}

void Sys_Mkdir (char *path)
{
    mkdir (path, 0777);
}

char *strlwr (char *s)
{
        char *origs = s;
    while (*s) {
        *s = tolower(*s);
        s++;
    }
    return origs;
}

//============================================

static    char    findbase[MAX_OSPATH];
static    char    findpath[MAX_OSPATH];
static    char    findpattern[MAX_OSPATH];
static    DIR        *fdir;

static qboolean CompareAttributes(char *path, char *name,
    unsigned musthave, unsigned canthave )
{
    struct stat st;
    char fn[MAX_OSPATH];

// . and .. never match
    if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
        return false;

    sprintf(fn, "%s/%s", path, name);
    if (stat(fn, &st) == -1)
        return false; // shouldn't happen

    if ( ( st.st_mode & S_IFDIR ) && ( canthave & SFF_SUBDIR ) )
        return false;

    if ( ( musthave & SFF_SUBDIR ) && !( st.st_mode & S_IFDIR ) )
        return false;

    return true;
}

char *Sys_FindFirst (char *path, unsigned musthave, unsigned canhave)
{
    struct dirent *d;
    char *p;

    if (fdir)
        Sys_Error ("Sys_BeginFind without close");

//    COM_FilePath (path, findbase);
    strcpy(findbase, path);

    if ((p = strrchr(findbase, '/')) != NULL) {
        *p = 0;
        strcpy(findpattern, p + 1);
    } else
        strcpy(findpattern, "*");

    if (strcmp(findpattern, "*.*") == 0)
        strcpy(findpattern, "*");

    if ((fdir = opendir(findbase)) == NULL)
        return NULL;
    while ((d = readdir(fdir)) != NULL) {
        if (!*findpattern || glob_match(findpattern, d->d_name)) {
//            if (*findpattern)
//                printf("%s matched %s\n", findpattern, d->d_name);
            if (CompareAttributes(findbase, d->d_name, musthave, canhave)) {
                sprintf (findpath, "%s/%s", findbase, d->d_name);
                return findpath;
            }
        }
    }
    return NULL;
}

char *Sys_FindNext (unsigned musthave, unsigned canhave)
{
    struct dirent *d;

    if (fdir == NULL)
        return NULL;
    while ((d = readdir(fdir)) != NULL) {
        if (!*findpattern || glob_match(findpattern, d->d_name)) {
//            if (*findpattern)
//                printf("%s matched %s\n", findpattern, d->d_name);
            if (CompareAttributes(findbase, d->d_name, musthave, canhave)) {
                sprintf (findpath, "%s/%s", findbase, d->d_name);
                return findpath;
            }
        }
    }
    return NULL;
}

void Sys_FindClose (void)
{
    if (fdir != NULL)
        closedir(fdir);
    fdir = NULL;
}


//============================================

