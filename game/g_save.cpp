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
#include "shared/g_client.hpp"

#define Function(f) {#f, f}

mmove_t mmove_reloc;

field fields[] = {
    {"classname", FOFS(classname), fieldtype_t::F_LSTRING},
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
    {"mass", FOFS(mass), fieldtype_t::F_INT},
    {"volume", FOFS(volume), fieldtype_t::F_FLOAT},
    {"attenuation", FOFS(attenuation), fieldtype_t::F_FLOAT},
    {"map", FOFS(map), fieldtype_t::F_LSTRING},
    {"origin", FOFS(s.origin), fieldtype_t::F_VECTOR},
    {"angles", FOFS(s.angles), fieldtype_t::F_VECTOR},
    {"angle", FOFS(s.angles), fieldtype_t::F_ANGLEHACK},

    {"goalentity", FOFS(goalentity), fieldtype_t::F_EDICT, FFL_NOSPAWN},
    {"movetarget", FOFS(movetarget), fieldtype_t::F_EDICT, FFL_NOSPAWN},
    {"enemy", FOFS(enemy), fieldtype_t::F_EDICT, FFL_NOSPAWN},
    {"oldenemy", FOFS(oldenemy), fieldtype_t::F_EDICT, FFL_NOSPAWN},
    {"activator", FOFS(activator), fieldtype_t::F_EDICT, FFL_NOSPAWN},
    {"groundentity", FOFS(groundentity), fieldtype_t::F_EDICT, FFL_NOSPAWN},
    {"teamchain", FOFS(teamchain), fieldtype_t::F_EDICT, FFL_NOSPAWN},
    {"teammaster", FOFS(teammaster), fieldtype_t::F_EDICT, FFL_NOSPAWN},
    {"owner", FOFS(owner), fieldtype_t::F_EDICT, FFL_NOSPAWN},
    {"mynoise", FOFS(mynoise), fieldtype_t::F_EDICT, FFL_NOSPAWN},
    {"mynoise2", FOFS(mynoise2), fieldtype_t::F_EDICT, FFL_NOSPAWN},
    {"target_ent", FOFS(target_ent), fieldtype_t::F_EDICT, FFL_NOSPAWN},
    {"chain", FOFS(chain), fieldtype_t::F_EDICT, FFL_NOSPAWN},

    {"prethink", FOFS(prethink), fieldtype_t::F_FUNCTION, FFL_NOSPAWN},
    {"think", FOFS(think), fieldtype_t::F_FUNCTION, FFL_NOSPAWN},
    {"blocked", FOFS(blocked), fieldtype_t::F_FUNCTION, FFL_NOSPAWN},
    {"touch", FOFS(touch), fieldtype_t::F_FUNCTION, FFL_NOSPAWN},
    {"use", FOFS(use), fieldtype_t::F_FUNCTION, FFL_NOSPAWN},
    {"pain", FOFS(pain), fieldtype_t::F_FUNCTION, FFL_NOSPAWN},
    {"die", FOFS(die), fieldtype_t::F_FUNCTION, FFL_NOSPAWN},

    {"stand", FOFS(monsterinfo.stand), fieldtype_t::F_FUNCTION, FFL_NOSPAWN},
    {"idle", FOFS(monsterinfo.idle), fieldtype_t::F_FUNCTION, FFL_NOSPAWN},
    {"search", FOFS(monsterinfo.search), fieldtype_t::F_FUNCTION, FFL_NOSPAWN},
    {"walk", FOFS(monsterinfo.walk), fieldtype_t::F_FUNCTION, FFL_NOSPAWN},
    {"run", FOFS(monsterinfo.run), fieldtype_t::F_FUNCTION, FFL_NOSPAWN},
    {"dodge", FOFS(monsterinfo.dodge), fieldtype_t::F_FUNCTION, FFL_NOSPAWN},
    {"attack", FOFS(monsterinfo.attack), fieldtype_t::F_FUNCTION, FFL_NOSPAWN},
    {"melee", FOFS(monsterinfo.melee), fieldtype_t::F_FUNCTION, FFL_NOSPAWN},
    {"sight", FOFS(monsterinfo.sight), fieldtype_t::F_FUNCTION, FFL_NOSPAWN},
    {"checkattack", FOFS(monsterinfo.checkattack), fieldtype_t::F_FUNCTION, FFL_NOSPAWN},
    {"currentmove", FOFS(monsterinfo.currentmove), fieldtype_t::F_MMOVE, FFL_NOSPAWN},

    {"endfunc", FOFS(moveinfo.endfunc), fieldtype_t::F_FUNCTION, FFL_NOSPAWN},

    // temp spawn vars -- only valid when the spawn function is called
    {"lip", STOFS(lip), fieldtype_t::F_INT, FFL_SPAWNTEMP},
    {"distance", STOFS(distance), fieldtype_t::F_INT, FFL_SPAWNTEMP},
    {"height", STOFS(height), fieldtype_t::F_INT, FFL_SPAWNTEMP},
    {"noise", STOFS(noise), fieldtype_t::F_LSTRING, FFL_SPAWNTEMP},
    {"pausetime", STOFS(pausetime), fieldtype_t::F_FLOAT, FFL_SPAWNTEMP},
    {"item", STOFS(item), fieldtype_t::F_LSTRING, FFL_SPAWNTEMP},

//need for item field in edict struct, FFL_SPAWNTEMP item will be skipped on saves
    {"item", FOFS(item), fieldtype_t::F_ITEM},

    {"gravity", STOFS(gravity), fieldtype_t::F_LSTRING, FFL_SPAWNTEMP},
    {"sky", STOFS(sky), fieldtype_t::F_LSTRING, FFL_SPAWNTEMP},
    {"skyrotate", STOFS(skyrotate), fieldtype_t::F_FLOAT, FFL_SPAWNTEMP},
    {"skyaxis", STOFS(skyaxis), fieldtype_t::F_VECTOR, FFL_SPAWNTEMP},
    {"minyaw", STOFS(minyaw), fieldtype_t::F_FLOAT, FFL_SPAWNTEMP},
    {"maxyaw", STOFS(maxyaw), fieldtype_t::F_FLOAT, FFL_SPAWNTEMP},
    {"minpitch", STOFS(minpitch), fieldtype_t::F_FLOAT, FFL_SPAWNTEMP},
    {"maxpitch", STOFS(maxpitch), fieldtype_t::F_FLOAT, FFL_SPAWNTEMP},
    {"nextmap", STOFS(nextmap), fieldtype_t::F_LSTRING, FFL_SPAWNTEMP},

    {0, 0, fieldtype_t::F_INT, 0}

};

field        levelfields[] =
{
    {"changemap", LLOFS(changemap), fieldtype_t::F_LSTRING},

    {"sight_client", LLOFS(sight_client), fieldtype_t::F_EDICT},
    {"sight_entity", LLOFS(sight_entity), fieldtype_t::F_EDICT},
    {"sound_entity", LLOFS(sound_entity), fieldtype_t::F_EDICT},
    {"sound2_entity", LLOFS(sound2_entity), fieldtype_t::F_EDICT},

    {NULL, 0, fieldtype_t::F_INT}
};

field        clientfields[] =
{
    {"pers.weapon", CLOFS(pers.weapon), fieldtype_t::F_ITEM},
    {"pers.lastweapon", CLOFS(pers.lastweapon), fieldtype_t::F_ITEM},
    {"newweapon", CLOFS(newweapon), fieldtype_t::F_ITEM},

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
    quake2::getInstance()->gi.dprintf ("==== InitGame ====\n");

    gun_x = quake2::getInstance()->gi.cvar_obj ("gun_x", "0", 0);
    gun_y = quake2::getInstance()->gi.cvar_obj ("gun_y", "0", 0);
    gun_z = quake2::getInstance()->gi.cvar_obj ("gun_z", "0", 0);

    //FIXME: sv_ prefix is wrong for these
    sv_rollspeed = quake2::getInstance()->gi.cvar_obj ("sv_rollspeed", "200", 0);
    sv_rollangle = quake2::getInstance()->gi.cvar_obj ("sv_rollangle", "2", 0);
    sv_maxvelocity = quake2::getInstance()->gi.cvar_obj ("sv_maxvelocity", "2000", 0);
    sv_gravity = quake2::getInstance()->gi.cvar_obj ("sv_gravity", "800", 0);

    // noset vars
    quake2::getInstance()->dedicated = quake2::getInstance()->gi.cvar_obj ("quake2::getInstance()->dedicated", "0", CVAR_NOSET);

    // latched vars
    sv_cheats = quake2::getInstance()->gi.cvar_obj ("cheats", "0", CVAR_SERVERINFO|CVAR_LATCH);
    quake2::getInstance()->gi.cvar_obj ("gamename", GAMEVERSION , CVAR_SERVERINFO | CVAR_LATCH);
    quake2::getInstance()->gi.cvar_obj ("gamedate", __DATE__ , CVAR_SERVERINFO | CVAR_LATCH);

    maxclients = quake2::getInstance()->gi.cvar_obj ("maxclients", "4", CVAR_SERVERINFO | CVAR_LATCH);
    maxspectators = quake2::getInstance()->gi.cvar_obj ("maxspectators", "4", CVAR_SERVERINFO);
    deathmatch = quake2::getInstance()->gi.cvar_obj ("deathmatch", "0", CVAR_LATCH);
    coop = quake2::getInstance()->gi.cvar_obj ("coop", "0", CVAR_LATCH);
    skill = quake2::getInstance()->gi.cvar_obj ("skill", "1", CVAR_LATCH);
    maxentities = quake2::getInstance()->gi.cvar_obj ("maxentities", "1024", CVAR_LATCH);

    // change anytime vars
    dmflags = quake2::getInstance()->gi.cvar_obj ("dmflags", "0", CVAR_SERVERINFO);
    fraglimit = quake2::getInstance()->gi.cvar_obj ("fraglimit", "0", CVAR_SERVERINFO);
    timelimit = quake2::getInstance()->gi.cvar_obj ("timelimit", "0", CVAR_SERVERINFO);
    password = quake2::getInstance()->gi.cvar_obj ("password", "", CVAR_USERINFO);
    spectator_password = quake2::getInstance()->gi.cvar_obj ("spectator_password", "", CVAR_USERINFO);
    filterban = quake2::getInstance()->gi.cvar_obj ("filterban", "1", 0);

    g_select_empty = quake2::getInstance()->gi.cvar_obj ("g_select_empty", "0", CVAR_ARCHIVE);

    run_pitch = quake2::getInstance()->gi.cvar_obj ("run_pitch", "0.002", 0);
    run_roll = quake2::getInstance()->gi.cvar_obj ("run_roll", "0.005", 0);
    bob_up  = quake2::getInstance()->gi.cvar_obj ("bob_up", "0.005", 0);
    bob_pitch = quake2::getInstance()->gi.cvar_obj ("bob_pitch", "0.002", 0);
    bob_roll = quake2::getInstance()->gi.cvar_obj ("bob_roll", "0.002", 0);

    // flood control
    flood_msgs = quake2::getInstance()->gi.cvar_obj ("flood_msgs", "4", 0);
    flood_persecond = quake2::getInstance()->gi.cvar_obj ("flood_persecond", "4", 0);
    flood_waitdelay = quake2::getInstance()->gi.cvar_obj ("flood_waitdelay", "10", 0);

    // dm map list
    sv_maplist = quake2::getInstance()->gi.cvar_obj ("sv_maplist", "", 0);

    // items
    InitItems ();

    //Com_sprintf (game.helpmessage1, sizeof(game.helpmessage1), "");

    //Com_sprintf (game.helpmessage2, sizeof(game.helpmessage2), "");

    // initialize all entities for this game
    game.maxentities = maxentities->value;
    g_edicts =  (edict*)quake2::getInstance()->gi.TagMalloc (game.maxentities * sizeof(g_edicts[0]), TAG_GAME);
    globals.edicts = g_edicts;
    globals.max_edicts = game.maxentities;

    // initialize all clients for this game
    game.maxclients = maxclients->value;
    game.clients = (gclient*)quake2::getInstance()->gi.TagMalloc (game.maxclients * sizeof(game.clients[0]), TAG_GAME);
    globals.num_edicts = game.maxclients+1;
}

//=========================================================

void WriteField1 (FILE *f, field *field, byte *base)
{
    void        *p;
    int            len;
    int            index;

    if (field->flags & FFL_SPAWNTEMP)
        return;

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

    //relative to code segment
    case fieldtype_t::F_FUNCTION:
        if (*(byte **)p == NULL)
            index = 0;
        else
            index = *(byte **)p - ((byte *)InitGame);
        *(int *)p = index;
        break;

    //relative to data segment
    case fieldtype_t::F_MMOVE:
        if (*(byte **)p == NULL)
            index = 0;
        else
            index = *(byte **)p - (byte *)&mmove_reloc;
        *(int *)p = index;
        break;

    default:
        quake2::getInstance()->gi.error ("WriteEdict: unknown field type");
    }
}


void WriteField2 (FILE *f, field *field, byte *base)
{
    int            len;
    void        *p;

    if (field->flags & FFL_SPAWNTEMP)
        return;

    p = (void *)(base + field->ofs);
    switch (field->type)
    {
    case fieldtype_t::F_LSTRING:
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

    if (field->flags & FFL_SPAWNTEMP)
        return;

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
            *(char **)p = (char *)quake2::getInstance()->gi.TagMalloc (len, TAG_LEVEL);
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

    //relative to code segment
    case fieldtype_t::F_FUNCTION:
        index = *(int *)p;
        if ( index == 0 )
            *(byte **)p = NULL;
        else
            *(byte **)p = ((byte *)InitGame) + index;
        break;

    //relative to data segment
    case fieldtype_t::F_MMOVE:
        index = *(int *)p;
        if (index == 0)
            *(byte **)p = NULL;
        else
            *(byte **)p = (byte *)&mmove_reloc + index;
        break;

    default:
        quake2::getInstance()->gi.error ("ReadEdict: unknown field type");
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
    for (field=clientfields ; !field->name.empty() ; ++field)
    {
        WriteField1 (f, field, (byte *)&temp);
    }

    // write the block
    fwrite (&temp, sizeof(temp), 1, f);

    // now write any allocated data following the edict
    for (field=clientfields ; !field->name.empty() ; ++field)
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

    for (field=clientfields ; !field->name.empty(); ++field)
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
        quake2::getInstance()->gi.error ("Couldn't open %s", filename);

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

    quake2::getInstance()->gi.FreeTags (TAG_GAME);

    f = fopen (filename, "rb");
    if (!f)
        quake2::getInstance()->gi.error ("Couldn't open %s", filename);

    fread (str, sizeof(str), 1, f);
    if (strcmp (str, __DATE__))
    {
        fclose (f);
        quake2::getInstance()->gi.error ("Savegame from an older version.\n");
    }

    g_edicts =  (edict*)quake2::getInstance()->gi.TagMalloc (game.maxentities * sizeof(g_edicts[0]), TAG_GAME);
    globals.edicts = g_edicts;

    fread (&game, sizeof(game), 1, f);
    game.clients = (gclient_t*) quake2::getInstance()->gi.TagMalloc (game.maxclients * sizeof(game.clients[0]), TAG_GAME);
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
    for (field=fields ; !field->name.empty() ; field++)
    {
        WriteField1 (f, field, (byte *)&temp);
    }

    // write the block
    fwrite (&temp, sizeof(temp), 1, f);

    // now write any allocated data following the edict
    for (field=fields ; !field->name.empty() ; field++)
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
    for (field=levelfields ; !field->name.empty() ; field++)
    {
        WriteField1 (f, field, (byte *)&temp);
    }

    // write the block
    fwrite (&temp, sizeof(temp), 1, f);

    // now write any allocated data following the edict
    for (field=levelfields ; !field->name.empty() ; field++)
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

    for (field=fields ; !field->name.empty() ; field++)
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

    for (field=levelfields ; !field->name.empty() ; field++)
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
        quake2::getInstance()->gi.error ("Couldn't open %s", filename);

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
        quake2::getInstance()->gi.error ("Couldn't open %s", filename);

    // free any dynamic memory allocated by loading the level
    // base state
    quake2::getInstance()->gi.FreeTags (TAG_LEVEL);

    // wipe all the entities
    memset (g_edicts, 0, game.maxentities*sizeof(g_edicts[0]));
    globals.num_edicts = maxclients->value+1;

    // check edict size
    fread (&i, sizeof(i), 1, f);
    if (i != sizeof(edict))
    {
        fclose (f);
        quake2::getInstance()->gi.error ("ReadLevel: mismatched edict size");
    }

    // check function pointer base address
    fread (&base, sizeof(base), 1, f);
#ifdef _WIN32
    if (base != (void *)InitGame)
    {
        fclose (f);
        quake2::getInstance()->gi.error ("ReadLevel: function pointers have moved");
    }
#else
    quake2::getInstance()->gi.dprintf("Function offsets %d\n", ((byte *)base) - ((byte *)InitGame));
#endif

    // load the level locals
    ReadLevelLocals (f);

    // load all the entities
    while (1)
    {
        if (fread (&entnum, sizeof(entnum), 1, f) != 1)
        {
            fclose (f);
            quake2::getInstance()->gi.error ("ReadLevel: failed to read entnum");
        }
        if (entnum == -1)
            break;
        if (entnum >= globals.num_edicts)
            globals.num_edicts = entnum+1;

        ent = &g_edicts[entnum];
        ReadEdict (f, ent);

        // let the server rebuild world links for this ent
        memset (&ent->area, 0, sizeof(ent->area));
        quake2::getInstance()->gi.linkentity (ent);
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
