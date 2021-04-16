#ifndef ____CTF_GAME_H__
#define ____CTF_GAME_H__

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

#define	GAME_API_VERSION	3

// edict->svflags

#define	SVF_NOCLIENT			0x00000001	// don't send entity to clients, even if it has effects
#define	SVF_DEADMONSTER			0x00000002	// treat as CONTENTS_DEADMONSTER for collision
#define	SVF_MONSTER				0x00000004	// treat as CONTENTS_MONSTER for collision
//ZOID
#define SVF_PROJECTILE			0x00000008  // entity is simple projectile, used for network optimization
// if an entity is projectile, the model index/x/y/z/pitch/yaw are sent, encoded into
// seven (or eight) bytes.  This is to speed up projectiles.  Currently, only the
// hyperblaster makes use of this.  use for items that are moving with a constant
// velocity that don't change direction or model
//ZOID

// edict->solid values

typedef enum
{
SOLID_NOT,			// no interaction with other objects
SOLID_TRIGGER,		// only touch when inside, after moving
SOLID_BBOX,			// touch on edge
SOLID_BSP			// bsp clip, touch on edge
} solid_t;

//===============================================================

// link_t is only used for entity area links now
typedef struct link_s
{
	struct link_s	*prev, *next;
} link_t;

#define	MAX_ENT_CLUSTERS	16


typedef edict edict;
typedef struct gclient gclient_t;


#ifndef GAME_INCLUDE

struct gclient
{
	player_state	ps;		// communicated by server to clients
	int				ping;
	// the game dll can add anything it wants after
	// this point in the structure
};


edict
{
	entity_state	s;
	struct gclient	*client;
	bool	inuse;
	int			linkcount;

	// FIXME: move these fields to a server private sv_entity_t
	link_t		area;				// linked to a division node or leaf

	int			num_clusters;		// if -1, use headnode instead
	int			clusternums[MAX_ENT_CLUSTERS];
	int			headnode;			// unused if num_clusters != -1
	int			areanum, areanum2;

	//================================

	int			svflags;			// SVF_NOCLIENT, SVF_DEADMONSTER, SVF_MONSTER, etc
	vec3_t		mins, maxs;
	vec3_t		absmin, absmax, size;
	solid_t		solid;
	int			clipmask;
	edict		*owner;

	// the game dll can add anything it wants after
	// this point in the structure
};

#endif		// GAME_INCLUDE

//===============================================================

//
// functions exported by the game subsystem
//
typedef struct
{
	int			apiversion;

	// the init function will only be called when a game starts,
	// not each time a level is loaded.  Persistant data for clients
	// and the server can be allocated in init
	void		(*Init) (void);
	void		(*Shutdown) (void);

	// each new level entered will cause a call to SpawnEntities
	void		(*SpawnEntities) (char *mapname, char *entstring, char *spawnpoint);

	// Read/Write Game is for storing persistant cross level information
	// about the world state and the clients.
	// WriteGame is called every time a level is exited.
	// ReadGame is called on a loadgame.
	void		(*WriteGame) (char *filename, bool autosave);
	void		(*ReadGame) (char *filename);

	// ReadLevel is called after the default map information has been
	// loaded with SpawnEntities
	void		(*WriteLevel) (char *filename);
	void		(*ReadLevel) (char *filename);

	bool	(*ClientConnect) (edict *ent, char *userinfo);
	void		(*ClientBegin) (edict *ent);
	void		(*ClientUserinfoChanged) (edict *ent, char *userinfo);
	void		(*ClientDisconnect) (edict *ent);
	void		(*ClientCommand) (edict *ent);
	void		(*ClientThink) (edict *ent, usercmd_t *cmd);

	void		(*RunFrame) (void);

	// ServerCommand will be called when an "sv <command>" command is issued on the
	// server console.
	// The game can issue gi.argc() / gi.argv() commands to get the rest
	// of the parameters
	void		(*ServerCommand) (void);

	//
	// global variables shared between game and server
	//

	// The edict array is allocated in the game dll so it
	// can vary in size from one game to another.
	//
	// The size will be fixed when ge->Init() is called
	edict	*edicts;
	int			edict_size;
	int			num_edicts;		// current number, <= max_edicts
	int			max_edicts;
} game_export_t;

game_export_t *GetGameApi (game_import *import);

#endif // ____CTF_GAME_H__