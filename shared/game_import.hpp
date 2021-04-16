
#pragma once

#include "shared/defines.hpp"


//
// functions provided by the main engine
//
struct game_import
{
	// special messages
    void	(*bprintf) (int printlevel, char *fmt, ...);
    void	(*dprintf) (char *fmt, ...);
    void	(*cprintf) (edict *ent, int printlevel, char *fmt, ...);
    void	(*centerprintf) (edict *ent, char *fmt, ...);
    void	(*sound) (edict *ent, int channel, int soundindex, float volume, float attenuation, float timeofs);
    void	(*positioned_sound) (vec3_t origin, edict *ent, int channel, int soundinedex, float volume, float attenuation, float timeofs);

	// config strings hold all the index strings, the lightstyles,
	// and misc data like the sky definition and cdtrack.
	// All of the current configstrings are sent to clients when
	// they connect, and changes are sent to all connected clients.
    void	(*configstring) (int num, char *string);

    void	(*error) (char *fmt, ...);

	// the *index functions create configstrings and some internal server state
    int		(*modelindex) (char *name);
    int		(*soundindex) (char *name);
    int		(*imageindex) (char *name);

    void	(*setmodel) (edict *ent, char *name);

	// collision detection
    trace_t	(*trace) (vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end, edict *passent, int contentmask);
    int		(*pointcontents) (vec3_t point);
    bool	(*inPVS) (vec3_t p1, vec3_t p2);
    bool	(*inPHS) (vec3_t p1, vec3_t p2);
    void		(*SetAreaPortalState) (int portalnum, bool open);
    bool	(*AreasConnected) (int area1, int area2);

	// an entity will never be sent to a client or used for collision
	// if it is not passed to linkentity.  If the size, position, or
	// solidity changes, it must be relinked.
    void	(*linkentity) (edict *ent);
    void	(*unlinkentity) (edict *ent);		// call before removing an interactive edict
    int		(*BoxEdicts) (vec3_t mins, vec3_t maxs, edict **list,    int maxcount, int areatype);
    void	(*Pmove) (pmove_t *pmove);		// player movement code common with client prediction

	// network messaging
    void	(*multicast) (vec3_t origin, multicast_t to);
    void	(*unicast) (edict *ent, bool reliable);
    void	(*WriteChar) (int c);
    void	(*WriteByte) (int c);
    void	(*WriteShort) (int c);
    void	(*WriteLong) (int c);
    void	(*WriteFloat) (float f);
    void	(*WriteString) (char *s);
    void	(*WritePosition) (vec3_t pos);	// some fractional bits
    void	(*WriteDir) (vec3_t pos);		// single byte encoded, very coarse
    void	(*WriteAngle) (float f);

	// managed memory allocation
    void	*(*TagMalloc) (int size, int tag);
    void	(*TagFree) (void *block);
    void	(*FreeTags) (int tag);

	// console variable interaction
    cvar	*(*cvar) (char *var_name, char *value, int flags);
    cvar	*(*cvar_set) (char *var_name, char *value);
    cvar	*(*cvar_forceset) (char *var_name, char *value);

	// ClientCommand and ServerCommand parameter access
    int		(*argc) (void);
    char	*(*argv) (int n);
    char	*(*args) (void);	// concatenation of all argv >= 1

	// add commands to the server console as if they were typed in
	// for map changing, etc
    void	(*AddCommandString) (char *text);

    void	(*DebugGraph) (float value, int color);
};
