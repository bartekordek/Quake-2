#ifndef ____SHARED_CLIENT_PERSISTANT_H__
#define ____SHARED_CLIENT_PERSISTANT_H__

#include "shared/defines.h"
#include "shared/gitem.h"

// client data that stays across multiple level loads
typedef struct
{
    char        userinfo[MAX_INFO_STRING];
    char        netname[16];
    int            hand;

    qboolean    connected;            // a loadgame will leave valid entities that
                                    // just don't have a connection yet

    // values saved and restored from edicts when changing levels
    int            health;
    int            max_health;
    qboolean    powerArmorActive;
    int            savedFlags;

    int            selected_item;
    int            inventory[MAX_ITEMS];

    // ammo capacities
    int            max_bullets;
    int            max_shells;
    int            max_rockets;
    int            max_grenades;
    int            max_cells;
    int            max_slugs;

    gitem_t        *weapon;
    gitem_t        *lastweapon;

    int            power_cubes;    // used for tracking the cubes in coop games
    int            score;            // for calculating total unit score in coop games

    int            game_helpchanged;
    int            helpchanged;

    qboolean    spectator;            // client is a spectator
} client_persistant_t;

#endif // ____SHARED_CLIENT_PERSISTANT_H__