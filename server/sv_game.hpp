#ifndef ____SERVER_SV_GAME_H__
#define ____SERVER_SV_GAME_H__

#include "shared/edict.hpp"

void PF_WriteChar( int c );
void PF_WriteByte( int c );
void PF_WriteShort( int c );
void PF_WriteLong( int c );
void PF_WriteFloat( float f );
void PF_WriteString( char* s );
void PF_WritePos( vec3_t pos );
void PF_WriteDir( vec3_t dir );
void PF_WriteAngle( float f );

void PF_Unicast( edict_t* ent, bool reliable );
void PF_dprintf( char* fmt, ... );
void PF_cprintf( edict_t* ent, int level, char* fmt, ... );
void PF_centerprintf( edict_t* ent, char* fmt, ... );
void PF_error( char* fmt, ... );
void PF_setmodel( edict_t* ent, char* name );
bool PF_inPVS( vec3_t p1, vec3_t p2 );
bool PF_inPHS( vec3_t p1, vec3_t p2 );
void PF_Configstring( int index, char* val );
void PF_StartSound( edict_t* entity, int channel, int sound_num,
                    float volume, float attenuation, float timeofs );

#endif  // ____SERVER_SV_GAME_H__