#ifndef ____GAME_G_MAIN_H__
#define ____GAME_G_MAIN_H__

#include "shared/shared_functions.hpp"

void SpawnEntities( char* mapname, char* entities, char* spawnpoint );
void ClientThink( struct edict_s* ent, usercmd_s* cmd );
bool ClientConnect( struct edict_s* ent, char* userinfo );
void ClientUserinfoChanged( struct edict_s* ent, char* userinfo );
void ClientDisconnect( struct edict_s* ent );
void ClientBegin( struct edict_s* ent );
void ClientCommand( struct edict_s* ent );
void RunEntity( struct edict_s* ent );
void WriteGame( char* filename, bool autosave );
void ReadGame( char* filename );
void WriteLevel( char* filename );
void ReadLevel( char* filename );
void InitGame( void );
void G_RunFrame( void );
void ShutdownGame( void );

#endif  // ____GAME_G_MAIN_H__