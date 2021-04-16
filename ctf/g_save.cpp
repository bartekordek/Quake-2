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

#include "g_local.hpp"

field fields[] = {
    {"classname", FOFS(classname), fieldtype_t::F_LSTRING},
    {"origin", FOFS(s.origin), fieldtype_t::F_VECTOR},
    {"model", FOFS(model), fieldtype_t::F_LSTRING},
    {"spawnflags", FOFS(spawnflags), fieldtype_t::F_INT},
    {"speed", FOFS(speed), fieldtype_t::F_FLOAT},
    {"accel", FOFS(accel), fieldtype_t::F_FLOAT},
    {"decel", FOFS(decel), fieldtype_t::F_FLOAT},
    {"target", FOFS(target), fieldtype_t::F_LSTRING},
    {"targetname", FOFS(targetname), fieldtype_t::F_LSTRING},
    {"pathtarget", FOFS(pathtarget), fieldtype_t::F_LSTRING},
    {"deathtarget", FOFS(deathtarget), fieldtype_t::F_LSTRING},
    {"killtarget", FOFS(killtarget), fieldtype_t::F_LSTRING},
    {"combattarget", FOFS(combattarget), fieldtype_t::F_LSTRING},
    {"message", FOFS(message), fieldtype_t::F_LSTRING},
    {"team", FOFS(team), fieldtype_t::F_LSTRING},
    {"wait", FOFS(wait), fieldtype_t::F_FLOAT},
    {"delay", FOFS(delay), fieldtype_t::F_FLOAT},
    {"random", FOFS(random), fieldtype_t::F_FLOAT},
    {"move_origin", FOFS(move_origin), fieldtype_t::F_VECTOR},
    {"move_angles", FOFS(move_angles), fieldtype_t::F_VECTOR},
    {"style", FOFS(style), fieldtype_t::F_INT},
    {"count", FOFS(count), fieldtype_t::F_INT},
    {"health", FOFS(health), fieldtype_t::F_INT},
    {"sounds", FOFS(sounds), fieldtype_t::F_INT},
    {"light", 0, fieldtype_t::F_IGNORE},
    {"dmg", FOFS(dmg), fieldtype_t::F_INT},
    {"angles", FOFS(s.angles), fieldtype_t::F_VECTOR},
    {"angle", FOFS(s.angles), fieldtype_t::F_ANGLEHACK},
    {"mass", FOFS(mass), fieldtype_t::F_INT},
    {"volume", FOFS(volume), fieldtype_t::F_FLOAT},
    {"attenuation", FOFS(attenuation), fieldtype_t::F_FLOAT},
    {"map", FOFS(map), fieldtype_t::F_LSTRING},

    // temp spawn vars -- only valid when the spawn function is called
    {"lip", STOFS(lip), fieldtype_t::F_INT, FFL_SPAWNTEMP},
    {"distance", STOFS(distance), fieldtype_t::F_INT, FFL_SPAWNTEMP},
    {"height", STOFS(height), fieldtype_t::F_INT, FFL_SPAWNTEMP},
    {"noise", STOFS(noise), fieldtype_t::F_LSTRING, FFL_SPAWNTEMP},
    {"pausetime", STOFS(pausetime), fieldtype_t::F_FLOAT, FFL_SPAWNTEMP},
    {"item", STOFS(item), fieldtype_t::F_LSTRING, FFL_SPAWNTEMP},
    {"gravity", STOFS(gravity), fieldtype_t::F_LSTRING, FFL_SPAWNTEMP},
    {"sky", STOFS(sky), fieldtype_t::F_LSTRING, FFL_SPAWNTEMP},
    {"skyrotate", STOFS(skyrotate), fieldtype_t::F_FLOAT, FFL_SPAWNTEMP},
    {"skyaxis", STOFS(skyaxis), fieldtype_t::F_VECTOR, FFL_SPAWNTEMP},
    {"minyaw", STOFS(minyaw), fieldtype_t::F_FLOAT, FFL_SPAWNTEMP},
    {"maxyaw", STOFS(maxyaw), fieldtype_t::F_FLOAT, FFL_SPAWNTEMP},
    {"minpitch", STOFS(minpitch), fieldtype_t::F_FLOAT, FFL_SPAWNTEMP},
    {"maxpitch", STOFS(maxpitch), fieldtype_t::F_FLOAT, FFL_SPAWNTEMP},
    {"nextmap", STOFS(nextmap), fieldtype_t::F_LSTRING, FFL_SPAWNTEMP}
};

// -------- just for savegames ----------
// all pointer fields should be listed here, or savegames
// won't work properly (they will crash and burn).
// this wasn't just tacked on to the fields array, because
// these don't need names, we wouldn't want map fields using
// some of these, and if one were accidentally present twice
// it would double swizzle (fuck) the pointer.

field        savefields[] =
{
    {"", FOFS(classname), fieldtype_t::F_LSTRING},
    {"", FOFS(target), fieldtype_t::F_LSTRING},
    {"", FOFS(targetname), fieldtype_t::F_LSTRING},
    {"", FOFS(killtarget), fieldtype_t::F_LSTRING},
    {"", FOFS(team), fieldtype_t::F_LSTRING},
    {"", FOFS(pathtarget), fieldtype_t::F_LSTRING},
    {"", FOFS(deathtarget), fieldtype_t::F_LSTRING},
    {"", FOFS(combattarget), fieldtype_t::F_LSTRING},
    {"", FOFS(model), fieldtype_t::F_LSTRING},
    {"", FOFS(map), fieldtype_t::F_LSTRING},
    {"", FOFS(message), fieldtype_t::F_LSTRING},

    {"", FOFS(client), fieldtype_t::F_CLIENT},
    {"", FOFS(item), fieldtype_t::F_ITEM},

    {"", FOFS(goalentity), fieldtype_t::F_EDICT},
    {"", FOFS(movetarget), fieldtype_t::F_EDICT},
    {"", FOFS(enemy), fieldtype_t::F_EDICT},
    {"", FOFS(oldenemy), fieldtype_t::F_EDICT},
    {"", FOFS(activator), fieldtype_t::F_EDICT},
    {"", FOFS(groundentity), fieldtype_t::F_EDICT},
    {"", FOFS(teamchain), fieldtype_t::F_EDICT},
    {"", FOFS(teammaster), fieldtype_t::F_EDICT},
    {"", FOFS(owner), fieldtype_t::F_EDICT},
    {"", FOFS(mynoise), fieldtype_t::F_EDICT},
    {"", FOFS(mynoise2), fieldtype_t::F_EDICT},
    {"", FOFS(target_ent), fieldtype_t::F_EDICT},
    {"", FOFS(chain), fieldtype_t::F_EDICT},

    {NULL, 0, fieldtype_t::F_INT}
};

field        levelfields[] =
{
    {"", LLOFS(changemap), fieldtype_t::F_LSTRING},

    {"", LLOFS(sight_client), fieldtype_t::F_EDICT},
    {"", LLOFS(sight_entity), fieldtype_t::F_EDICT},
    {"", LLOFS(sound_entity), fieldtype_t::F_EDICT},
    {"", LLOFS(sound2_entity), fieldtype_t::F_EDICT},

    {NULL, 0, fieldtype_t::F_INT}
};

field        clientfields[] =
{
    {"", CLOFS(pers.weapon), fieldtype_t::F_ITEM},
    {"", CLOFS(pers.lastweapon), fieldtype_t::F_ITEM},
    {"", CLOFS(newweapon), fieldtype_t::F_ITEM},

    {NULL, 0, fieldtype_t::F_INT}
};

/*
============
InitGame

This will be called when the dll is first loaded, which
only happens when a new game is started or a save game
is loaded.
============
*/
void InitGame (void)
{
    gi.dprintf ("==== InitGame ====\n");

    gun_x = gi.cvar_obj ("gun_x", "0", 0);
    gun_y = gi.cvar_obj ("gun_y", "0", 0);
    gun_z = gi.cvar_obj ("gun_z", "0", 0);

    //FIXME: sv_ prefix is wrong for these
    sv_rollspeed = gi.cvar_obj ("sv_rollspeed", "200", 0);
    sv_rollangle = gi.cvar_obj ("sv_rollangle", "2", 0);
    sv_maxvelocity = gi.cvar_obj ("sv_maxvelocity", "2000", 0);
    sv_gravity = gi.cvar_obj ("sv_gravity", "800", 0);

    // noset vars
    dedicated = gi.cvar_obj ("dedicated", "0", CVAR_NOSET);

    // latched vars
    sv_cheats = gi.cvar_obj ("cheats", "0", CVAR_SERVERINFO|CVAR_LATCH);
    gi.cvar_obj ("gamename", GAMEVERSION , CVAR_SERVERINFO | CVAR_LATCH);
    gi.cvar_obj ("gamedate", __DATE__ , CVAR_SERVERINFO | CVAR_LATCH);

    maxclients = gi.cvar_obj ("maxclients", "4", CVAR_SERVERINFO | CVAR_LATCH);
    deathmatch = gi.cvar_obj ("deathmatch", "0", CVAR_LATCH);
    coop = gi.cvar_obj ("coop", "0", CVAR_LATCH);
    skill = gi.cvar_obj ("skill", "1", CVAR_LATCH);
    maxentities = gi.cvar_obj ("maxentities", "1024", CVAR_LATCH);

//ZOID
//This game.dll only supports deathmatch
    if (!deathmatch->value) {
        gi.dprintf("Forcing deathmatch.");
        gi.cvar_set("deathmatch", "1");
    }
    //force coop off
    if (coop->value)
        gi.cvar_set("coop", "0");
//ZOID


    // change anytime vars
    dmflags = gi.cvar_obj ("dmflags", "0", CVAR_SERVERINFO);
    fraglimit = gi.cvar_obj ("fraglimit", "0", CVAR_SERVERINFO);
    timelimit = gi.cvar_obj ("timelimit", "0", CVAR_SERVERINFO);
//ZOID
    capturelimit = gi.cvar_obj ("capturelimit", "0", CVAR_SERVERINFO);
    instantweap = gi.cvar_obj ("instantweap", "0", CVAR_SERVERINFO);
//ZOID
     password = gi.cvar_obj ("password", "", CVAR_USERINFO);

    g_select_empty = gi.cvar_obj ("g_select_empty", "0", CVAR_ARCHIVE);

    run_pitch = gi.cvar_obj ("run_pitch", "0.002", 0);
    run_roll = gi.cvar_obj ("run_roll", "0.005", 0);
    bob_up  = gi.cvar_obj ("bob_up", "0.005", 0);
    bob_pitch = gi.cvar_obj ("bob_pitch", "0.002", 0);
    bob_roll = gi.cvar_obj ("bob_roll", "0.002", 0);

    // flood control
    flood_msgs = gi.cvar_obj ("flood_msgs", "4", 0);
    flood_persecond = gi.cvar_obj ("flood_persecond", "4", 0);
    flood_waitdelay = gi.cvar_obj ("flood_waitdelay", "10", 0);

    // dm map list
    sv_maplist = gi.cvar_obj ("sv_maplist", "", 0);

    // items
    InitItems ();

    //Com_sprintf (game.helpmessage1, sizeof(game.helpmessage1), "");

    //Com_sprintf (game.helpmessage2, sizeof(game.helpmessage2), "");

    // initialize all entities for this game
    game.maxentities = maxentities->value;
    g_edicts =  gi.TagMalloc (game.maxentities * sizeof(g_edicts[0]), TAG_GAME);
    globals.edicts = g_edicts;
    globals.max_edicts = game.maxentities;

    // initialize all clients for this game
    game.maxclients = maxclients->value;
    game.clients = gi.TagMalloc (game.maxclients * sizeof(game.clients[0]), TAG_GAME);
    globals.num_edicts = game.maxclients+1;

//ZOID
    CTFInit();
//ZOID
}

//=========================================================

void WriteField1 (FILE *f, field *field, byte *base)
{
    void        *p;
    int            len;
    int            index;

    p = (void *)(base + field->ofs);
    switch (field->type)
    {
    case fieldtype_t::F_INT:
    case fieldtype_t::F_FLOAT:
    case fieldtype_t::F_ANGLEHACK:
    case fieldtype_t::F_VECTOR:
    case fieldtype_t::F_IGNORE:
        break;

    case fieldtype_t::F_LSTRING:
    case fieldtype_t::F_GSTRING:
        if ( *(char **)p )
            len = strlen(*(char **)p) + 1;
        else
            len = 0;
        *(int *)p = len;
        break;
    case fieldtype_t::F_EDICT:
        if ( *(edict **)p == NULL)
            index = -1;
        else
            index = *(edict **)p - g_edicts;
        *(int *)p = index;
        break;
    case fieldtype_t::F_CLIENT:
        if ( *(gclient_t **)p == NULL)
            index = -1;
        else
            index = *(gclient_t **)p - game.clients;
        *(int *)p = index;
        break;
    case fieldtype_t::F_ITEM:
        if ( *(edict **)p == NULL)
            index = -1;
        else
            index = *(gitem **)p - itemlist;
        *(int *)p = index;
        break;

    default:
        gi.error ("WriteEdict: unknown field type");
    }
}

void WriteField2 (FILE *f, field *field, byte *base)
{
    int            len;
    void        *p;

    p = (void *)(base + field->ofs);
    switch (field->type)
    {
    case fieldtype_t::F_LSTRING:
    case fieldtype_t::F_GSTRING:
        if ( *(char **)p )
        {
            len = strlen(*(char **)p) + 1;
            fwrite (*(char **)p, len, 1, f);
        }
        break;
    }
}

void ReadField (FILE *f, field *field, byte *base)
{
    void        *p;
    int            len;
    int            index;

    p = (void *)(base + field->ofs);
    switch (field->type)
    {
    case fieldtype_t::F_INT:
    case fieldtype_t::F_FLOAT:
    case fieldtype_t::F_ANGLEHACK:
    case fieldtype_t::F_VECTOR:
    case fieldtype_t::F_IGNORE:
        break;

    case fieldtype_t::F_LSTRING:
        len = *(int *)p;
        if (!len)
            *(char **)p = NULL;
        else
        {
            *(char **)p = gi.TagMalloc (len, TAG_LEVEL);
            fread (*(char **)p, len, 1, f);
        }
        break;
    case fieldtype_t::F_GSTRING:
        len = *(int *)p;
        if (!len)
            *(char **)p = NULL;
        else
        {
            *(char **)p = gi.TagMalloc (len, TAG_GAME);
            fread (*(char **)p, len, 1, f);
        }
        break;
    case fieldtype_t::F_EDICT:
        index = *(int *)p;
        if ( index == -1 )
            *(edict **)p = NULL;
        else
            *(edict **)p = &g_edicts[index];
        break;
    case fieldtype_t::F_CLIENT:
        index = *(int *)p;
        if ( index == -1 )
            *(gclient_t **)p = NULL;
        else
            *(gclient_t **)p = &game.clients[index];
        break;
    case fieldtype_t::F_ITEM:
        index = *(int *)p;
        if ( index == -1 )
            *(gitem **)p = NULL;
        else
            *(gitem **)p = &itemlist[index];
        break;

    default:
        gi.error ("ReadEdict: unknown field type");
    }
}

//=========================================================

/*
==============
WriteClient

All pointer variables (except function pointers) must be handled specially.
==============
*/
void WriteClient (FILE *f, gclient_t *client)
{
    field        *field;
    gclient_t    temp;

    // all of the ints, floats, and vectors stay as they are
    temp = *client;

    // change the pointers to lengths or indexes
    for (field=clientfields ; field->name ; field++)
    {
        WriteField1 (f, field, (byte *)&temp);
    }

    // write the block
    fwrite (&temp, sizeof(temp), 1, f);

    // now write any allocated data following the edict
    for (field=clientfields ; field->name ; field++)
    {
        WriteField2 (f, field, (byte *)client);
    }
}

/*
==============
ReadClient

All pointer variables (except function pointers) must be handled specially.
==============
*/
void ReadClient (FILE *f, gclient_t *client)
{
    field        *field;

    fread (client, sizeof(*client), 1, f);

    for (field=clientfields ; field->name ; field++)
    {
        ReadField (f, field, (byte *)client);
    }
}

/*
============
WriteGame

This will be called whenever the game goes to a new level,
and when the user explicitly saves the game.

Game information include cross level data, like multi level
triggers, help computer info, and all client states.

A single player death will automatically restore from the
last save position.
============
*/
void WriteGame (char *filename, bool autosave)
{
    FILE    *f;
    int        i;
    char    str[16];

    if (!autosave)
        SaveClientData ();

    f = fopen (filename, "wb");
    if (!f)
        gi.error ("Couldn't open %s", filename);

    memset (str, 0, sizeof(str));
    strcpy (str, __DATE__);
    fwrite (str, sizeof(str), 1, f);

    game.autosaved = autosave;
    fwrite (&game, sizeof(game), 1, f);
    game.autosaved = false;

    for (i=0 ; i<game.maxclients ; i++)
        WriteClient (f, &game.clients[i]);

    fclose (f);
}

void ReadGame (char *filename)
{
    FILE    *f;
    int        i;
    char    str[16];

    gi.FreeTags (TAG_GAME);

    f = fopen (filename, "rb");
    if (!f)
        gi.error ("Couldn't open %s", filename);

    fread (str, sizeof(str), 1, f);
    if (strcmp (str, __DATE__))
    {
        fclose (f);
        gi.error ("Savegame from an older version.\n");
    }

    g_edicts =  gi.TagMalloc (game.maxentities * sizeof(g_edicts[0]), TAG_GAME);
    globals.edicts = g_edicts;

    fread (&game, sizeof(game), 1, f);
    game.clients = gi.TagMalloc (game.maxclients * sizeof(game.clients[0]), TAG_GAME);
    for (i=0 ; i<game.maxclients ; i++)
        ReadClient (f, &game.clients[i]);

    fclose (f);
}

//==========================================================


/*
==============
WriteEdict

All pointer variables (except function pointers) must be handled specially.
==============
*/
void WriteEdict (FILE *f, edict *ent)
{
    field        *field;
    edict        temp;

    // all of the ints, floats, and vectors stay as they are
    temp = *ent;

    // change the pointers to lengths or indexes
    for (field=savefields ; field->name ; field++)
    {
        WriteField1 (f, field, (byte *)&temp);
    }

    // write the block
    fwrite (&temp, sizeof(temp), 1, f);

    // now write any allocated data following the edict
    for (field=savefields ; field->name ; field++)
    {
        WriteField2 (f, field, (byte *)ent);
    }

}

/*
==============
WriteLevelLocals

All pointer variables (except function pointers) must be handled specially.
==============
*/
void WriteLevelLocals (FILE *f)
{
    field        *field;
    level_locals_t        temp;

    // all of the ints, floats, and vectors stay as they are
    temp = level;

    // change the pointers to lengths or indexes
    for (field=levelfields ; field->name ; field++)
    {
        WriteField1 (f, field, (byte *)&temp);
    }

    // write the block
    fwrite (&temp, sizeof(temp), 1, f);

    // now write any allocated data following the edict
    for (field=levelfields ; field->name ; field++)
    {
        WriteField2 (f, field, (byte *)&level);
    }
}


/*
==============
ReadEdict

All pointer variables (except function pointers) must be handled specially.
==============
*/
void ReadEdict (FILE *f, edict *ent)
{
    field        *field;

    fread (ent, sizeof(*ent), 1, f);

    for (field=savefields ; field->name ; field++)
    {
        ReadField (f, field, (byte *)ent);
    }
}

/*
==============
ReadLevelLocals

All pointer variables (except function pointers) must be handled specially.
==============
*/
void ReadLevelLocals (FILE *f)
{
    field        *field;

    fread (&level, sizeof(level), 1, f);

    for (field=levelfields ; field->name ; field++)
    {
        ReadField (f, field, (byte *)&level);
    }
}

/*
=================
WriteLevel

=================
*/
void WriteLevel (char *filename)
{
    int        i;
    edict    *ent;
    FILE    *f;
    void    *base;

    f = fopen (filename, "wb");
    if (!f)
        gi.error ("Couldn't open %s", filename);

    // write out edict size for checking
    i = sizeof(edict);
    fwrite (&i, sizeof(i), 1, f);

    // write out a function pointer for checking
    base = (void *)InitGame;
    fwrite (&base, sizeof(base), 1, f);

    // write out level_locals_t
    WriteLevelLocals (f);

    // write out all the entities
    for (i=0 ; i<globals.num_edicts ; i++)
    {
        ent = &g_edicts[i];
        if (!ent->inuse)
            continue;
        fwrite (&i, sizeof(i), 1, f);
        WriteEdict (f, ent);
    }
    i = -1;
    fwrite (&i, sizeof(i), 1, f);

    fclose (f);
}


/*
=================
ReadLevel

SpawnEntities will allready have been called on the
level the same way it was when the level was saved.

That is necessary to get the baselines
set up identically.

The server will have cleared all of the world links before
calling ReadLevel.

No clients are connected yet.
=================
*/
void ReadLevel (char *filename)
{
    int        entnum;
    FILE    *f;
    int        i;
    void    *base;
    edict    *ent;

    f = fopen (filename, "rb");
    if (!f)
        gi.error ("Couldn't open %s", filename);

    // free any dynamic memory allocated by loading the level
    // base state
    gi.FreeTags (TAG_LEVEL);

    // wipe all the entities
    memset (g_edicts, 0, game.maxentities*sizeof(g_edicts[0]));
    globals.num_edicts = maxclients->value+1;

    // check edict size
    fread (&i, sizeof(i), 1, f);
    if (i != sizeof(edict))
    {
        fclose (f);
        gi.error ("ReadLevel: mismatched edict size");
    }

    // check function pointer base address
    fread (&base, sizeof(base), 1, f);
    if (base != (void *)InitGame)
    {
        fclose (f);
        gi.error ("ReadLevel: function pointers have moved");
    }

    // load the level locals
    ReadLevelLocals (f);

    // load all the entities
    while (1)
    {
        if (fread (&entnum, sizeof(entnum), 1, f) != 1)
        {
            fclose (f);
            gi.error ("ReadLevel: failed to read entnum");
        }
        if (entnum == -1)
            break;
        if (entnum >= globals.num_edicts)
            globals.num_edicts = entnum+1;

        ent = &g_edicts[entnum];
        ReadEdict (f, ent);

        // let the server rebuild world links for this ent
        memset (&ent->area, 0, sizeof(ent->area));
        gi.linkentity (ent);
    }

    fclose (f);

    // mark all clients as unconnected
    for (i=0 ; i<maxclients->value ; i++)
    {
        ent = &g_edicts[i+1];
        ent->client = game.clients + i;
        ent->client->pers.connected = false;
    }

    // do any load time things at this point
    for (i=0 ; i<globals.num_edicts ; i++)
    {
        ent = &g_edicts[i];

        if (!ent->inuse)
            continue;

        // fire any cross-level triggers
        if (ent->classname)
            if (strcmp(ent->classname, "target_crosslevel_target") == 0)
                ent->nextthink = level.time + ent->delay;
    }
}

