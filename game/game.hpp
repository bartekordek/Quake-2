#ifndef ____GAME_GAME_H__
#define ____GAME_GAME_H__

/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

// game.h -- game dll information visible to server

#include "shared/edict.hpp"

#define    GAME_API_VERSION    3

// edict->svflags

#define    SVF_NOCLIENT            0x00000001    // don't send entity to clients, even if it has effects
#define    SVF_DEADMONSTER            0x00000002    // treat as CONTENTS_DEADMONSTER for collision
#define    SVF_MONSTER                0x00000004    // treat as CONTENTS_MONSTER for collision

//===============================================================

// functions provided by the main engine
typedef struct
{
    // special messages
    void    (*bprintf) (int printlevel, char *fmt, ...);
    void    (*dprintf) (char *fmt, ...);
    void    (*cprintf) (struct edict_s *ent, int printlevel, char *fmt, ...);
    void    (*centerprintf) (struct edict_s *ent, char *fmt, ...);
    void    (*sound) (struct edict_s *ent, int channel, int soundindex, float volume, float attenuation, float timeofs);
    void    (*positioned_sound) (vec3_t origin, struct edict_s *ent, int channel, int soundinedex, float volume, float attenuation, float timeofs);

    // config strings hold all the index strings, the lightstyles,
    // and misc data like the sky definition and cdtrack.
    // All of the current configstrings are sent to clients when
    // they connect, and changes are sent to all connected clients.
    void    (*configstring) (int num, char *string);

    void    (*error) (char *fmt, ...);

    // the *index functions create configstrings and some internal server state
    int        (*modelindex) (char *name);
    int        (*soundindex) (char *name);
    int        (*imageindex) (char *name);

    void    (*setmodel) (struct edict_s *ent, char *name);

    // collision detection
    trace_t    (*trace) (vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end, struct edict_s *passent, int contentmask);
    int        (*pointcontents) (vec3_t point);
    bool    (*inPVS) (vec3_t p1, vec3_t p2);
    bool    (*inPHS) (vec3_t p1, vec3_t p2);
    void        (*SetAreaPortalState) (int portalnum, bool open);
    bool    (*AreasConnected) (int area1, int area2);

    // an entity will never be sent to a client or used for collision
    // if it is not passed to linkentity.  If the size, position, or
    // solidity changes, it must be relinked.
    void    (*linkentity) (struct edict_s *ent);
    void    (*unlinkentity) (struct edict_s *ent);        // call before removing an interactive edict
    int        (*BoxEdicts) (vec3_t mins, vec3_t maxs, struct edict_s **list,    int maxcount, int areatype);
    void    (*Pmove) (pmove_t *pmove);        // player movement code common with client prediction

    // network messaging
    void    (*multicast) (vec3_t origin, multicast_t to);
    void    (*unicast) (struct edict_s *ent, bool reliable);
    void    (*WriteChar) (int c);
    void    (*WriteByte) (int c);
    void    (*WriteShort) (int c);
    void    (*WriteLong) (int c);
    void    (*WriteFloat) (float f);
    void    (*WriteString) (char *s);
    void    (*WritePosition) (vec3_t pos);    // some fractional bits
    void    (*WriteDir) (vec3_t pos);        // single byte encoded, very coarse
    void    (*WriteAngle) (float f);

    // managed memory allocation
    void    *(*TagMalloc) (int size, int tag);
    void    (*TagFree) (void *block);
    void    (*FreeTags) (int tag);

    // console variable interaction
    cvar_t    *(*cvar) (char *var_name, char *value, int flags);
    cvar_t    *(*cvar_set) (char *var_name, char *value);
    cvar_t    *(*cvar_forceset) (char *var_name, char *value);

    // ClientCommand and ServerCommand parameter access
    int        (*argc) (void);
    char    *(*argv) (int n);
    char    *(*args) (void);    // concatenation of all argv >= 1

    // add commands to the server console as if they were typed in
    // for map changing, etc
    void    (*AddCommandString) (char *text);

    void    (*DebugGraph) (float value, int color);

    bool initialized;
} game_import_t;

//
// functions exported by the game subsystem
//
typedef struct
{
    int            apiversion;

    // the init function will only be called when a game starts,
    // not each time a level is loaded.  Persistant data for clients
    // and the server can be allocated in init
    void        (*Init) (void);
    void        (*Shutdown) (void);

    // each new level entered will cause a call to SpawnEntities
    void        (*SpawnEntities) (char *mapname, char *entstring, char *spawnpoint);

    // Read/Write Game is for storing persistant cross level information
    // about the world state and the clients.
    // WriteGame is called every time a level is exited.
    // ReadGame is called on a loadgame.
    void        (*WriteGame) (char *filename, bool autosave);
    void        (*ReadGame) (char *filename);

    // ReadLevel is called after the default map information has been
    // loaded with SpawnEntities
    void        (*WriteLevel) (char *filename);
    void        (*ReadLevel) (char *filename);

    bool    (*ClientConnect) (struct edict_s *ent, char *userinfo);
    void        (*ClientBegin) (struct edict_s *ent);
    void        (*ClientUserinfoChanged) (struct edict_s *ent, char *userinfo);
    void        (*ClientDisconnect) (struct edict_s *ent);
    void        (*ClientCommand) (struct edict_s *ent);
    void        (*ClientThink) (struct edict_s *ent, usercmd_s *cmd);

    void        (*RunFrame) (void);

    // ServerCommand will be called when an "sv <command>" command is issued on the
    // server console.
    // The game can issue gi.argc() / gi.argv() commands to get the rest
    // of the parameters
    void        (*ServerCommand) (void);

    //
    // global variables shared between game and server
    //

    // The edict array is allocated in the game dll so it
    // can vary in size from one game to another.
    //
    // The size will be fixed when ge->Init() is called
    edict_t* edicts;
    int edict_size;
    int num_edicts;        // current number, <= max_edicts
    int max_edicts;

    bool initialized;
} game_export_t;

game_export_t *GetGameApi (game_import_t *import);

#endif // ____GAME_GAME_H__