#ifndef ____SHARED_CLIENT_RESPAWN_H__
#define ____SHARED_CLIENT_RESPAWN_H__

#include "shared/client_persistant.hpp"

// client data that stays across deathmatch respawns
typedef struct
{
    client_persistant_t coop_respawn;// what to set client->pers to on a respawn
    int            enterframe;            // level.framenum the client entered the game
    int            score;                // frags, etc
//ZOID
    int            ctf_team;            // CTF team
    int            ctf_state;
    float        ctf_lasthurtcarrier;
    float        ctf_lastreturnedflag;
    float        ctf_flagsince;
    float        ctf_lastfraggedcarrier;
    bool    id_state;
    bool    voted; // for elections
    bool    ready;
    bool    admin;
    struct ghost_s *ghost; // for ghost codes
//ZOID
    vec3_t        cmd_angles;            // angles sent over in the last command
    int            game_helpchanged;
    int            helpchanged;

    bool    spectator;            // client is a spectator
} client_respawn_t;

#endif // ____SHARED_CLIENT_RESPAWN_H__