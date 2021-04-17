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
#include "m_player.hpp"
#include "shared/plane.hpp"
#include "shared/surface.hpp"

typedef enum match_s
{
    MATCH_NONE,
    MATCH_SETUP,
    MATCH_PREGAME,
    MATCH_GAME,
    MATCH_POST
} match_t;

typedef enum
{
    ELECT_NONE,
    ELECT_MATCH,
    ELECT_ADMIN,
    ELECT_MAP
} elect_t;

typedef struct ctfgame_s
{
    int team1, team2;
    int total1, total2;  // these are only set when going into intermission!
    float last_flag_capture;
    int last_capture_team;

    match_t match;    // match state
    float matchtime;  // time for match start/end (depends on state)
    int lasttime;     // last time update

    elect_t election;  // election type
    edict* etarget;    // for admin election, who's being elected
    char elevel[32];   // for map election, target level
    int evotes;        // votes so far
    int needvotes;     // votes needed
    float electtime;   // remaining time until election times out
    char emsg[256];    // election name

    ghost_t ghosts[MAX_CLIENTS];  // ghost codes
} ctfgame_t;

ctfgame_t ctfgame;

cvar* ctf;
cvar* ctf_forcejoin;

cvar* competition;
cvar* matchlock;
cvar* electpercentage;
cvar* matchtime;
cvar* matchsetuptime;
cvar* matchstarttime;
cvar* admin_password;
cvar* warp_list;

char* ctf_statusbar =
    "yb    -24 "

    // health
    "xv    0 "
    "hnum "
    "xv    50 "
    "pic 0 "

    // ammo
    "if 2 "
    "    xv    100 "
    "    anum "
    "    xv    150 "
    "    pic 2 "
    "endif "

    // armor
    "if 4 "
    "    xv    200 "
    "    rnum "
    "    xv    250 "
    "    pic 4 "
    "endif "

    // selected item
    "if 6 "
    "    xv    296 "
    "    pic 6 "
    "endif "

    "yb    -50 "

    // picked up item
    "if 7 "
    "    xv    0 "
    "    pic 7 "
    "    xv    26 "
    "    yb    -42 "
    "    stat_string 8 "
    "    yb    -50 "
    "endif "

    // timer
    "if 9 "
    "xv 246 "
    "num 2 10 "
    "xv 296 "
    "pic 9 "
    "endif "

    //  help / weapon icon
    "if 11 "
    "xv 148 "
    "pic 11 "
    "endif "

    //  frags
    "xr    -50 "
    "yt 2 "
    "num 3 14 "

    // tech
    "yb -129 "
    "if 26 "
    "xr -26 "
    "pic 26 "
    "endif "

    // red team
    "yb -102 "
    "if 17 "
    "xr -26 "
    "pic 17 "
    "endif "
    "xr -62 "
    "num 2 18 "
    // joined overlay
    "if 22 "
    "yb -104 "
    "xr -28 "
    "pic 22 "
    "endif "

    // blue team
    "yb -75 "
    "if 19 "
    "xr -26 "
    "pic 19 "
    "endif "
    "xr -62 "
    "num 2 20 "
    "if 23 "
    "yb -77 "
    "xr -28 "
    "pic 23 "
    "endif "

    // have flag graph
    "if 21 "
    "yt 26 "
    "xr -24 "
    "pic 21 "
    "endif "

    // id view state
    "if 27 "
    "xv 0 "
    "yb -58 "
    "string \"Viewing\" "
    "xv 64 "
    "stat_string 27 "
    "endif "

    "if 28 "
    "xl 0 "
    "yb -78 "
    "stat_string 28 "
    "endif ";

static char* tnames[] = { "item_tech1", "item_tech2", "item_tech3",
                          "item_tech4", NULL };

void stuffcmd( edict* ent, char* s )
{
    quake2::getInstance()->gi.WriteByte( 11 );
    quake2::getInstance()->gi.WriteString( s );
    quake2::getInstance()->gi.unicast( ent, true );
}

/*--------------------------------------------------------------------------*/

/*
=================
findradius

Returns entities that have origins within a spherical area

findradius (origin, radius)
=================
*/
static edict* loc_findradius( edict* from, vec3_t org, float rad )
{
    vec3_t eorg;
    int j;

    if ( !from )
        from = g_edicts;
    else
        from++;
    for ( ; from < &g_edicts[globals.num_edicts]; from++ )
    {
        if ( !from->inuse )
            continue;
#if 0
        if (from->solid == SOLID_NOT)
            continue;
#endif
        for ( j = 0; j < 3; j++ )
            eorg[j] = org[j] - ( from->s.origin[j] +
                                 ( from->mins[j] + from->maxs[j] ) * 0.5 );
        if ( VectorLength( eorg ) > rad )
            continue;
        return from;
    }

    return NULL;
}

static void loc_buildboxpoints( vec3_t p[8], vec3_t org, vec3_t mins,
                                vec3_t maxs )
{
    VectorAdd( org, mins, p[0] );
    VectorCopy( p[0], p[1] );
    p[1][0] -= mins[0];
    VectorCopy( p[0], p[2] );
    p[2][1] -= mins[1];
    VectorCopy( p[0], p[3] );
    p[3][0] -= mins[0];
    p[3][1] -= mins[1];
    VectorAdd( org, maxs, p[4] );
    VectorCopy( p[4], p[5] );
    p[5][0] -= maxs[0];
    VectorCopy( p[0], p[6] );
    p[6][1] -= maxs[1];
    VectorCopy( p[0], p[7] );
    p[7][0] -= maxs[0];
    p[7][1] -= maxs[1];
}

static bool loc_CanSee( edict* targ, edict* inflictor )
{
    trace_t trace;
    vec3_t targpoints[8];
    int i;
    vec3_t viewpoint;

    // bmodels need special checking because their origin is 0,0,0
    if ( targ->movetype == MOVETYPE_PUSH )
        return false;  // bmodels not supported

    loc_buildboxpoints( targpoints, targ->s.origin, targ->mins, targ->maxs );

    VectorCopy( inflictor->s.origin, viewpoint );
    viewpoint[2] += inflictor->viewheight;

    for ( i = 0; i < 8; i++ )
    {
        trace = quake2::getInstance()->gi.trace( viewpoint, vec3_origin,
                                                 vec3_origin, targpoints[i],
                                                 inflictor, MASK_SOLID );
        if ( trace.fraction == 1.0 )
            return true;
    }

    return false;
}

/*--------------------------------------------------------------------------*/

static gitem* flag1_item;
static gitem* flag2_item;

void CTFSpawn( void )
{
    if ( !flag1_item )
        flag1_item = FindItemByClassname( "item_flag_team1" );
    if ( !flag2_item )
        flag2_item = FindItemByClassname( "item_flag_team2" );
    memset( &ctfgame, 0, sizeof( ctfgame ) );
    CTFSetupTechSpawn();

    if ( competition->value > 1 )
    {
        ctfgame.match = MATCH_SETUP;
        ctfgame.matchtime = level.time + matchsetuptime->value * 60;
    }
}

void CTFInit( void )
{
    ctf = quake2::getInstance()->gi.cvar_obj( "ctf", "1", CVAR_SERVERINFO );
    ctf_forcejoin =
        quake2::getInstance()->gi.cvar_obj( "ctf_forcejoin", "", 0 );
    competition = quake2::getInstance()->gi.cvar_obj( "competition", "0",
                                                      CVAR_SERVERINFO );
    matchlock =
        quake2::getInstance()->gi.cvar_obj( "matchlock", "1", CVAR_SERVERINFO );
    electpercentage =
        quake2::getInstance()->gi.cvar_obj( "electpercentage", "66", 0 );
    matchtime = quake2::getInstance()->gi.cvar_obj( "matchtime", "20",
                                                    CVAR_SERVERINFO );
    matchsetuptime =
        quake2::getInstance()->gi.cvar_obj( "matchsetuptime", "10", 0 );
    matchstarttime =
        quake2::getInstance()->gi.cvar_obj( "matchstarttime", "20", 0 );
    admin_password =
        quake2::getInstance()->gi.cvar_obj( "admin_password", "", 0 );
    warp_list = quake2::getInstance()->gi.cvar_obj(
        "warp_list", "q2ctf1 q2ctf2 q2ctf3 q2ctf4 q2ctf5", 0 );
}

/*--------------------------------------------------------------------------*/

char* CTFTeamName( int team )
{
    switch ( team )
    {
        case CTF_TEAM1:
            return "RED";
        case CTF_TEAM2:
            return "BLUE";
    }
    return "UKNOWN";
}

char* CTFOtherTeamName( int team )
{
    switch ( team )
    {
        case CTF_TEAM1:
            return "BLUE";
        case CTF_TEAM2:
            return "RED";
    }
    return "UKNOWN";
}

int CTFOtherTeam( int team )
{
    switch ( team )
    {
        case CTF_TEAM1:
            return CTF_TEAM2;
        case CTF_TEAM2:
            return CTF_TEAM1;
    }
    return -1;  // invalid value
}

/*--------------------------------------------------------------------------*/

edict* SelectRandomDeathmatchSpawnPoint( void );
edict* SelectFarthestDeathmatchSpawnPoint( void );
float PlayersRangeFromSpot( edict* spot );

void CTFAssignSkin( edict* ent, char* s )
{
    int playernum = ent - g_edicts - 1;
    char* p;
    char t[64];

    // Com_sprintf(t, sizeof(t), "%s", s);

    if ( ( p = strrchr( t, '/' ) ) != NULL )
        p[1] = 0;
    else
        strcpy( t, "male/" );

    switch ( ent->client->resp.ctf_team )
    {
        case CTF_TEAM1:
            quake2::getInstance()->gi.configstring(
                CS_PLAYERSKINS + playernum,
                va( "%s\\%s%s", ent->client->pers.netname, t,
                    CTF_TEAM1_SKIN ) );
            break;
        case CTF_TEAM2:
            quake2::getInstance()->gi.configstring(
                CS_PLAYERSKINS + playernum,
                va( "%s\\%s%s", ent->client->pers.netname, t,
                    CTF_TEAM2_SKIN ) );
            break;
        default:
            quake2::getInstance()->gi.configstring(
                CS_PLAYERSKINS + playernum,
                va( "%s\\%s", ent->client->pers.netname, s ) );
            break;
    }
    //    quake2::getInstance()->gi.cprintf(ent, PRINT_HIGH, "You have been
    //    assigned to %s team.\n", ent->client->pers.netname);
}

void CTFAssignTeam( gclient_t* who )
{
    edict* player;
    int i;
    int team1count = 0, team2count = 0;

    who->resp.ctf_state = 0;

    if ( !( (int)dmflags->value & DF_CTF_FORCEJOIN ) )
    {
        who->resp.ctf_team = CTF_NOTEAM;
        return;
    }

    for ( i = 1; i <= maxclients->value; i++ )
    {
        player = &g_edicts[i];

        if ( !player->inuse || player->client == who )
            continue;

        switch ( player->client->resp.ctf_team )
        {
            case CTF_TEAM1:
                team1count++;
                break;
            case CTF_TEAM2:
                team2count++;
        }
    }
    if ( team1count < team2count )
        who->resp.ctf_team = CTF_TEAM1;
    else if ( team2count < team1count )
        who->resp.ctf_team = CTF_TEAM2;
    else if ( rand() & 1 )
        who->resp.ctf_team = CTF_TEAM1;
    else
        who->resp.ctf_team = CTF_TEAM2;
}

/*
================
SelectCTFSpawnPoint

go to a ctf point, but NOT the two points closest
to other players
================
*/
edict* SelectCTFSpawnPoint( edict* ent )
{
    edict *spot, *spot1, *spot2;
    int count = 0;
    int selection;
    float range, range1, range2;
    char* cname;

    if ( ent->client->resp.ctf_state )
        if ( (int)( dmflags->value ) & DF_SPAWN_FARTHEST )
            return SelectFarthestDeathmatchSpawnPoint();
        else
            return SelectRandomDeathmatchSpawnPoint();

    ent->client->resp.ctf_state++;

    switch ( ent->client->resp.ctf_team )
    {
        case CTF_TEAM1:
            cname = "info_player_team1";
            break;
        case CTF_TEAM2:
            cname = "info_player_team2";
            break;
        default:
            return SelectRandomDeathmatchSpawnPoint();
    }

    spot = NULL;
    range1 = range2 = 99999;
    spot1 = spot2 = NULL;

    while ( ( spot = G_Find( spot, FOFS( classname ), cname ) ) != NULL )
    {
        count++;
        range = PlayersRangeFromSpot( spot );
        if ( range < range1 )
        {
            range1 = range;
            spot1 = spot;
        }
        else if ( range < range2 )
        {
            range2 = range;
            spot2 = spot;
        }
    }

    if ( !count )
        return SelectRandomDeathmatchSpawnPoint();

    if ( count <= 2 )
    {
        spot1 = spot2 = NULL;
    }
    else
        count -= 2;

    selection = rand() % count;

    spot = NULL;
    do
    {
        spot = G_Find( spot, FOFS( classname ), cname );
        if ( spot == spot1 || spot == spot2 )
            selection++;
    } while ( selection-- );

    return spot;
}

/*------------------------------------------------------------------------*/
/*
CTFFragBonuses

Calculate the bonuses for flag defense, flag carrier defense, etc.
Note that bonuses are not cumaltive.  You get one, they are in importance
order.
*/
void CTFFragBonuses( edict* targ, edict* inflictor, edict* attacker )
{
    int i;
    edict* ent;
    gitem *flag_item, *enemy_flag_item;
    int otherteam;
    edict *flag, *carrier;
    char* c;
    vec3_t v1, v2;

    if ( targ->client && attacker->client )
    {
        if ( attacker->client->resp.ghost )
            if ( attacker != targ )
                attacker->client->resp.ghost->kills++;
        if ( targ->client->resp.ghost )
            targ->client->resp.ghost->deaths++;
    }

    // no bonus for fragging yourself
    if ( !targ->client || !attacker->client || targ == attacker )
        return;

    otherteam = CTFOtherTeam( targ->client->resp.ctf_team );
    if ( otherteam < 0 )
        return;  // whoever died isn't on a team

    // same team, if the flag at base, check to he has the enemy flag
    if ( targ->client->resp.ctf_team == CTF_TEAM1 )
    {
        flag_item = flag1_item;
        enemy_flag_item = flag2_item;
    }
    else
    {
        flag_item = flag2_item;
        enemy_flag_item = flag1_item;
    }

    // did the attacker frag the flag carrier?
    if ( targ->client->pers.inventory[ITEM_INDEX( enemy_flag_item )] )
    {
        attacker->client->resp.ctf_lastfraggedcarrier = level.time;
        attacker->client->resp.score += CTF_FRAG_CARRIER_BONUS;
        quake2::getInstance()->gi.cprintf(
            attacker, PRINT_MEDIUM,
            "BONUS: %d points for fragging enemy flag carrier.\n",
            CTF_FRAG_CARRIER_BONUS );

        // the target had the flag, clear the hurt carrier
        // field on the other team
        for ( i = 1; i <= maxclients->value; i++ )
        {
            ent = g_edicts + i;
            if ( ent->inuse && ent->client->resp.ctf_team == otherteam )
                ent->client->resp.ctf_lasthurtcarrier = 0;
        }
        return;
    }

    if ( targ->client->resp.ctf_lasthurtcarrier &&
         level.time - targ->client->resp.ctf_lasthurtcarrier <
             CTF_CARRIER_DANGER_PROTECT_TIMEOUT &&
         !attacker->client->pers.inventory[ITEM_INDEX( flag_item )] )
    {
        // attacker is on the same team as the flag carrier and
        // fragged a guy who hurt our flag carrier
        attacker->client->resp.score += CTF_CARRIER_DANGER_PROTECT_BONUS;
        quake2::getInstance()->gi.bprintf(
            PRINT_MEDIUM,
            "%s defends %s's flag carrier against an agressive enemy\n",
            attacker->client->pers.netname,
            CTFTeamName( attacker->client->resp.ctf_team ) );
        if ( attacker->client->resp.ghost )
            attacker->client->resp.ghost->carrierdef++;
        return;
    }

    // flag and flag carrier area defense bonuses

    // we have to find the flag and carrier entities

    // find the flag
    switch ( attacker->client->resp.ctf_team )
    {
        case CTF_TEAM1:
            c = "item_flag_team1";
            break;
        case CTF_TEAM2:
            c = "item_flag_team2";
            break;
        default:
            return;
    }

    flag = NULL;
    while ( ( flag = G_Find( flag, FOFS( classname ), c ) ) != NULL )
    {
        if ( !( flag->spawnflags & DROPPED_ITEM ) )
            break;
    }

    if ( !flag )
        return;  // can't find attacker's flag

    // find attacker's team's flag carrier
    for ( i = 1; i <= maxclients->value; i++ )
    {
        carrier = g_edicts + i;
        if ( carrier->inuse &&
             carrier->client->pers.inventory[ITEM_INDEX( flag_item )] )
            break;
        carrier = NULL;
    }

    // ok we have the attackers flag and a pointer to the carrier

    // check to see if we are defending the base's flag
    VectorSubtract( targ->s.origin, flag->s.origin, v1 );
    VectorSubtract( attacker->s.origin, flag->s.origin, v2 );

    if ( ( VectorLength( v1 ) < CTF_TARGET_PROTECT_RADIUS ||
           VectorLength( v2 ) < CTF_TARGET_PROTECT_RADIUS ||
           loc_CanSee( flag, targ ) || loc_CanSee( flag, attacker ) ) &&
         attacker->client->resp.ctf_team != targ->client->resp.ctf_team )
    {
        // we defended the base flag
        attacker->client->resp.score += CTF_FLAG_DEFENSE_BONUS;
        if ( flag->solid == SOLID_NOT )
            quake2::getInstance()->gi.bprintf(
                PRINT_MEDIUM, "%s defends the %s base.\n",
                attacker->client->pers.netname,
                CTFTeamName( attacker->client->resp.ctf_team ) );
        else
            quake2::getInstance()->gi.bprintf(
                PRINT_MEDIUM, "%s defends the %s flag.\n",
                attacker->client->pers.netname,
                CTFTeamName( attacker->client->resp.ctf_team ) );
        if ( attacker->client->resp.ghost )
            attacker->client->resp.ghost->basedef++;
        return;
    }

    if ( carrier && carrier != attacker )
    {
        VectorSubtract( targ->s.origin, carrier->s.origin, v1 );
        VectorSubtract( attacker->s.origin, carrier->s.origin, v1 );

        if ( VectorLength( v1 ) < CTF_ATTACKER_PROTECT_RADIUS ||
             VectorLength( v2 ) < CTF_ATTACKER_PROTECT_RADIUS ||
             loc_CanSee( carrier, targ ) || loc_CanSee( carrier, attacker ) )
        {
            attacker->client->resp.score += CTF_CARRIER_PROTECT_BONUS;
            quake2::getInstance()->gi.bprintf(
                PRINT_MEDIUM, "%s defends the %s's flag carrier.\n",
                attacker->client->pers.netname,
                CTFTeamName( attacker->client->resp.ctf_team ) );
            if ( attacker->client->resp.ghost )
                attacker->client->resp.ghost->carrierdef++;
            return;
        }
    }
}

void CTFCheckHurtCarrier( edict* targ, edict* attacker )
{
    gitem* flag_item;

    if ( !targ->client || !attacker->client )
        return;

    if ( targ->client->resp.ctf_team == CTF_TEAM1 )
        flag_item = flag2_item;
    else
        flag_item = flag1_item;

    if ( targ->client->pers.inventory[ITEM_INDEX( flag_item )] &&
         targ->client->resp.ctf_team != attacker->client->resp.ctf_team )
        attacker->client->resp.ctf_lasthurtcarrier = level.time;
}

/*------------------------------------------------------------------------*/

void CTFResetFlag( int ctf_team )
{
    char* c;
    edict* ent;

    switch ( ctf_team )
    {
        case CTF_TEAM1:
            c = "item_flag_team1";
            break;
        case CTF_TEAM2:
            c = "item_flag_team2";
            break;
        default:
            return;
    }

    ent = NULL;
    while ( ( ent = G_Find( ent, FOFS( classname ), c ) ) != NULL )
    {
        if ( ent->spawnflags & DROPPED_ITEM )
            G_FreeEdict( ent );
        else
        {
            ent->svflags &= ~SVF_NOCLIENT;
            ent->solid = SOLID_TRIGGER;
            quake2::getInstance()->gi.linkentity( ent );
            ent->s.event = EV_ITEM_RESPAWN;
        }
    }
}

void CTFResetFlags( void )
{
    CTFResetFlag( CTF_TEAM1 );
    CTFResetFlag( CTF_TEAM2 );
}

bool CTFPickup_Flag( edict* ent, edict* other )
{
    int ctf_team;
    int i;
    edict* player;
    gitem *flag_item, *enemy_flag_item;

    // figure out what team this flag is
    if ( strcmp( ent->classname, "item_flag_team1" ) == 0 )
        ctf_team = CTF_TEAM1;
    else if ( strcmp( ent->classname, "item_flag_team2" ) == 0 )
        ctf_team = CTF_TEAM2;
    else
    {
        quake2::getInstance()->gi.cprintf(
            ent, PRINT_HIGH, "Don't know what team the flag is on.\n" );
        return false;
    }

    // same team, if the flag at base, check to he has the enemy flag
    if ( ctf_team == CTF_TEAM1 )
    {
        flag_item = flag1_item;
        enemy_flag_item = flag2_item;
    }
    else
    {
        flag_item = flag2_item;
        enemy_flag_item = flag1_item;
    }

    if ( ctf_team == other->client->resp.ctf_team )
    {
        if ( !( ent->spawnflags & DROPPED_ITEM ) )
        {
            // the flag is at home base.  if the player has the enemy
            // flag, he's just won!

            if ( other->client->pers.inventory[ITEM_INDEX( enemy_flag_item )] )
            {
                quake2::getInstance()->gi.bprintf(
                    PRINT_HIGH, "%s captured the %s flag!\n",
                    other->client->pers.netname, CTFOtherTeamName( ctf_team ) );
                other->client->pers.inventory[ITEM_INDEX( enemy_flag_item )] =
                    0;

                ctfgame.last_flag_capture = level.time;
                ctfgame.last_capture_team = ctf_team;
                if ( ctf_team == CTF_TEAM1 )
                    ctfgame.team1++;
                else
                    ctfgame.team2++;

                quake2::getInstance()->gi.sound(
                    ent, CHAN_RELIABLE + CHAN_NO_PHS_ADD + CHAN_VOICE,
                    quake2::getInstance()->gi.soundindex( "ctf/flagcap.wav" ),
                    1, ATTN_NONE, 0 );

                // other gets another 10 frag bonus
                other->client->resp.score += CTF_CAPTURE_BONUS;
                if ( other->client->resp.ghost )
                    other->client->resp.ghost->caps++;

                // Ok, let's do the player loop, hand out the bonuses
                for ( i = 1; i <= maxclients->value; i++ )
                {
                    player = &g_edicts[i];
                    if ( !player->inuse )
                        continue;

                    if ( player->client->resp.ctf_team !=
                         other->client->resp.ctf_team )
                        player->client->resp.ctf_lasthurtcarrier = -5;
                    else if ( player->client->resp.ctf_team ==
                              other->client->resp.ctf_team )
                    {
                        if ( player != other )
                            player->client->resp.score += CTF_TEAM_BONUS;
                        // award extra points for capture assists
                        if ( player->client->resp.ctf_lastreturnedflag +
                                 CTF_RETURN_FLAG_ASSIST_TIMEOUT >
                             level.time )
                        {
                            quake2::getInstance()->gi.bprintf(
                                PRINT_HIGH,
                                "%s gets an assist for returning the flag!\n",
                                player->client->pers.netname );
                            player->client->resp.score +=
                                CTF_RETURN_FLAG_ASSIST_BONUS;
                        }
                        if ( player->client->resp.ctf_lastfraggedcarrier +
                                 CTF_FRAG_CARRIER_ASSIST_TIMEOUT >
                             level.time )
                        {
                            quake2::getInstance()->gi.bprintf(
                                PRINT_HIGH,
                                "%s gets an assist for fragging the "
                                "flag carrier!\n",
                                player->client->pers.netname );
                            player->client->resp.score +=
                                CTF_FRAG_CARRIER_ASSIST_BONUS;
                        }
                    }
                }

                CTFResetFlags();
                return false;
            }
            return false;  // its at home base already
        }
        // hey, its not home.  return it by teleporting it back
        quake2::getInstance()->gi.bprintf(
            PRINT_HIGH, "%s returned the %s flag!\n",
            other->client->pers.netname, CTFTeamName( ctf_team ) );
        other->client->resp.score += CTF_RECOVERY_BONUS;
        other->client->resp.ctf_lastreturnedflag = level.time;
        quake2::getInstance()->gi.sound(
            ent, CHAN_RELIABLE + CHAN_NO_PHS_ADD + CHAN_VOICE,
            quake2::getInstance()->gi.soundindex( "ctf/flagret.wav" ), 1,
            ATTN_NONE, 0 );
        // CTFResetFlag will remove this entity!  We must return false
        CTFResetFlag( ctf_team );
        return false;
    }

    // hey, its not our flag, pick it up
    quake2::getInstance()->gi.bprintf( PRINT_HIGH, "%s got the %s flag!\n",
                                       other->client->pers.netname,
                                       CTFTeamName( ctf_team ) );
    other->client->resp.score += CTF_FLAG_BONUS;

    other->client->pers.inventory[ITEM_INDEX( flag_item )] = 1;
    other->client->resp.ctf_flagsince = level.time;

    // pick up the flag
    // if it's not a dropped flag, we just make is disappear
    // if it's dropped, it will be removed by the pickup caller
    if ( !( ent->spawnflags & DROPPED_ITEM ) )
    {
        ent->flags |= FL_RESPAWN;
        ent->svflags |= SVF_NOCLIENT;
        ent->solid = SOLID_NOT;
    }
    return true;
}

static void CTFDropFlagTouch( edict* ent, edict* other, plane_s* plane,
                              csurface_s* surf )
{
    // owner (who dropped us) can't touch for two secs
    if ( other == ent->owner &&
         ent->nextthink - level.time > CTF_AUTO_FLAG_RETURN_TIMEOUT - 2 )
        return;

    Touch_Item( ent, other, plane, surf );
}

static void CTFDropFlagThink( edict* ent )
{
    // auto return the flag
    // reset flag will remove ourselves
    if ( strcmp( ent->classname, "item_flag_team1" ) == 0 )
    {
        CTFResetFlag( CTF_TEAM1 );
        quake2::getInstance()->gi.bprintf( PRINT_HIGH,
                                           "The %s flag has returned!\n",
                                           CTFTeamName( CTF_TEAM1 ) );
    }
    else if ( strcmp( ent->classname, "item_flag_team2" ) == 0 )
    {
        CTFResetFlag( CTF_TEAM2 );
        quake2::getInstance()->gi.bprintf( PRINT_HIGH,
                                           "The %s flag has returned!\n",
                                           CTFTeamName( CTF_TEAM2 ) );
    }
}

// Called from PlayerDie, to drop the flag from a dying player
void CTFDeadDropFlag( edict* self )
{
    edict* dropped = NULL;

    if ( self->client->pers.inventory[ITEM_INDEX( flag1_item )] )
    {
        dropped = Drop_Item( self, flag1_item );
        self->client->pers.inventory[ITEM_INDEX( flag1_item )] = 0;
        quake2::getInstance()->gi.bprintf( PRINT_HIGH, "%s lost the %s flag!\n",
                                           self->client->pers.netname,
                                           CTFTeamName( CTF_TEAM1 ) );
    }
    else if ( self->client->pers.inventory[ITEM_INDEX( flag2_item )] )
    {
        dropped = Drop_Item( self, flag2_item );
        self->client->pers.inventory[ITEM_INDEX( flag2_item )] = 0;
        quake2::getInstance()->gi.bprintf( PRINT_HIGH, "%s lost the %s flag!\n",
                                           self->client->pers.netname,
                                           CTFTeamName( CTF_TEAM2 ) );
    }

    if ( dropped )
    {
        dropped->think = CTFDropFlagThink;
        dropped->nextthink = level.time + CTF_AUTO_FLAG_RETURN_TIMEOUT;
        dropped->touch = CTFDropFlagTouch;
    }
}

bool CTFDrop_Flag( edict* ent, gitem* item )
{
    if ( rand() & 1 )
        quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH,
                                           "Only lusers drop flags.\n" );
    else
        quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH,
                                           "Winners don't drop flags.\n" );
    return false;
}

static void CTFFlagThink( edict* ent )
{
    if ( ent->solid != SOLID_NOT )
        ent->s.frame = 173 + ( ( ( ent->s.frame - 173 ) + 1 ) % 16 );
    ent->nextthink = level.time + FRAMETIME;
}

void CTFFlagSetup( edict* ent )
{
    trace_t tr;
    vec3_t dest;
    float* v;

    v = tv( -15, -15, -15 );
    VectorCopy( v, ent->mins );
    v = tv( 15, 15, 15 );
    VectorCopy( v, ent->maxs );

    if ( ent->model )
        quake2::getInstance()->gi.setmodel( ent, ent->model );
    else
        quake2::getInstance()->gi.setmodel( ent, ent->item->world_model );
    ent->solid = SOLID_TRIGGER;
    ent->movetype = MOVETYPE_TOSS;
    ent->touch = Touch_Item;

    v = tv( 0, 0, -128 );
    VectorAdd( ent->s.origin, v, dest );

    tr = quake2::getInstance()->gi.trace( ent->s.origin, ent->mins, ent->maxs,
                                          dest, ent, MASK_SOLID );
    if ( tr.startsolid )
    {
        quake2::getInstance()->gi.dprintf(
            "CTFFlagSetup: %s startsolid at %s\n", ent->classname,
            vtos( ent->s.origin ) );
        G_FreeEdict( ent );
        return;
    }

    VectorCopy( tr.endpos, ent->s.origin );

    quake2::getInstance()->gi.linkentity( ent );

    ent->nextthink = level.time + FRAMETIME;
    ent->think = CTFFlagThink;
}

void CTFEffects( edict* player )
{
    player->s.effects &= ~( EF_FLAG1 | EF_FLAG2 );
    if ( player->health > 0 )
    {
        if ( player->client->pers.inventory[ITEM_INDEX( flag1_item )] )
        {
            player->s.effects |= EF_FLAG1;
        }
        if ( player->client->pers.inventory[ITEM_INDEX( flag2_item )] )
        {
            player->s.effects |= EF_FLAG2;
        }
    }

    if ( player->client->pers.inventory[ITEM_INDEX( flag1_item )] )
        player->s.modelindex3 =
            quake2::getInstance()->gi.modelindex( "players/male/flag1.md2" );
    else if ( player->client->pers.inventory[ITEM_INDEX( flag2_item )] )
        player->s.modelindex3 =
            quake2::getInstance()->gi.modelindex( "players/male/flag2.md2" );
    else
        player->s.modelindex3 = 0;
}

// called when we enter the intermission
void CTFCalcScores( void )
{
    int i;

    ctfgame.total1 = ctfgame.total2 = 0;
    for ( i = 0; i < maxclients->value; i++ )
    {
        if ( !g_edicts[i + 1].inuse )
            continue;
        if ( game.clients[i].resp.ctf_team == CTF_TEAM1 )
            ctfgame.total1 += game.clients[i].resp.score;
        else if ( game.clients[i].resp.ctf_team == CTF_TEAM2 )
            ctfgame.total2 += game.clients[i].resp.score;
    }
}

void CTFID_f( edict* ent )
{
    if ( ent->client->resp.id_state )
    {
        quake2::getInstance()->gi.cprintf(
            ent, PRINT_HIGH, "Disabling player identication display.\n" );
        ent->client->resp.id_state = false;
    }
    else
    {
        quake2::getInstance()->gi.cprintf(
            ent, PRINT_HIGH, "Activating player identication display.\n" );
        ent->client->resp.id_state = true;
    }
}

static void CTFSetIDView( edict* ent )
{
    vec3_t forward, dir;
    trace_t tr;
    edict *who, *best;
    float bd = 0, d;
    int i;

    ent->client->ps.stats[STAT_CTF_ID_VIEW] = 0;

    AngleVectors( ent->client->v_angle, forward, NULL, NULL );
    VectorScale( forward, 1024, forward );
    VectorAdd( ent->s.origin, forward, forward );
    tr = quake2::getInstance()->gi.trace( ent->s.origin, NULL, NULL, forward,
                                          ent, MASK_SOLID );
    if ( tr.fraction < 1 && tr.ent && tr.ent->client )
    {
        ent->client->ps.stats[STAT_CTF_ID_VIEW] =
            CS_PLAYERSKINS + ( ent - g_edicts - 1 );
        return;
    }

    AngleVectors( ent->client->v_angle, forward, NULL, NULL );
    best = NULL;
    for ( i = 1; i <= maxclients->value; i++ )
    {
        who = g_edicts + i;
        if ( !who->inuse || who->solid == SOLID_NOT )
            continue;
        VectorSubtract( who->s.origin, ent->s.origin, dir );
        VectorNormalize( dir );
        d = DotProduct( forward, dir );
        if ( d > bd && loc_CanSee( ent, who ) )
        {
            bd = d;
            best = who;
        }
    }
    if ( bd > 0.90 )
        ent->client->ps.stats[STAT_CTF_ID_VIEW] =
            CS_PLAYERSKINS + ( best - g_edicts - 1 );
}

void SetCTFStats( edict* ent )
{
    gitem* tech;
    int i;
    int p1, p2;
    edict* e;

    if ( ctfgame.match > MATCH_NONE )
        ent->client->ps.stats[STAT_CTF_MATCH] = CONFIG_CTF_MATCH;
    else
        ent->client->ps.stats[STAT_CTF_MATCH] = 0;

    // ghosting
    if ( ent->client->resp.ghost )
    {
        ent->client->resp.ghost->score = ent->client->resp.score;
        strcpy( ent->client->resp.ghost->netname, ent->client->pers.netname );
        ent->client->resp.ghost->number = ent->s.number;
    }

    // logo headers for the frag display
    ent->client->ps.stats[STAT_CTF_TEAM1_HEADER] =
        quake2::getInstance()->gi.imageindex( "ctfsb1" );
    ent->client->ps.stats[STAT_CTF_TEAM2_HEADER] =
        quake2::getInstance()->gi.imageindex( "ctfsb2" );

    // if during intermission, we must blink the team header of the winning team
    if ( level.intermissiontime && ( level.framenum & 8 ) )
    {  // blink 1/8th second
        // note that ctfgame.total[12] is set when we go to intermission
        if ( ctfgame.team1 > ctfgame.team2 )
            ent->client->ps.stats[STAT_CTF_TEAM1_HEADER] = 0;
        else if ( ctfgame.team2 > ctfgame.team1 )
            ent->client->ps.stats[STAT_CTF_TEAM2_HEADER] = 0;
        else if ( ctfgame.total1 > ctfgame.total2 )  // frag tie breaker
            ent->client->ps.stats[STAT_CTF_TEAM1_HEADER] = 0;
        else if ( ctfgame.total2 > ctfgame.total1 )
            ent->client->ps.stats[STAT_CTF_TEAM2_HEADER] = 0;
        else
        {  // tie game!
            ent->client->ps.stats[STAT_CTF_TEAM1_HEADER] = 0;
            ent->client->ps.stats[STAT_CTF_TEAM2_HEADER] = 0;
        }
    }

    // tech icon
    i = 0;
    ent->client->ps.stats[STAT_CTF_TECH] = 0;
    while ( tnames[i] )
    {
        if ( ( tech = FindItemByClassname( tnames[i] ) ) != NULL &&
             ent->client->pers.inventory[ITEM_INDEX( tech )] )
        {
            ent->client->ps.stats[STAT_CTF_TECH] =
                quake2::getInstance()->gi.imageindex( tech->icon );
            break;
        }
        i++;
    }

    // figure out what icon to display for team logos
    // three states:
    //   flag at base
    //   flag taken
    //   flag dropped
    p1 = quake2::getInstance()->gi.imageindex( "i_ctf1" );
    e = G_Find( NULL, FOFS( classname ), "item_flag_team1" );
    if ( e != NULL )
    {
        if ( e->solid == SOLID_NOT )
        {
            int i;

            // not at base
            // check if on player
            p1 = quake2::getInstance()->gi.imageindex(
                "i_ctf1d" );  // default to dropped
            for ( i = 1; i <= maxclients->value; i++ )
                if ( g_edicts[i].inuse &&
                     g_edicts[i]
                         .client->pers.inventory[ITEM_INDEX( flag1_item )] )
                {
                    // enemy has it
                    p1 = quake2::getInstance()->gi.imageindex( "i_ctf1t" );
                    break;
                }
        }
        else if ( e->spawnflags & DROPPED_ITEM )
            p1 = quake2::getInstance()->gi.imageindex(
                "i_ctf1d" );  // must be dropped
    }
    p2 = quake2::getInstance()->gi.imageindex( "i_ctf2" );
    e = G_Find( NULL, FOFS( classname ), "item_flag_team2" );
    if ( e != NULL )
    {
        if ( e->solid == SOLID_NOT )
        {
            int i;

            // not at base
            // check if on player
            p2 = quake2::getInstance()->gi.imageindex(
                "i_ctf2d" );  // default to dropped
            for ( i = 1; i <= maxclients->value; i++ )
                if ( g_edicts[i].inuse &&
                     g_edicts[i]
                         .client->pers.inventory[ITEM_INDEX( flag2_item )] )
                {
                    // enemy has it
                    p2 = quake2::getInstance()->gi.imageindex( "i_ctf2t" );
                    break;
                }
        }
        else if ( e->spawnflags & DROPPED_ITEM )
            p2 = quake2::getInstance()->gi.imageindex(
                "i_ctf2d" );  // must be dropped
    }

    ent->client->ps.stats[STAT_CTF_TEAM1_PIC] = p1;
    ent->client->ps.stats[STAT_CTF_TEAM2_PIC] = p2;

    if ( ctfgame.last_flag_capture &&
         level.time - ctfgame.last_flag_capture < 5 )
    {
        if ( ctfgame.last_capture_team == CTF_TEAM1 )
            if ( level.framenum & 8 )
                ent->client->ps.stats[STAT_CTF_TEAM1_PIC] = p1;
            else
                ent->client->ps.stats[STAT_CTF_TEAM1_PIC] = 0;
        else if ( level.framenum & 8 )
            ent->client->ps.stats[STAT_CTF_TEAM2_PIC] = p2;
        else
            ent->client->ps.stats[STAT_CTF_TEAM2_PIC] = 0;
    }

    ent->client->ps.stats[STAT_CTF_TEAM1_CAPS] = ctfgame.team1;
    ent->client->ps.stats[STAT_CTF_TEAM2_CAPS] = ctfgame.team2;

    ent->client->ps.stats[STAT_CTF_FLAG_PIC] = 0;
    if ( ent->client->resp.ctf_team == CTF_TEAM1 &&
         ent->client->pers.inventory[ITEM_INDEX( flag2_item )] &&
         ( level.framenum & 8 ) )
        ent->client->ps.stats[STAT_CTF_FLAG_PIC] =
            quake2::getInstance()->gi.imageindex( "i_ctf2" );

    else if ( ent->client->resp.ctf_team == CTF_TEAM2 &&
              ent->client->pers.inventory[ITEM_INDEX( flag1_item )] &&
              ( level.framenum & 8 ) )
        ent->client->ps.stats[STAT_CTF_FLAG_PIC] =
            quake2::getInstance()->gi.imageindex( "i_ctf1" );

    ent->client->ps.stats[STAT_CTF_JOINED_TEAM1_PIC] = 0;
    ent->client->ps.stats[STAT_CTF_JOINED_TEAM2_PIC] = 0;
    if ( ent->client->resp.ctf_team == CTF_TEAM1 )
        ent->client->ps.stats[STAT_CTF_JOINED_TEAM1_PIC] =
            quake2::getInstance()->gi.imageindex( "i_ctfj" );
    else if ( ent->client->resp.ctf_team == CTF_TEAM2 )
        ent->client->ps.stats[STAT_CTF_JOINED_TEAM2_PIC] =
            quake2::getInstance()->gi.imageindex( "i_ctfj" );

    ent->client->ps.stats[STAT_CTF_ID_VIEW] = 0;
    if ( ent->client->resp.id_state )
        CTFSetIDView( ent );
}

/*------------------------------------------------------------------------*/

/*QUAKED info_player_team1 (1 0 0) (-16 -16 -24) (16 16 32)
potential team1 spawning position for ctf games
*/
void SP_info_player_team1( edict* self ) {}

/*QUAKED info_player_team2 (0 0 1) (-16 -16 -24) (16 16 32)
potential team2 spawning position for ctf games
*/
void SP_info_player_team2( edict* self ) {}

/*------------------------------------------------------------------------*/
/* GRAPPLE                                                                  */
/*------------------------------------------------------------------------*/

// ent is player
void CTFPlayerResetGrapple( edict* ent )
{
    if ( ent->client && ent->client->ctf_grapple )
        CTFResetGrapple( ent->client->ctf_grapple );
}

// self is grapple, not player
void CTFResetGrapple( edict* self )
{
    if ( self->owner->client->ctf_grapple )
    {
        float volume = 1.0;
        gclient_t* cl;

        if ( self->owner->client->silencer_shots )
            volume = 0.2;

        quake2::getInstance()->gi.sound( self->owner,
                                         CHAN_RELIABLE + CHAN_WEAPON,
                                         quake2::getInstance()->gi.soundindex(
                                             "weapons/grapple/grreset.wav" ),
                                         volume, ATTN_NORM, 0 );
        cl = self->owner->client;
        cl->ctf_grapple = NULL;
        cl->ctf_grapplereleasetime = level.time;
        cl->ctf_grapplestate =
            CTF_GRAPPLE_STATE_FLY;  // we're firing, not on hook
        cl->ps.pmove.pm_flags &= ~PMF_NO_PREDICTION;
        G_FreeEdict( self );
    }
}

void CTFGrappleTouch( edict* self, edict* other, plane_s* plane,
                      csurface_s* surf )
{
    float volume = 1.0;

    if ( other == self->owner )
        return;

    if ( self->owner->client->ctf_grapplestate != CTF_GRAPPLE_STATE_FLY )
        return;

    if ( surf && ( surf->flags & SURF_SKY ) )
    {
        CTFResetGrapple( self );
        return;
    }

    VectorCopy( vec3_origin, self->velocity );

    PlayerNoise( self->owner, self->s.origin, PNOISE_IMPACT );

    if ( other->takedamage )
    {
        T_Damage( other, self, self->owner, self->velocity, self->s.origin,
                  plane->normal, self->dmg, 1, 0, MOD_GRAPPLE );
        CTFResetGrapple( self );
        return;
    }

    self->owner->client->ctf_grapplestate =
        CTF_GRAPPLE_STATE_PULL;  // we're on hook
    self->enemy = other;

    self->solid = SOLID_NOT;

    if ( self->owner->client->silencer_shots )
        volume = 0.2;

    quake2::getInstance()->gi.sound(
        self->owner, CHAN_RELIABLE + CHAN_WEAPON,
        quake2::getInstance()->gi.soundindex( "weapons/grapple/grpull.wav" ),
        volume, ATTN_NORM, 0 );
    quake2::getInstance()->gi.sound(
        self, CHAN_WEAPON,
        quake2::getInstance()->gi.soundindex( "weapons/grapple/grhit.wav" ),
        volume, ATTN_NORM, 0 );

    quake2::getInstance()->gi.WriteByte( svc_temp_entity );
    quake2::getInstance()->gi.WriteByte( TE_SPARKS );
    quake2::getInstance()->gi.WritePosition( self->s.origin );
    if ( !plane )
        quake2::getInstance()->gi.WriteDir( vec3_origin );
    else
        quake2::getInstance()->gi.WriteDir( plane->normal );
    quake2::getInstance()->gi.multicast( self->s.origin,
                                         multicast_t::MULTICAST_PVS );
}

// draw beam between grapple and self
void CTFGrappleDrawCable( edict* self )
{
    vec3_t offset, start, end, f, r;
    vec3_t dir;
    float distance;

    AngleVectors( self->owner->client->v_angle, f, r, NULL );
    VectorSet( offset, 16, 16, self->owner->viewheight - 8 );
    P_ProjectSource( self->owner->client, self->owner->s.origin, offset, f, r,
                     start );

    VectorSubtract( start, self->owner->s.origin, offset );

    VectorSubtract( start, self->s.origin, dir );
    distance = VectorLength( dir );
    // don't draw cable if close
    if ( distance < 64 )
        return;

#if 0
    if (distance > 256)
        return;

    // check for min/max pitch
    vectoangles (dir, angles);
    if (angles[0] < -180)
        angles[0] += 360;
    if (fabs(angles[0]) > 45)
        return;

    trace_t    tr; //!!

    tr = quake2::getInstance()->gi.trace (start, NULL, NULL, self->s.origin, self, MASK_SHOT);
    if (tr.ent != self) {
        CTFResetGrapple(self);
        return;
    }
#endif

    // adjust start for beam origin being in middle of a segment
    //    VectorMA (start, 8, f, start);

    VectorCopy( self->s.origin, end );
    // adjust end z for end spot since the monster is currently dead
    //    end[2] = self->absmin[2] + self->size[2] / 2;

    quake2::getInstance()->gi.WriteByte( svc_temp_entity );
#if 1  // def USE_GRAPPLE_CABLE
    quake2::getInstance()->gi.WriteByte( TE_GRAPPLE_CABLE );
    quake2::getInstance()->gi.WriteShort( self->owner - g_edicts );
    quake2::getInstance()->gi.WritePosition( self->owner->s.origin );
    quake2::getInstance()->gi.WritePosition( end );
    quake2::getInstance()->gi.WritePosition( offset );
#else
    quake2::getInstance()->gi.WriteByte( TE_MEDIC_CABLE_ATTACK );
    quake2::getInstance()->gi.WriteShort( self - g_edicts );
    quake2::getInstance()->gi.WritePosition( end );
    quake2::getInstance()->gi.WritePosition( start );
#endif
    quake2::getInstance()->gi.multicast( self->s.origin,
                                         multicast_t::MULTICAST_PVS );
}

void SV_AddGravity( edict* ent );

// pull the player toward the grapple
void CTFGrapplePull( edict* self )
{
    vec3_t hookdir, v;
    float vlen;

    if ( strcmp( self->owner->client->pers.weapon->classname,
                 "weapon_grapple" ) == 0 &&
         !self->owner->client->newweapon &&
         self->owner->client->weaponstate != WEAPON_FIRING &&
         self->owner->client->weaponstate != WEAPON_ACTIVATING )
    {
        CTFResetGrapple( self );
        return;
    }

    if ( self->enemy )
    {
        if ( self->enemy->solid == SOLID_NOT )
        {
            CTFResetGrapple( self );
            return;
        }
        if ( self->enemy->solid == SOLID_BBOX )
        {
            VectorScale( self->enemy->size, 0.5, v );
            VectorAdd( v, self->enemy->s.origin, v );
            VectorAdd( v, self->enemy->mins, self->s.origin );
            quake2::getInstance()->gi.linkentity( self );
        }
        else
            VectorCopy( self->enemy->velocity, self->velocity );
        if ( self->enemy->takedamage &&
             !CheckTeamDamage( self->enemy, self->owner ) )
        {
            float volume = 1.0;

            if ( self->owner->client->silencer_shots )
                volume = 0.2;

            T_Damage( self->enemy, self, self->owner, self->velocity,
                      self->s.origin, vec3_origin, 1, 1, 0, MOD_GRAPPLE );
            quake2::getInstance()->gi.sound(
                self, CHAN_WEAPON,
                quake2::getInstance()->gi.soundindex(
                    "weapons/grapple/grhurt.wav" ),
                volume, ATTN_NORM, 0 );
        }
        if ( self->enemy->deadflag )
        {  // he died
            CTFResetGrapple( self );
            return;
        }
    }

    CTFGrappleDrawCable( self );

    if ( self->owner->client->ctf_grapplestate > CTF_GRAPPLE_STATE_FLY )
    {
        // pull player toward grapple
        // this causes icky stuff with prediction, we need to extend
        // the prediction layer to include two new fields in the player
        // move stuff: a point and a velocity.  The client should add
        // that velociy in the direction of the point
        vec3_t forward, up;

        AngleVectors( self->owner->client->v_angle, forward, NULL, up );
        VectorCopy( self->owner->s.origin, v );
        v[2] += self->owner->viewheight;
        VectorSubtract( self->s.origin, v, hookdir );

        vlen = VectorLength( hookdir );

        if ( self->owner->client->ctf_grapplestate == CTF_GRAPPLE_STATE_PULL &&
             vlen < 64 )
        {
            float volume = 1.0;

            if ( self->owner->client->silencer_shots )
                volume = 0.2;

            self->owner->client->ps.pmove.pm_flags |= PMF_NO_PREDICTION;
            quake2::getInstance()->gi.sound(
                self->owner, CHAN_RELIABLE + CHAN_WEAPON,
                quake2::getInstance()->gi.soundindex(
                    "weapons/grapple/grhang.wav" ),
                volume, ATTN_NORM, 0 );
            self->owner->client->ctf_grapplestate = CTF_GRAPPLE_STATE_HANG;
        }

        VectorNormalize( hookdir );
        VectorScale( hookdir, CTF_GRAPPLE_PULL_SPEED, hookdir );
        VectorCopy( hookdir, self->owner->velocity );
        SV_AddGravity( self->owner );
    }
}

void CTFFireGrapple( edict* self, vec3_t start, vec3_t dir, int damage,
                     int speed, int effect )
{
    edict* grapple;
    trace_t tr;

    VectorNormalize( dir );

    grapple = G_Spawn();
    VectorCopy( start, grapple->s.origin );
    VectorCopy( start, grapple->s.old_origin );
    vectoangles( dir, grapple->s.angles );
    VectorScale( dir, speed, grapple->velocity );
    grapple->movetype = MOVETYPE_FLYMISSILE;
    grapple->clipmask = MASK_SHOT;
    grapple->solid = SOLID_BBOX;
    grapple->s.effects |= effect;
    VectorClear( grapple->mins );
    VectorClear( grapple->maxs );
    grapple->s.modelindex = quake2::getInstance()->gi.modelindex(
        "models/weapons/grapple/hook/tris.md2" );
    //    grapple->s.sound = quake2::getInstance()->gi.soundindex
    //    ("misc/lasfly.wav");
    grapple->owner = self;
    grapple->touch = CTFGrappleTouch;
    //    grapple->nextthink = level.time + FRAMETIME;
    //    grapple->think = CTFGrappleThink;
    grapple->dmg = damage;
    self->client->ctf_grapple = grapple;
    self->client->ctf_grapplestate =
        CTF_GRAPPLE_STATE_FLY;  // we're firing, not on hook
    quake2::getInstance()->gi.linkentity( grapple );

    tr = quake2::getInstance()->gi.trace(
        self->s.origin, NULL, NULL, grapple->s.origin, grapple, MASK_SHOT );
    if ( tr.fraction < 1.0 )
    {
        VectorMA( grapple->s.origin, -10, dir, grapple->s.origin );
        grapple->touch( grapple, tr.ent, NULL, NULL );
    }
}

void CTFGrappleFire( edict* ent, vec3_t g_offset, int damage, int effect )
{
    vec3_t forward, right;
    vec3_t start;
    vec3_t offset;
    float volume = 1.0;

    if ( ent->client->ctf_grapplestate > CTF_GRAPPLE_STATE_FLY )
        return;  // it's already out

    AngleVectors( ent->client->v_angle, forward, right, NULL );
    //    VectorSet(offset, 24, 16, ent->viewheight-8+2);
    VectorSet( offset, 24, 8, ent->viewheight - 8 + 2 );
    VectorAdd( offset, g_offset, offset );
    P_ProjectSource( ent->client, ent->s.origin, offset, forward, right,
                     start );

    VectorScale( forward, -2, ent->client->kick_origin );
    ent->client->kick_angles[0] = -1;

    if ( ent->client->silencer_shots )
        volume = 0.2;

    quake2::getInstance()->gi.sound(
        ent, CHAN_RELIABLE + CHAN_WEAPON,
        quake2::getInstance()->gi.soundindex( "weapons/grapple/grfire.wav" ),
        volume, ATTN_NORM, 0 );
    CTFFireGrapple( ent, start, forward, damage, CTF_GRAPPLE_SPEED, effect );

#if 0
    // send muzzle flash
    quake2::getInstance()->gi.WriteByte (svc_muzzleflash);
    quake2::getInstance()->gi.WriteShort (ent-g_edicts);
    quake2::getInstance()->gi.WriteByte (MZ_BLASTER);
    quake2::getInstance()->gi.multicast (ent->s.origin, multicast_t::MULTICAST_PVS);
#endif

    PlayerNoise( ent, start, PNOISE_WEAPON );
}

void CTFWeapon_Grapple_Fire( edict* ent )
{
    int damage;

    damage = 10;
    CTFGrappleFire( ent, vec3_origin, damage, 0 );
    ent->client->ps.gunframe++;
}

void CTFWeapon_Grapple( edict* ent )
{
    static int pause_frames[] = { 10, 18, 27, 0 };
    static int fire_frames[] = { 6, 0 };
    int prevstate;

    // if the the attack button is still down, stay in the firing frame
    if ( ( ent->client->buttons & BUTTON_ATTACK ) &&
         ent->client->weaponstate == WEAPON_FIRING && ent->client->ctf_grapple )
        ent->client->ps.gunframe = 9;

    if ( !( ent->client->buttons & BUTTON_ATTACK ) && ent->client->ctf_grapple )
    {
        CTFResetGrapple( ent->client->ctf_grapple );
        if ( ent->client->weaponstate == WEAPON_FIRING )
            ent->client->weaponstate = WEAPON_READY;
    }

    if ( ent->client->newweapon &&
         ent->client->ctf_grapplestate > CTF_GRAPPLE_STATE_FLY &&
         ent->client->weaponstate == WEAPON_FIRING )
    {
        // he wants to change weapons while grappled
        ent->client->weaponstate = WEAPON_DROPPING;
        ent->client->ps.gunframe = 32;
    }

    prevstate = ent->client->weaponstate;
    Weapon_Generic( ent, 5, 9, 31, 36, pause_frames, fire_frames,
                    CTFWeapon_Grapple_Fire );

    // if we just switched back to grapple, immediately go to fire frame
    if ( prevstate == WEAPON_ACTIVATING &&
         ent->client->weaponstate == WEAPON_READY &&
         ent->client->ctf_grapplestate > CTF_GRAPPLE_STATE_FLY )
    {
        if ( !( ent->client->buttons & BUTTON_ATTACK ) )
            ent->client->ps.gunframe = 9;
        else
            ent->client->ps.gunframe = 5;
        ent->client->weaponstate = WEAPON_FIRING;
    }
}

void CTFTeam_f( edict* ent )
{
    char *t, *s;
    int desired_team;

    t = quake2::getInstance()->gi.args();
    if ( !*t )
    {
        quake2::getInstance()->gi.cprintf(
            ent, PRINT_HIGH, "You are on the %s team.\n",
            CTFTeamName( ent->client->resp.ctf_team ) );
        return;
    }

    if ( ctfgame.match > MATCH_SETUP )
    {
        quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH,
                                           "Can't change teams in a match.\n" );
        return;
    }

    if ( Q_stricmp( t, "red" ) == 0 )
        desired_team = CTF_TEAM1;
    else if ( Q_stricmp( t, "blue" ) == 0 )
        desired_team = CTF_TEAM2;
    else
    {
        quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH,
                                           "Unknown team %s.\n", t );
        return;
    }

    if ( ent->client->resp.ctf_team == desired_team )
    {
        quake2::getInstance()->gi.cprintf(
            ent, PRINT_HIGH, "You are already on the %s team.\n",
            CTFTeamName( ent->client->resp.ctf_team ) );
        return;
    }

    ////
    ent->svflags = 0;
    ent->flags &= ~FL_GODMODE;
    ent->client->resp.ctf_team = desired_team;
    ent->client->resp.ctf_state = 0;
    s = Info_ValueForKey( ent->client->pers.userinfo, "skin" );
    CTFAssignSkin( ent, s );

    if ( ent->solid == SOLID_NOT )
    {  // spectator
        PutClientInServer( ent );
        // add a teleportation effect
        ent->s.event = EV_PLAYER_TELEPORT;
        // hold in place briefly
        ent->client->ps.pmove.pm_flags = PMF_TIME_TELEPORT;
        ent->client->ps.pmove.pm_time = 14;
        quake2::getInstance()->gi.bprintf(
            PRINT_HIGH, "%s joined the %s team.\n", ent->client->pers.netname,
            CTFTeamName( desired_team ) );
        return;
    }

    ent->health = 0;
    player_die( ent, ent, ent, 100000, vec3_origin );
    // don't even bother waiting for death frames
    ent->deadflag = DEAD_DEAD;
    respawn( ent );

    ent->client->resp.score = 0;

    quake2::getInstance()->gi.bprintf(
        PRINT_HIGH, "%s changed to the %s team.\n", ent->client->pers.netname,
        CTFTeamName( desired_team ) );
}

/*
==================
CTFScoreboardMessage
==================
*/
void CTFScoreboardMessage( edict* ent, edict* killer )
{
    char entry[1024];
    char string[1400];
    int len;
    int i, j, k, n;
    int sorted[2][MAX_CLIENTS];
    int sortedscores[2][MAX_CLIENTS];
    int score, total[2], totalscore[2];
    int last[2];
    gclient_t* cl;
    edict* cl_ent;
    int team;
    int maxsize = 1000;

    // sort the clients by team and score
    total[0] = total[1] = 0;
    last[0] = last[1] = 0;
    totalscore[0] = totalscore[1] = 0;
    for ( i = 0; i < game.maxclients; i++ )
    {
        cl_ent = g_edicts + 1 + i;
        if ( !cl_ent->inuse )
            continue;
        if ( game.clients[i].resp.ctf_team == CTF_TEAM1 )
            team = 0;
        else if ( game.clients[i].resp.ctf_team == CTF_TEAM2 )
            team = 1;
        else
            continue;  // unknown team?

        score = game.clients[i].resp.score;
        for ( j = 0; j < total[team]; j++ )
        {
            if ( score > sortedscores[team][j] )
                break;
        }
        for ( k = total[team]; k > j; k-- )
        {
            sorted[team][k] = sorted[team][k - 1];
            sortedscores[team][k] = sortedscores[team][k - 1];
        }
        sorted[team][j] = i;
        sortedscores[team][j] = score;
        totalscore[team] += score;
        total[team]++;
    }

    // print level name and exit rules
    // add the clients in sorted order
    *string = 0;
    len = 0;

    // team one
    sprintf( string,
             "if 24 xv 8 yv 8 pic 24 endif "
             "xv 40 yv 28 string \"%4d/%-3d\" "
             "xv 98 yv 12 num 2 18 "
             "if 25 xv 168 yv 8 pic 25 endif "
             "xv 200 yv 28 string \"%4d/%-3d\" "
             "xv 256 yv 12 num 2 20 ",
             totalscore[0], total[0], totalscore[1], total[1] );
    len = strlen( string );

    for ( i = 0; i < 16; i++ )
    {
        if ( i >= total[0] && i >= total[1] )
            break;  // we're done

#if 0  // ndef NEW_SCORE
       // set up y
        sprintf(entry, "yv %d ", 42 + i * 8);
        if (maxsize - len > strlen(entry)) {
            strcat(string, entry);
            len = strlen(string);
        }
#else
        *entry = 0;
#endif

        // left side
        if ( i < total[0] )
        {
            cl = &game.clients[sorted[0][i]];
            cl_ent = g_edicts + 1 + sorted[0][i];

#if 0  // ndef NEW_SCORE
            sprintf(entry+strlen(entry),
            "xv 0 %s \"%3d %3d %-12.12s\" ",
            (cl_ent == ent) ? "string2" : "string",
            cl->resp.score,
            (cl->ping > 999) ? 999 : cl->ping,
            cl->pers.netname);

            if (cl_ent->client->pers.inventory[ITEM_INDEX(flag2_item)])
                strcat(entry, "xv 56 picn sbfctf2 ");
#else
            sprintf( entry + strlen( entry ), "ctf 0 %d %d %d %d ", 42 + i * 8,
                     sorted[0][i], cl->resp.score,
                     cl->ping > 999 ? 999 : cl->ping );

            if ( cl_ent->client->pers.inventory[ITEM_INDEX( flag2_item )] )
                sprintf( entry + strlen( entry ), "xv 56 yv %d picn sbfctf2 ",
                         42 + i * 8 );
#endif

            if ( maxsize - len > strlen( entry ) )
            {
                strcat( string, entry );
                len = strlen( string );
                last[0] = i;
            }
        }

        // right side
        if ( i < total[1] )
        {
            cl = &game.clients[sorted[1][i]];
            cl_ent = g_edicts + 1 + sorted[1][i];

#if 0  // ndef NEW_SCORE
            sprintf(entry+strlen(entry),
            "xv 160 %s \"%3d %3d %-12.12s\" ",
            (cl_ent == ent) ? "string2" : "string",
            cl->resp.score,
            (cl->ping > 999) ? 999 : cl->ping,
            cl->pers.netname);

            if (cl_ent->client->pers.inventory[ITEM_INDEX(flag1_item)])
                strcat(entry, "xv 216 picn sbfctf1 ");

#else

            sprintf( entry + strlen( entry ), "ctf 160 %d %d %d %d ",
                     42 + i * 8, sorted[1][i], cl->resp.score,
                     cl->ping > 999 ? 999 : cl->ping );

            if ( cl_ent->client->pers.inventory[ITEM_INDEX( flag1_item )] )
                sprintf( entry + strlen( entry ), "xv 216 yv %d picn sbfctf1 ",
                         42 + i * 8 );
#endif
            if ( maxsize - len > strlen( entry ) )
            {
                strcat( string, entry );
                len = strlen( string );
                last[1] = i;
            }
        }
    }

    // put in spectators if we have enough room
    if ( last[0] > last[1] )
        j = last[0];
    else
        j = last[1];
    j = ( j + 2 ) * 8 + 42;

    k = n = 0;
    if ( maxsize - len > 50 )
    {
        for ( i = 0; i < maxclients->value; i++ )
        {
            cl_ent = g_edicts + 1 + i;
            cl = &game.clients[i];
            if ( !cl_ent->inuse || cl_ent->solid != SOLID_NOT ||
                 cl_ent->client->resp.ctf_team != CTF_NOTEAM )
                continue;

            if ( !k )
            {
                k = 1;
                sprintf( entry, "xv 0 yv %d string2 \"Spectators\" ", j );
                strcat( string, entry );
                len = strlen( string );
                j += 8;
            }

            sprintf( entry + strlen( entry ), "ctf %d %d %d %d %d ",
                     ( n & 1 ) ? 160 : 0,  // x
                     j,                    // y
                     i,                    // playernum
                     cl->resp.score, cl->ping > 999 ? 999 : cl->ping );
            if ( maxsize - len > strlen( entry ) )
            {
                strcat( string, entry );
                len = strlen( string );
            }

            if ( n & 1 )
                j += 8;
            n++;
        }
    }

    if ( total[0] - last[0] > 1 )  // couldn't fit everyone
        sprintf( string + strlen( string ),
                 "xv 8 yv %d string \"..and %d more\" ",
                 42 + ( last[0] + 1 ) * 8, total[0] - last[0] - 1 );
    if ( total[1] - last[1] > 1 )  // couldn't fit everyone
        sprintf( string + strlen( string ),
                 "xv 168 yv %d string \"..and %d more\" ",
                 42 + ( last[1] + 1 ) * 8, total[1] - last[1] - 1 );

    quake2::getInstance()->gi.WriteByte( svc_layout );
    quake2::getInstance()->gi.WriteString( string );
}

/*------------------------------------------------------------------------*/
/* TECH                                                                      */
/*------------------------------------------------------------------------*/

void CTFHasTech( edict* who )
{
    if ( level.time - who->client->ctf_lasttechmsg > 2 )
    {
        quake2::getInstance()->gi.centerprintf(
            who, "You already have a TECH powerup." );
        who->client->ctf_lasttechmsg = level.time;
    }
}

gitem* CTFWhat_Tech( edict* ent )
{
    gitem* tech;
    int i;

    i = 0;
    while ( tnames[i] )
    {
        if ( ( tech = FindItemByClassname( tnames[i] ) ) != NULL &&
             ent->client->pers.inventory[ITEM_INDEX( tech )] )
        {
            return tech;
        }
        i++;
    }
    return NULL;
}

bool CTFPickup_Tech( edict* ent, edict* other )
{
    gitem* tech;
    int i;

    i = 0;
    while ( tnames[i] )
    {
        if ( ( tech = FindItemByClassname( tnames[i] ) ) != NULL &&
             other->client->pers.inventory[ITEM_INDEX( tech )] )
        {
            CTFHasTech( other );
            return false;  // has this one
        }
        i++;
    }

    // client only gets one tech
    other->client->pers.inventory[ITEM_INDEX( ent->item )]++;
    other->client->ctf_regentime = level.time;
    return true;
}

static void SpawnTech( gitem* item, edict* spot );

static edict* FindTechSpawn( void )
{
    edict* spot = NULL;
    int i = rand() % 16;

    while ( i-- )
        spot = G_Find( spot, FOFS( classname ), "info_player_deathmatch" );
    if ( !spot )
        spot = G_Find( spot, FOFS( classname ), "info_player_deathmatch" );
    return spot;
}

static void TechThink( edict* tech )
{
    edict* spot;

    if ( ( spot = FindTechSpawn() ) != NULL )
    {
        SpawnTech( tech->item, spot );
        G_FreeEdict( tech );
    }
    else
    {
        tech->nextthink = level.time + CTF_TECH_TIMEOUT;
        tech->think = TechThink;
    }
}

void CTFDrop_Tech( edict* ent, gitem* item )
{
    edict* tech;

    tech = Drop_Item( ent, item );
    tech->nextthink = level.time + CTF_TECH_TIMEOUT;
    tech->think = TechThink;
    ent->client->pers.inventory[ITEM_INDEX( item )] = 0;
}

void CTFDeadDropTech( edict* ent )
{
    gitem* tech;
    edict* dropped;
    int i;

    i = 0;
    while ( tnames[i] )
    {
        if ( ( tech = FindItemByClassname( tnames[i] ) ) != NULL &&
             ent->client->pers.inventory[ITEM_INDEX( tech )] )
        {
            dropped = Drop_Item( ent, tech );
            // hack the velocity to make it bounce random
            dropped->velocity[0] = ( rand() % 600 ) - 300;
            dropped->velocity[1] = ( rand() % 600 ) - 300;
            dropped->nextthink = level.time + CTF_TECH_TIMEOUT;
            dropped->think = TechThink;
            dropped->owner = NULL;
            ent->client->pers.inventory[ITEM_INDEX( tech )] = 0;
        }
        i++;
    }
}

static void SpawnTech( gitem* item, edict* spot )
{
    edict* ent;
    vec3_t forward, right;
    vec3_t angles;

    ent = G_Spawn();

    ent->classname = item->classname;
    ent->item = item;
    ent->spawnflags = DROPPED_ITEM;
    ent->s.effects = item->world_model_flags;
    ent->s.renderfx = RF_GLOW;
    VectorSet( ent->mins, -15, -15, -15 );
    VectorSet( ent->maxs, 15, 15, 15 );
    quake2::getInstance()->gi.setmodel( ent, ent->item->world_model );
    ent->solid = SOLID_TRIGGER;
    ent->movetype = MOVETYPE_TOSS;
    ent->touch = Touch_Item;
    ent->owner = ent;

    angles[0] = 0;
    angles[1] = rand() % 360;
    angles[2] = 0;

    AngleVectors( angles, forward, right, NULL );
    VectorCopy( spot->s.origin, ent->s.origin );
    ent->s.origin[2] += 16;
    VectorScale( forward, 100, ent->velocity );
    ent->velocity[2] = 300;

    ent->nextthink = level.time + CTF_TECH_TIMEOUT;
    ent->think = TechThink;

    quake2::getInstance()->gi.linkentity( ent );
}

static void SpawnTechs( edict* ent )
{
    gitem* tech;
    edict* spot;
    int i;

    i = 0;
    while ( tnames[i] )
    {
        if ( ( tech = FindItemByClassname( tnames[i] ) ) != NULL &&
             ( spot = FindTechSpawn() ) != NULL )
            SpawnTech( tech, spot );
        i++;
    }
    if ( ent )
        G_FreeEdict( ent );
}

// frees the passed edict!
void CTFRespawnTech( edict* ent )
{
    edict* spot;

    if ( ( spot = FindTechSpawn() ) != NULL )
        SpawnTech( ent->item, spot );
    G_FreeEdict( ent );
}

void CTFSetupTechSpawn( void )
{
    edict* ent;

    if ( ( (int)dmflags->value & DF_CTF_NO_TECH ) )
        return;

    ent = G_Spawn();
    ent->nextthink = level.time + 2;
    ent->think = SpawnTechs;
}

void CTFResetTech( void )
{
    edict* ent;
    int i;

    for ( ent = g_edicts + 1, i = 1; i < globals.num_edicts; i++, ent++ )
    {
        if ( ent->inuse )
            if ( ent->item && ( ent->item->flags & IT_TECH ) )
                G_FreeEdict( ent );
    }
    SpawnTechs( NULL );
}

int CTFApplyResistance( edict* ent, int dmg )
{
    static gitem* tech = NULL;
    float volume = 1.0;

    if ( ent->client && ent->client->silencer_shots )
        volume = 0.2;

    if ( !tech )
        tech = FindItemByClassname( "item_tech1" );
    if ( dmg && tech && ent->client &&
         ent->client->pers.inventory[ITEM_INDEX( tech )] )
    {
        // make noise
        quake2::getInstance()->gi.sound(
            ent, CHAN_VOICE,
            quake2::getInstance()->gi.soundindex( "ctf/tech1.wav" ), volume,
            ATTN_NORM, 0 );
        return dmg / 2;
    }
    return dmg;
}

int CTFApplyStrength( edict* ent, int dmg )
{
    static gitem* tech = NULL;

    if ( !tech )
        tech = FindItemByClassname( "item_tech2" );
    if ( dmg && tech && ent->client &&
         ent->client->pers.inventory[ITEM_INDEX( tech )] )
    {
        return dmg * 2;
    }
    return dmg;
}

bool CTFApplyStrengthSound( edict* ent )
{
    static gitem* tech = NULL;
    float volume = 1.0;

    if ( ent->client && ent->client->silencer_shots )
        volume = 0.2;

    if ( !tech )
        tech = FindItemByClassname( "item_tech2" );
    if ( tech && ent->client &&
         ent->client->pers.inventory[ITEM_INDEX( tech )] )
    {
        if ( ent->client->ctf_techsndtime < level.time )
        {
            ent->client->ctf_techsndtime = level.time + 1;
            if ( ent->client->quad_framenum > level.framenum )
                quake2::getInstance()->gi.sound(
                    ent, CHAN_VOICE,
                    quake2::getInstance()->gi.soundindex( "ctf/tech2x.wav" ),
                    volume, ATTN_NORM, 0 );
            else
                quake2::getInstance()->gi.sound(
                    ent, CHAN_VOICE,
                    quake2::getInstance()->gi.soundindex( "ctf/tech2.wav" ),
                    volume, ATTN_NORM, 0 );
        }
        return true;
    }
    return false;
}

bool CTFApplyHaste( edict* ent )
{
    static gitem* tech = NULL;

    if ( !tech )
        tech = FindItemByClassname( "item_tech3" );
    if ( tech && ent->client &&
         ent->client->pers.inventory[ITEM_INDEX( tech )] )
        return true;
    return false;
}

void CTFApplyHasteSound( edict* ent )
{
    static gitem* tech = NULL;
    float volume = 1.0;

    if ( ent->client && ent->client->silencer_shots )
        volume = 0.2;

    if ( !tech )
        tech = FindItemByClassname( "item_tech3" );
    if ( tech && ent->client &&
         ent->client->pers.inventory[ITEM_INDEX( tech )] &&
         ent->client->ctf_techsndtime < level.time )
    {
        ent->client->ctf_techsndtime = level.time + 1;
        quake2::getInstance()->gi.sound(
            ent, CHAN_VOICE,
            quake2::getInstance()->gi.soundindex( "ctf/tech3.wav" ), volume,
            ATTN_NORM, 0 );
    }
}

void CTFApplyRegeneration( edict* ent )
{
    static gitem* tech = NULL;
    bool noise = false;
    gclient_t* client;
    int index;
    float volume = 1.0;

    client = ent->client;
    if ( !client )
        return;

    if ( ent->client->silencer_shots )
        volume = 0.2;

    if ( !tech )
        tech = FindItemByClassname( "item_tech4" );
    if ( tech && client->pers.inventory[ITEM_INDEX( tech )] )
    {
        if ( client->ctf_regentime < level.time )
        {
            client->ctf_regentime = level.time;
            if ( ent->health < 150 )
            {
                ent->health += 5;
                if ( ent->health > 150 )
                    ent->health = 150;
                client->ctf_regentime += 0.5;
                noise = true;
            }
            index = ArmorIndex( ent );
            if ( index && client->pers.inventory[index] < 150 )
            {
                client->pers.inventory[index] += 5;
                if ( client->pers.inventory[index] > 150 )
                    client->pers.inventory[index] = 150;
                client->ctf_regentime += 0.5;
                noise = true;
            }
        }
        if ( noise && ent->client->ctf_techsndtime < level.time )
        {
            ent->client->ctf_techsndtime = level.time + 1;
            quake2::getInstance()->gi.sound(
                ent, CHAN_VOICE,
                quake2::getInstance()->gi.soundindex( "ctf/tech4.wav" ), volume,
                ATTN_NORM, 0 );
        }
    }
}

bool CTFHasRegeneration( edict* ent )
{
    static gitem* tech = NULL;

    if ( !tech )
        tech = FindItemByClassname( "item_tech4" );
    if ( tech && ent->client &&
         ent->client->pers.inventory[ITEM_INDEX( tech )] )
        return true;
    return false;
}

/*
======================================================================

SAY_TEAM

======================================================================
*/

// This array is in 'importance order', it indicates what items are
// more important when reporting their names.
struct
{
    char* classname;
    int priority;
} loc_names[] = { { "item_flag_team1", 1 },
                  { "item_flag_team2", 1 },
                  { "item_quad", 2 },
                  { "item_invulnerability", 2 },
                  { "weapon_bfg", 3 },
                  { "weapon_railgun", 4 },
                  { "weapon_rocketlauncher", 4 },
                  { "weapon_hyperblaster", 4 },
                  { "weapon_chaingun", 4 },
                  { "weapon_grenadelauncher", 4 },
                  { "weapon_machinegun", 4 },
                  { "weapon_supershotgun", 4 },
                  { "weapon_shotgun", 4 },
                  { "item_power_screen", 5 },
                  { "item_power_shield", 5 },
                  { "item_armor_body", 6 },
                  { "item_armor_combat", 6 },
                  { "item_armor_jacket", 6 },
                  { "item_silencer", 7 },
                  { "item_breather", 7 },
                  { "item_enviro", 7 },
                  { "item_adrenaline", 7 },
                  { "item_bandolier", 8 },
                  { "item_pack", 8 },
                  { NULL, 0 } };

static void CTFSay_Team_Location( edict* who, char* buf )
{
    edict* what = NULL;
    edict* hot = NULL;
    float hotdist = 999999, newdist;
    vec3_t v;
    int hotindex = 999;
    int i;
    gitem* item;
    int nearteam = -1;
    edict *flag1, *flag2;
    bool hotsee = false;
    bool cansee;

    while ( ( what = loc_findradius( what, who->s.origin, 1024 ) ) != NULL )
    {
        // find what in loc_classnames
        for ( i = 0; loc_names[i].classname; i++ )
            if ( strcmp( what->classname, loc_names[i].classname ) == 0 )
                break;
        if ( !loc_names[i].classname )
            continue;
        // something we can see get priority over something we can't
        cansee = loc_CanSee( what, who );
        if ( cansee && !hotsee )
        {
            hotsee = true;
            hotindex = loc_names[i].priority;
            hot = what;
            VectorSubtract( what->s.origin, who->s.origin, v );
            hotdist = VectorLength( v );
            continue;
        }
        // if we can't see this, but we have something we can see, skip it
        if ( hotsee && !cansee )
            continue;
        if ( hotsee && hotindex < loc_names[i].priority )
            continue;
        VectorSubtract( what->s.origin, who->s.origin, v );
        newdist = VectorLength( v );
        if ( newdist < hotdist ||
             ( cansee && loc_names[i].priority < hotindex ) )
        {
            hot = what;
            hotdist = newdist;
            hotindex = i;
            hotsee = loc_CanSee( hot, who );
        }
    }

    if ( !hot )
    {
        strcpy( buf, "nowhere" );
        return;
    }

    // we now have the closest item
    // see if there's more than one in the map, if so
    // we need to determine what team is closest
    what = NULL;
    while ( ( what = G_Find( what, FOFS( classname ), hot->classname ) ) !=
            NULL )
    {
        if ( what == hot )
            continue;
        // if we are here, there is more than one, find out if hot
        // is closer to red flag or blue flag
        if ( ( flag1 = G_Find( NULL, FOFS( classname ), "item_flag_team1" ) ) !=
                 NULL &&
             ( flag2 = G_Find( NULL, FOFS( classname ), "item_flag_team2" ) ) !=
                 NULL )
        {
            VectorSubtract( hot->s.origin, flag1->s.origin, v );
            hotdist = VectorLength( v );
            VectorSubtract( hot->s.origin, flag2->s.origin, v );
            newdist = VectorLength( v );
            if ( hotdist < newdist )
                nearteam = CTF_TEAM1;
            else if ( hotdist > newdist )
                nearteam = CTF_TEAM2;
        }
        break;
    }

    if ( ( item = FindItemByClassname( hot->classname ) ) == NULL )
    {
        strcpy( buf, "nowhere" );
        return;
    }

    // in water?
    if ( who->waterlevel )
        strcpy( buf, "in the water " );
    else
        *buf = 0;

    // near or above
    VectorSubtract( who->s.origin, hot->s.origin, v );
    if ( fabs( v[2] ) > fabs( v[0] ) && fabs( v[2] ) > fabs( v[1] ) )
        if ( v[2] > 0 )
            strcat( buf, "above " );
        else
            strcat( buf, "below " );
    else
        strcat( buf, "near " );

    if ( nearteam == CTF_TEAM1 )
        strcat( buf, "the red " );
    else if ( nearteam == CTF_TEAM2 )
        strcat( buf, "the blue " );
    else
        strcat( buf, "the " );

    strcat( buf, item->pickup_name );
}

static void CTFSay_Team_Armor( edict* who, char* buf )
{
    gitem* item;
    int index, cells;
    int power_armor_type;

    *buf = 0;

    power_armor_type = PowerArmorType( who );
    if ( power_armor_type )
    {
        cells = who->client->pers.inventory[ITEM_INDEX( FindItem( "cells" ) )];
        if ( cells )
            sprintf( buf + strlen( buf ), "%s with %i cells ",
                     ( power_armor_type == POWER_ARMOR_SCREEN )
                         ? "Power Screen"
                         : "Power Shield",
                     cells );
    }

    index = ArmorIndex( who );
    if ( index )
    {
        item = GetItemByIndex( index );
        if ( item )
        {
            if ( *buf )
                strcat( buf, "and " );
            sprintf( buf + strlen( buf ), "%i units of %s",
                     who->client->pers.inventory[index], item->pickup_name );
        }
    }

    if ( !*buf )
        strcpy( buf, "no armor" );
}

static void CTFSay_Team_Health( edict* who, char* buf )
{
    if ( who->health <= 0 )
        strcpy( buf, "dead" );
    else
        sprintf( buf, "%i health", who->health );
}

static void CTFSay_Team_Tech( edict* who, char* buf )
{
    gitem* tech;
    int i;

    // see if the player has a tech powerup
    i = 0;
    while ( tnames[i] )
    {
        if ( ( tech = FindItemByClassname( tnames[i] ) ) != NULL &&
             who->client->pers.inventory[ITEM_INDEX( tech )] )
        {
            sprintf( buf, "the %s", tech->pickup_name );
            return;
        }
        i++;
    }
    strcpy( buf, "no powerup" );
}

static void CTFSay_Team_Weapon( edict* who, char* buf )
{
    if ( who->client->pers.weapon )
        strcpy( buf, who->client->pers.weapon->pickup_name );
    else
        strcpy( buf, "none" );
}

static void CTFSay_Team_Sight( edict* who, char* buf )
{
    int i;
    edict* targ;
    int n = 0;
    char s[1024];
    char s2[1024];

    *s = *s2 = 0;
    for ( i = 1; i <= maxclients->value; i++ )
    {
        targ = g_edicts + i;
        if ( !targ->inuse || targ == who || !loc_CanSee( targ, who ) )
            continue;
        if ( *s2 )
        {
            if ( strlen( s ) + strlen( s2 ) + 3 < sizeof( s ) )
            {
                if ( n )
                    strcat( s, ", " );
                strcat( s, s2 );
                *s2 = 0;
            }
            n++;
        }
        strcpy( s2, targ->client->pers.netname );
    }
    if ( *s2 )
    {
        if ( strlen( s ) + strlen( s2 ) + 6 < sizeof( s ) )
        {
            if ( n )
                strcat( s, " and " );
            strcat( s, s2 );
        }
        strcpy( buf, s );
    }
    else
        strcpy( buf, "no one" );
}

void CTFSay_Team( edict* who, char* msg )
{
    char outmsg[1024];
    char buf[1024];
    int i;
    char* p;
    edict* cl_ent;

    if ( CheckFlood( who ) )
        return;

    outmsg[0] = 0;

    if ( *msg == '\"' )
    {
        msg[strlen( msg ) - 1] = 0;
        msg++;
    }

    for ( p = outmsg; *msg && ( p - outmsg ) < sizeof( outmsg ) - 1; msg++ )
    {
        if ( *msg == '%' )
        {
            switch ( *++msg )
            {
                case 'l':
                case 'L':
                    CTFSay_Team_Location( who, buf );
                    strcpy( p, buf );
                    p += strlen( buf );
                    break;
                case 'a':
                case 'A':
                    CTFSay_Team_Armor( who, buf );
                    strcpy( p, buf );
                    p += strlen( buf );
                    break;
                case 'h':
                case 'H':
                    CTFSay_Team_Health( who, buf );
                    strcpy( p, buf );
                    p += strlen( buf );
                    break;
                case 't':
                case 'T':
                    CTFSay_Team_Tech( who, buf );
                    strcpy( p, buf );
                    p += strlen( buf );
                    break;
                case 'w':
                case 'W':
                    CTFSay_Team_Weapon( who, buf );
                    strcpy( p, buf );
                    p += strlen( buf );
                    break;

                case 'n':
                case 'N':
                    CTFSay_Team_Sight( who, buf );
                    strcpy( p, buf );
                    p += strlen( buf );
                    break;

                default:
                    *p++ = *msg;
            }
        }
        else
            *p++ = *msg;
    }
    *p = 0;

    for ( i = 0; i < maxclients->value; i++ )
    {
        cl_ent = g_edicts + 1 + i;
        if ( !cl_ent->inuse )
            continue;
        if ( cl_ent->client->resp.ctf_team == who->client->resp.ctf_team )
            quake2::getInstance()->gi.cprintf( cl_ent, PRINT_CHAT, "(%s): %s\n",
                                               who->client->pers.netname,
                                               outmsg );
    }
}

/*-----------------------------------------------------------------------*/
/*QUAKED misc_ctf_banner (1 .5 0) (-4 -64 0) (4 64 248) TEAM2
The origin is the bottom of the banner.
The banner is 248 tall.
*/
static void misc_ctf_banner_think( edict* ent )
{
    ent->s.frame = ( ent->s.frame + 1 ) % 16;
    ent->nextthink = level.time + FRAMETIME;
}

void SP_misc_ctf_banner( edict* ent )
{
    ent->movetype = MOVETYPE_NONE;
    ent->solid = SOLID_NOT;
    ent->s.modelindex =
        quake2::getInstance()->gi.modelindex( "models/ctf/banner/tris.md2" );
    if ( ent->spawnflags & 1 )  // team2
        ent->s.skinnum = 1;

    ent->s.frame = rand() % 16;
    quake2::getInstance()->gi.linkentity( ent );

    ent->think = misc_ctf_banner_think;
    ent->nextthink = level.time + FRAMETIME;
}

/*QUAKED misc_ctf_small_banner (1 .5 0) (-4 -32 0) (4 32 124) TEAM2
The origin is the bottom of the banner.
The banner is 124 tall.
*/
void SP_misc_ctf_small_banner( edict* ent )
{
    ent->movetype = MOVETYPE_NONE;
    ent->solid = SOLID_NOT;
    ent->s.modelindex =
        quake2::getInstance()->gi.modelindex( "models/ctf/banner/small.md2" );
    if ( ent->spawnflags & 1 )  // team2
        ent->s.skinnum = 1;

    ent->s.frame = rand() % 16;
    quake2::getInstance()->gi.linkentity( ent );

    ent->think = misc_ctf_banner_think;
    ent->nextthink = level.time + FRAMETIME;
}

/*-----------------------------------------------------------------------*/

static void SetLevelName( pmenu_t* p )
{
    static char levelname[33];

    levelname[0] = '*';
    if ( g_edicts[0].message )
        strncpy( levelname + 1, g_edicts[0].message, sizeof( levelname ) - 2 );
    else
        strncpy( levelname + 1, level.mapname, sizeof( levelname ) - 2 );
    levelname[sizeof( levelname ) - 1] = 0;
    p->text = levelname;
}

/*-----------------------------------------------------------------------*/

/* ELECTIONS */

bool CTFBeginElection( edict* ent, elect_t type, char* msg )
{
    int i;
    int count;
    edict* e;

    if ( electpercentage->value == 0 )
    {
        quake2::getInstance()->gi.cprintf(
            ent, PRINT_HIGH,
            "Elections are disabled, only an admin can process this "
            "action.\n" );
        return false;
    }

    if ( ctfgame.election != ELECT_NONE )
    {
        quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH,
                                           "Election already in progress.\n" );
        return false;
    }

    // clear votes
    count = 0;
    for ( i = 1; i <= maxclients->value; i++ )
    {
        e = g_edicts + i;
        e->client->resp.voted = false;
        if ( e->inuse )
            count++;
    }

    if ( count < 2 )
    {
        quake2::getInstance()->gi.cprintf(
            ent, PRINT_HIGH, "Not enough players for election.\n" );
        return false;
    }

    ctfgame.etarget = ent;
    ctfgame.election = type;
    ctfgame.evotes = 0;
    ctfgame.needvotes = ( count * electpercentage->value ) / 100;
    ctfgame.electtime = level.time + 20;  // twenty seconds for election
    strncpy( ctfgame.emsg, msg, sizeof( ctfgame.emsg ) - 1 );

    // tell everyone
    quake2::getInstance()->gi.bprintf( PRINT_CHAT, "%s\n", ctfgame.emsg );
    quake2::getInstance()->gi.bprintf(
        PRINT_HIGH, "Type YES or NO to vote on this request.\n" );
    quake2::getInstance()->gi.bprintf(
        PRINT_HIGH, "Votes: %d  Needed: %d  Time left: %ds\n", ctfgame.evotes,
        ctfgame.needvotes, (int)( ctfgame.electtime - level.time ) );

    return true;
}

void DoRespawn( edict* ent );

void CTFResetAllPlayers( void )
{
    int i;
    edict* ent;

    for ( i = 1; i <= maxclients->value; i++ )
    {
        ent = g_edicts + i;
        if ( !ent->inuse )
            continue;

        if ( ent->client->menu )
            PMenu_Close( ent );

        CTFPlayerResetGrapple( ent );
        CTFDeadDropFlag( ent );
        CTFDeadDropTech( ent );

        ent->client->resp.ctf_team = CTF_NOTEAM;
        ent->client->resp.ready = false;

        ent->svflags = 0;
        ent->flags &= ~FL_GODMODE;
        PutClientInServer( ent );
    }

    // reset the level
    CTFResetTech();
    CTFResetFlags();

    for ( ent = g_edicts + 1, i = 1; i < globals.num_edicts; i++, ent++ )
    {
        if ( ent->inuse && !ent->client )
        {
            if ( ent->solid == SOLID_NOT && ent->think == DoRespawn &&
                 ent->nextthink >= level.time )
            {
                ent->nextthink = 0;
                DoRespawn( ent );
            }
        }
    }
    if ( ctfgame.match == MATCH_SETUP )
        ctfgame.matchtime = level.time + matchsetuptime->value * 60;
}

void CTFAssignGhost( edict* ent )
{
    int ghost, i;

    for ( ghost = 0; ghost < MAX_CLIENTS; ghost++ )
        if ( !ctfgame.ghosts[ghost].code )
            break;
    if ( ghost == MAX_CLIENTS )
        return;
    ctfgame.ghosts[ghost].team = ent->client->resp.ctf_team;
    ctfgame.ghosts[ghost].score = 0;
    for ( ;; )
    {
        ctfgame.ghosts[ghost].code = 10000 + ( rand() % 90000 );
        for ( i = 0; i < MAX_CLIENTS; i++ )
            if ( i != ghost &&
                 ctfgame.ghosts[i].code == ctfgame.ghosts[ghost].code )
                break;
        if ( i == MAX_CLIENTS )
            break;
    }
    ctfgame.ghosts[ghost].ent = ent;
    strcpy( ctfgame.ghosts[ghost].netname, ent->client->pers.netname );
    ent->client->resp.ghost = ctfgame.ghosts + ghost;
    quake2::getInstance()->gi.cprintf( ent, PRINT_CHAT,
                                       "Your ghost code is **** %d ****\n",
                                       ctfgame.ghosts[ghost].code );
    quake2::getInstance()->gi.cprintf(
        ent, PRINT_HIGH,
        "If you lose connection, you can rejoin with your score "
        "intact by typing \"ghost %d\".\n",
        ctfgame.ghosts[ghost].code );
}

// start a match
void CTFStartMatch( void )
{
    int i;
    edict* ent;
    int ghost = 0;

    ctfgame.match = MATCH_GAME;
    ctfgame.matchtime = level.time + matchtime->value * 60;

    ctfgame.team1 = ctfgame.team2 = 0;

    memset( ctfgame.ghosts, 0, sizeof( ctfgame.ghosts ) );

    for ( i = 1; i <= maxclients->value; i++ )
    {
        ent = g_edicts + i;
        if ( !ent->inuse )
            continue;

        ent->client->resp.score = 0;
        ent->client->resp.ctf_state = 0;
        ent->client->resp.ghost = NULL;

        quake2::getInstance()->gi.centerprintf(
            ent,
            "******************\n\nMATCH HAS STARTED!\n\n******************" );

        if ( ent->client->resp.ctf_team != CTF_NOTEAM )
        {
            // make up a ghost code
            CTFAssignGhost( ent );
            CTFPlayerResetGrapple( ent );
            ent->svflags = SVF_NOCLIENT;
            ent->flags &= ~FL_GODMODE;

            ent->client->respawn_time =
                level.time + 1.0 + ( ( rand() % 30 ) / 10.0 );
            ent->client->ps.pmove.pm_type = pmtype_t::PM_DEAD;
            ent->client->anim_priority = ANIM_DEATH;
            ent->s.frame = FRAME_death308 - 1;
            ent->client->anim_end = FRAME_death308;
            ent->deadflag = DEAD_DEAD;
            ent->movetype = MOVETYPE_NOCLIP;
            ent->client->ps.gunindex = 0;
            quake2::getInstance()->gi.linkentity( ent );
        }
    }
}

void CTFEndMatch( void )
{
    ctfgame.match = MATCH_POST;
    quake2::getInstance()->gi.bprintf( PRINT_CHAT, "MATCH COMPLETED!\n" );

    CTFCalcScores();

    quake2::getInstance()->gi.bprintf( PRINT_HIGH,
                                       "RED TEAM:  %d captures, %d points\n",
                                       ctfgame.team1, ctfgame.total1 );
    quake2::getInstance()->gi.bprintf( PRINT_HIGH,
                                       "BLUE TEAM:  %d captures, %d points\n",
                                       ctfgame.team2, ctfgame.total2 );

    if ( ctfgame.team1 > ctfgame.team2 )
        quake2::getInstance()->gi.bprintf(
            PRINT_CHAT, "RED team won over the BLUE team by %d CAPTURES!\n",
            ctfgame.team1 - ctfgame.team2 );
    else if ( ctfgame.team2 > ctfgame.team1 )
        quake2::getInstance()->gi.bprintf(
            PRINT_CHAT, "BLUE team won over the RED team by %d CAPTURES!\n",
            ctfgame.team2 - ctfgame.team1 );
    else if ( ctfgame.total1 > ctfgame.total2 )  // frag tie breaker
        quake2::getInstance()->gi.bprintf(
            PRINT_CHAT, "RED team won over the BLUE team by %d POINTS!\n",
            ctfgame.total1 - ctfgame.total2 );
    else if ( ctfgame.total2 > ctfgame.total1 )
        quake2::getInstance()->gi.bprintf(
            PRINT_CHAT, "BLUE team won over the RED team by %d POINTS!\n",
            ctfgame.total2 - ctfgame.total1 );
    else
        quake2::getInstance()->gi.bprintf( PRINT_CHAT, "TIE GAME!\n" );

    EndDMLevel();
}

bool CTFNextMap( void )
{
    if ( ctfgame.match == MATCH_POST )
    {
        ctfgame.match = MATCH_SETUP;
        CTFResetAllPlayers();
        return true;
    }
    return false;
}

void CTFWinElection( void )
{
    switch ( ctfgame.election )
    {
        case ELECT_MATCH:
            // reset into match mode
            if ( competition->value < 3 )
                quake2::getInstance()->gi.cvar_set( "competition", "2" );
            ctfgame.match = MATCH_SETUP;
            CTFResetAllPlayers();
            break;

        case ELECT_ADMIN:
            ctfgame.etarget->client->resp.admin = true;
            quake2::getInstance()->gi.bprintf(
                PRINT_HIGH, "%s has become an admin.\n",
                ctfgame.etarget->client->pers.netname );
            quake2::getInstance()->gi.cprintf(
                ctfgame.etarget, PRINT_HIGH,
                "Type 'admin' to access the adminstration menu.\n" );
            break;

        case ELECT_MAP:
            quake2::getInstance()->gi.bprintf(
                PRINT_HIGH, "%s is warping to level %s.\n",
                ctfgame.etarget->client->pers.netname, ctfgame.elevel );
            strncpy( level.forcemap, ctfgame.elevel,
                     sizeof( level.forcemap ) - 1 );
            EndDMLevel();
            break;
    }
    ctfgame.election = ELECT_NONE;
}

void CTFVoteYes( edict* ent )
{
    if ( ctfgame.election == ELECT_NONE )
    {
        quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH,
                                           "No election is in progress.\n" );
        return;
    }
    if ( ent->client->resp.voted )
    {
        quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH,
                                           "You already voted.\n" );
        return;
    }
    if ( ctfgame.etarget == ent )
    {
        quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH,
                                           "You can't vote for yourself.\n" );
        return;
    }

    ent->client->resp.voted = true;

    ctfgame.evotes++;
    if ( ctfgame.evotes == ctfgame.needvotes )
    {
        // the election has been won
        CTFWinElection();
        return;
    }
    quake2::getInstance()->gi.bprintf( PRINT_HIGH, "%s\n", ctfgame.emsg );
    quake2::getInstance()->gi.bprintf(
        PRINT_CHAT, "Votes: %d  Needed: %d  Time left: %ds\n", ctfgame.evotes,
        ctfgame.needvotes, (int)( ctfgame.electtime - level.time ) );
}

void CTFVoteNo( edict* ent )
{
    if ( ctfgame.election == ELECT_NONE )
    {
        quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH,
                                           "No election is in progress.\n" );
        return;
    }
    if ( ent->client->resp.voted )
    {
        quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH,
                                           "You already voted.\n" );
        return;
    }
    if ( ctfgame.etarget == ent )
    {
        quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH,
                                           "You can't vote for yourself.\n" );
        return;
    }

    ent->client->resp.voted = true;

    quake2::getInstance()->gi.bprintf( PRINT_HIGH, "%s\n", ctfgame.emsg );
    quake2::getInstance()->gi.bprintf(
        PRINT_CHAT, "Votes: %d  Needed: %d  Time left: %ds\n", ctfgame.evotes,
        ctfgame.needvotes, (int)( ctfgame.electtime - level.time ) );
}

void CTFReady( edict* ent )
{
    int i, j;
    edict* e;
    int t1, t2;

    if ( ent->client->resp.ctf_team == CTF_NOTEAM )
    {
        quake2::getInstance()->gi.cprintf(
            ent, PRINT_HIGH, "Pick a team first (hit <TAB> for menu)\n" );
        return;
    }

    if ( ctfgame.match != MATCH_SETUP )
    {
        quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH,
                                           "A match is not being setup.\n" );
        return;
    }

    if ( ent->client->resp.ready )
    {
        quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH,
                                           "You have already commited.\n" );
        return;
    }

    ent->client->resp.ready = true;
    quake2::getInstance()->gi.bprintf( PRINT_HIGH, "%s is ready.\n",
                                       ent->client->pers.netname );

    t1 = t2 = 0;
    for ( j = 0, i = 1; i <= maxclients->value; i++ )
    {
        e = g_edicts + i;
        if ( !e->inuse )
            continue;
        if ( e->client->resp.ctf_team != CTF_NOTEAM && !e->client->resp.ready )
            j++;
        if ( e->client->resp.ctf_team == CTF_TEAM1 )
            t1++;
        else if ( e->client->resp.ctf_team == CTF_TEAM2 )
            t2++;
    }
    if ( !j && t1 && t2 )
    {
        // everyone has commited
        quake2::getInstance()->gi.bprintf(
            PRINT_CHAT, "All players have commited.  Match starting\n" );
        ctfgame.match = MATCH_PREGAME;
        ctfgame.matchtime = level.time + matchstarttime->value;
    }
}

void CTFNotReady( edict* ent )
{
    if ( ent->client->resp.ctf_team == CTF_NOTEAM )
    {
        quake2::getInstance()->gi.cprintf(
            ent, PRINT_HIGH, "Pick a team first (hit <TAB> for menu)\n" );
        return;
    }

    if ( ctfgame.match != MATCH_SETUP && ctfgame.match != MATCH_PREGAME )
    {
        quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH,
                                           "A match is not being setup.\n" );
        return;
    }

    if ( !ent->client->resp.ready )
    {
        quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH,
                                           "You haven't commited.\n" );
        return;
    }

    ent->client->resp.ready = false;
    quake2::getInstance()->gi.bprintf( PRINT_HIGH, "%s is no longer ready.\n",
                                       ent->client->pers.netname );

    if ( ctfgame.match == MATCH_PREGAME )
    {
        quake2::getInstance()->gi.bprintf( PRINT_CHAT, "Match halted.\n" );
        ctfgame.match = MATCH_SETUP;
        ctfgame.matchtime = level.time + matchsetuptime->value * 60;
    }
}

void CTFGhost( edict* ent )
{
    int i;
    int n;

    if ( quake2::getInstance()->gi.argc() < 2 )
    {
        quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH,
                                           "Usage:  ghost <code>\n" );
        return;
    }

    if ( ent->client->resp.ctf_team != CTF_NOTEAM )
    {
        quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH,
                                           "You are already in the game.\n" );
        return;
    }
    if ( ctfgame.match != MATCH_GAME )
    {
        quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH,
                                           "No match is in progress.\n" );
        return;
    }

    n = atoi( quake2::getInstance()->gi.argv( 1 ) );

    for ( i = 0; i < MAX_CLIENTS; i++ )
    {
        if ( ctfgame.ghosts[i].code && ctfgame.ghosts[i].code == n )
        {
            quake2::getInstance()->gi.cprintf(
                ent, PRINT_HIGH,
                "Ghost code accepted, your position has been reinstated.\n" );
            ctfgame.ghosts[i].ent->client->resp.ghost = NULL;
            ent->client->resp.ctf_team = ctfgame.ghosts[i].team;
            ent->client->resp.ghost = ctfgame.ghosts + i;
            ent->client->resp.score = ctfgame.ghosts[i].score;
            ent->client->resp.ctf_state = 0;
            ctfgame.ghosts[i].ent = ent;
            ent->svflags = 0;
            ent->flags &= ~FL_GODMODE;
            PutClientInServer( ent );
            quake2::getInstance()->gi.bprintf(
                PRINT_HIGH, "%s has been reinstated to %s team.\n",
                ent->client->pers.netname,
                CTFTeamName( ent->client->resp.ctf_team ) );
            return;
        }
    }
    quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH,
                                       "Invalid ghost code.\n" );
}

bool CTFMatchSetup( void )
{
    if ( ctfgame.match == MATCH_SETUP || ctfgame.match == MATCH_PREGAME )
        return true;
    return false;
}

bool CTFMatchOn( void )
{
    if ( ctfgame.match == MATCH_GAME )
        return true;
    return false;
}

/*-----------------------------------------------------------------------*/

void CTFJoinTeam1( edict* ent, pmenuhnd_t* p );
void CTFJoinTeam2( edict* ent, pmenuhnd_t* p );
void CTFCredits( edict* ent, pmenuhnd_t* p );
void CTFReturnToMain( edict* ent, pmenuhnd_t* p );
void CTFChaseCam( edict* ent, pmenuhnd_t* p );

pmenu_t creditsmenu[] = {
    { "*Quake II", PMENU_ALIGN_CENTER, NULL },
    { "*ThreeWave Capture the Flag", PMENU_ALIGN_CENTER, NULL },
    { NULL, PMENU_ALIGN_CENTER, NULL },
    { "*Programming", PMENU_ALIGN_CENTER, NULL },
    { "Dave 'Zoid' Kirsch", PMENU_ALIGN_CENTER, NULL },
    { "*Level Design", PMENU_ALIGN_CENTER, NULL },
    { "Christian Antkow", PMENU_ALIGN_CENTER, NULL },
    { "Tim Willits", PMENU_ALIGN_CENTER, NULL },
    { "Dave 'Zoid' Kirsch", PMENU_ALIGN_CENTER, NULL },
    { "*Art", PMENU_ALIGN_CENTER, NULL },
    { "Adrian Carmack Paul Steed", PMENU_ALIGN_CENTER, NULL },
    { "Kevin Cloud", PMENU_ALIGN_CENTER, NULL },
    { "*Sound", PMENU_ALIGN_CENTER, NULL },
    { "Tom 'Bjorn' Klok", PMENU_ALIGN_CENTER, NULL },
    { "*Original CTF Art Design", PMENU_ALIGN_CENTER, NULL },
    { "Brian 'Whaleboy' Cozzens", PMENU_ALIGN_CENTER, NULL },
    { NULL, PMENU_ALIGN_CENTER, NULL },
    { "Return to Main Menu", PMENU_ALIGN_LEFT, CTFReturnToMain } };

static const int jmenu_level = 2;
static const int jmenu_match = 3;
static const int jmenu_red = 5;
static const int jmenu_blue = 7;
static const int jmenu_chase = 9;
static const int jmenu_reqmatch = 11;

pmenu_t joinmenu[] = {
    { "*Quake II", PMENU_ALIGN_CENTER, NULL },
    { "*ThreeWave Capture the Flag", PMENU_ALIGN_CENTER, NULL },
    { NULL, PMENU_ALIGN_CENTER, NULL },
    { NULL, PMENU_ALIGN_CENTER, NULL },
    { NULL, PMENU_ALIGN_CENTER, NULL },
    { "Join Red Team", PMENU_ALIGN_LEFT, CTFJoinTeam1 },
    { NULL, PMENU_ALIGN_LEFT, NULL },
    { "Join Blue Team", PMENU_ALIGN_LEFT, CTFJoinTeam2 },
    { NULL, PMENU_ALIGN_LEFT, NULL },
    { "Chase Camera", PMENU_ALIGN_LEFT, CTFChaseCam },
    { "Credits", PMENU_ALIGN_LEFT, CTFCredits },
    { NULL, PMENU_ALIGN_LEFT, NULL },
    { NULL, PMENU_ALIGN_LEFT, NULL },
    { "Use [ and ] to move cursor", PMENU_ALIGN_LEFT, NULL },
    { "ENTER to select", PMENU_ALIGN_LEFT, NULL },
    { "ESC to Exit Menu", PMENU_ALIGN_LEFT, NULL },
    { "(TAB to Return)", PMENU_ALIGN_LEFT, NULL },
    { "v" CTF_STRING_VERSION, PMENU_ALIGN_RIGHT, NULL },
};

pmenu_t nochasemenu[] = {
    { "*Quake II", PMENU_ALIGN_CENTER, NULL },
    { "*ThreeWave Capture the Flag", PMENU_ALIGN_CENTER, NULL },
    { NULL, PMENU_ALIGN_CENTER, NULL },
    { NULL, PMENU_ALIGN_CENTER, NULL },
    { "No one to chase", PMENU_ALIGN_LEFT, NULL },
    { NULL, PMENU_ALIGN_CENTER, NULL },
    { "Return to Main Menu", PMENU_ALIGN_LEFT, CTFReturnToMain } };

void CTFJoinTeam( edict* ent, int desired_team )
{
    char* s;

    PMenu_Close( ent );

    ent->svflags &= ~SVF_NOCLIENT;
    ent->client->resp.ctf_team = desired_team;
    ent->client->resp.ctf_state = 0;
    s = Info_ValueForKey( ent->client->pers.userinfo, "skin" );
    CTFAssignSkin( ent, s );

    // assign a ghost if we are in match mode
    if ( ctfgame.match == MATCH_GAME )
    {
        if ( ent->client->resp.ghost )
            ent->client->resp.ghost->code = 0;
        ent->client->resp.ghost = NULL;
        CTFAssignGhost( ent );
    }

    PutClientInServer( ent );
    // add a teleportation effect
    ent->s.event = EV_PLAYER_TELEPORT;
    // hold in place briefly
    ent->client->ps.pmove.pm_flags = PMF_TIME_TELEPORT;
    ent->client->ps.pmove.pm_time = 14;
    quake2::getInstance()->gi.bprintf( PRINT_HIGH, "%s joined the %s team.\n",
                                       ent->client->pers.netname,
                                       CTFTeamName( desired_team ) );

    if ( ctfgame.match == MATCH_SETUP )
    {
        quake2::getInstance()->gi.centerprintf( ent,
                                                "***********************\n"
                                                "Type \"ready\" in console\n"
                                                "to ready up.\n"
                                                "***********************" );
    }
}

void CTFJoinTeam1( edict* ent, pmenuhnd_t* p )
{
    CTFJoinTeam( ent, CTF_TEAM1 );
}

void CTFJoinTeam2( edict* ent, pmenuhnd_t* p )
{
    CTFJoinTeam( ent, CTF_TEAM2 );
}

void CTFChaseCam( edict* ent, pmenuhnd_t* p )
{
    int i;
    edict* e;

    if ( ent->client->chase_target )
    {
        ent->client->chase_target = NULL;
        PMenu_Close( ent );
        return;
    }

    for ( i = 1; i <= maxclients->value; i++ )
    {
        e = g_edicts + i;
        if ( e->inuse && e->solid != SOLID_NOT )
        {
            ent->client->chase_target = e;
            PMenu_Close( ent );
            ent->client->update_chase = true;
            return;
        }
    }

    SetLevelName( nochasemenu + jmenu_level );

    PMenu_Close( ent );
    PMenu_Open( ent, nochasemenu, -1, sizeof( nochasemenu ) / sizeof( pmenu_t ),
                NULL );
}

void CTFReturnToMain( edict* ent, pmenuhnd_t* p )
{
    PMenu_Close( ent );
    CTFOpenJoinMenu( ent );
}

void CTFRequestMatch( edict* ent, pmenuhnd_t* p )
{
    char text[1024];

    PMenu_Close( ent );

    sprintf( text, "%s has requested to switch to competition mode.",
             ent->client->pers.netname );
    CTFBeginElection( ent, ELECT_MATCH, text );
}

void DeathmatchScoreboard( edict* ent );

void CTFShowScores( edict* ent, pmenu_t* p )
{
    PMenu_Close( ent );

    ent->client->showscores = true;
    ent->client->showinventory = false;
    DeathmatchScoreboard( ent );
}

int CTFUpdateJoinMenu( edict* ent )
{
    static char team1players[32];
    static char team2players[32];
    int num1, num2, i;

    if ( ctfgame.match >= MATCH_PREGAME && matchlock->value )
    {
        joinmenu[jmenu_red].text = "MATCH IS LOCKED";
        joinmenu[jmenu_red].SelectFunc = NULL;
        joinmenu[jmenu_blue].text = "  (entry is not permitted)";
        joinmenu[jmenu_blue].SelectFunc = NULL;
    }
    else
    {
        if ( ctfgame.match >= MATCH_PREGAME )
        {
            joinmenu[jmenu_red].text = "Join Red MATCH Team";
            joinmenu[jmenu_blue].text = "Join Blue MATCH Team";
        }
        else
        {
            joinmenu[jmenu_red].text = "Join Red Team";
            joinmenu[jmenu_blue].text = "Join Blue Team";
        }
        joinmenu[jmenu_red].SelectFunc = CTFJoinTeam1;
        joinmenu[jmenu_blue].SelectFunc = CTFJoinTeam2;
    }

    if ( ctf_forcejoin->string && *ctf_forcejoin->string )
    {
        if ( stricmp( ctf_forcejoin->string, "red" ) == 0 )
        {
            joinmenu[jmenu_blue].text = NULL;
            joinmenu[jmenu_blue].SelectFunc = NULL;
        }
        else if ( stricmp( ctf_forcejoin->string, "blue" ) == 0 )
        {
            joinmenu[jmenu_red].text = NULL;
            joinmenu[jmenu_red].SelectFunc = NULL;
        }
    }

    if ( ent->client->chase_target )
        joinmenu[jmenu_chase].text = "Leave Chase Camera";
    else
        joinmenu[jmenu_chase].text = "Chase Camera";

    SetLevelName( joinmenu + jmenu_level );

    num1 = num2 = 0;
    for ( i = 0; i < maxclients->value; i++ )
    {
        if ( !g_edicts[i + 1].inuse )
            continue;
        if ( game.clients[i].resp.ctf_team == CTF_TEAM1 )
            num1++;
        else if ( game.clients[i].resp.ctf_team == CTF_TEAM2 )
            num2++;
    }

    sprintf( team1players, "  (%d players)", num1 );
    sprintf( team2players, "  (%d players)", num2 );

    switch ( ctfgame.match )
    {
        case MATCH_NONE:
            joinmenu[jmenu_match].text = NULL;
            break;

        case MATCH_SETUP:
            joinmenu[jmenu_match].text = "*MATCH SETUP IN PROGRESS";
            break;

        case MATCH_PREGAME:
            joinmenu[jmenu_match].text = "*MATCH STARTING";
            break;

        case MATCH_GAME:
            joinmenu[jmenu_match].text = "*MATCH IN PROGRESS";
            break;
    }

    if ( joinmenu[jmenu_red].text )
        joinmenu[jmenu_red + 1].text = team1players;
    else
        joinmenu[jmenu_red + 1].text = NULL;
    if ( joinmenu[jmenu_blue].text )
        joinmenu[jmenu_blue + 1].text = team2players;
    else
        joinmenu[jmenu_blue + 1].text = NULL;

    joinmenu[jmenu_reqmatch].text = NULL;
    joinmenu[jmenu_reqmatch].SelectFunc = NULL;
    if ( competition->value && ctfgame.match < MATCH_SETUP )
    {
        joinmenu[jmenu_reqmatch].text = "Request Match";
        joinmenu[jmenu_reqmatch].SelectFunc = CTFRequestMatch;
    }

    if ( num1 > num2 )
        return CTF_TEAM1;
    else if ( num2 > num1 )
        return CTF_TEAM2;
    return ( rand() & 1 ) ? CTF_TEAM1 : CTF_TEAM2;
}

void CTFOpenJoinMenu( edict* ent )
{
    int team;

    team = CTFUpdateJoinMenu( ent );
    if ( ent->client->chase_target )
        team = 8;
    else if ( team == CTF_TEAM1 )
        team = 4;
    else
        team = 6;
    PMenu_Open( ent, joinmenu, team, sizeof( joinmenu ) / sizeof( pmenu_t ),
                NULL );
}

void CTFCredits( edict* ent, pmenuhnd_t* p )
{
    PMenu_Close( ent );
    PMenu_Open( ent, creditsmenu, -1, sizeof( creditsmenu ) / sizeof( pmenu_t ),
                NULL );
}

bool CTFStartClient( edict* ent )
{
    if ( ent->client->resp.ctf_team != CTF_NOTEAM )
        return false;

    if ( !( (int)dmflags->value & DF_CTF_FORCEJOIN ) ||
         ctfgame.match >= MATCH_SETUP )
    {
        // start as 'observer'
        ent->movetype = MOVETYPE_NOCLIP;
        ent->solid = SOLID_NOT;
        ent->svflags |= SVF_NOCLIENT;
        ent->client->resp.ctf_team = CTF_NOTEAM;
        ent->client->ps.gunindex = 0;
        quake2::getInstance()->gi.linkentity( ent );

        CTFOpenJoinMenu( ent );
        return true;
    }
    return false;
}

void CTFObserver( edict* ent )
{
    // start as 'observer'
    if ( ent->movetype == MOVETYPE_NOCLIP )
    {
        quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH,
                                           "You are already an observer.\n" );
        return;
    }

    CTFPlayerResetGrapple( ent );
    CTFDeadDropFlag( ent );
    CTFDeadDropTech( ent );

    ent->movetype = MOVETYPE_NOCLIP;
    ent->solid = SOLID_NOT;
    ent->svflags |= SVF_NOCLIENT;
    ent->client->resp.ctf_team = CTF_NOTEAM;
    ent->client->ps.gunindex = 0;
    ent->client->resp.score = 0;
    quake2::getInstance()->gi.linkentity( ent );
    CTFOpenJoinMenu( ent );
}

bool CTFInMatch( void )
{
    if ( ctfgame.match > MATCH_NONE )
        return true;
    return false;
}

bool CTFCheckRules( void )
{
    int t;
    int i, j;
    char text[64];
    edict* ent;

    if ( ctfgame.election != ELECT_NONE && ctfgame.electtime <= level.time )
    {
        quake2::getInstance()->gi.bprintf(
            PRINT_CHAT, "Election timed out and has been cancelled.\n" );
        ctfgame.election = ELECT_NONE;
    }

    if ( ctfgame.match != MATCH_NONE )
    {
        t = ctfgame.matchtime - level.time;

        if ( t <= 0 )
        {  // time ended on something
            switch ( ctfgame.match )
            {
                case MATCH_SETUP:
                    // go back to normal mode
                    if ( competition->value < 3 )
                    {
                        ctfgame.match = MATCH_NONE;
                        quake2::getInstance()->gi.cvar_set( "competition",
                                                            "1" );
                        CTFResetAllPlayers();
                    }
                    else
                    {
                        // reset the time
                        ctfgame.matchtime =
                            level.time + matchsetuptime->value * 60;
                    }
                    return false;

                case MATCH_PREGAME:
                    // match started!
                    CTFStartMatch();
                    return false;

                case MATCH_GAME:
                    // match ended!
                    CTFEndMatch();
                    return false;
            }
        }

        if ( t == ctfgame.lasttime )
            return false;

        ctfgame.lasttime = t;

        switch ( ctfgame.match )
        {
            case MATCH_SETUP:
                for ( j = 0, i = 1; i <= maxclients->value; i++ )
                {
                    ent = g_edicts + i;
                    if ( !ent->inuse )
                        continue;
                    if ( ent->client->resp.ctf_team != CTF_NOTEAM &&
                         !ent->client->resp.ready )
                        j++;
                }

                if ( competition->value < 3 )
                    sprintf( text, "%02d:%02d SETUP: %d not ready", t / 60,
                             t % 60, j );
                else
                    sprintf( text, "SETUP: %d not ready", j );

                quake2::getInstance()->gi.configstring( CONFIG_CTF_MATCH,
                                                        text );
                break;

            case MATCH_PREGAME:
                sprintf( text, "%02d:%02d UNTIL START", t / 60, t % 60 );
                quake2::getInstance()->gi.configstring( CONFIG_CTF_MATCH,
                                                        text );
                break;

            case MATCH_GAME:
                sprintf( text, "%02d:%02d MATCH", t / 60, t % 60 );
                quake2::getInstance()->gi.configstring( CONFIG_CTF_MATCH,
                                                        text );
                break;
        }
        return false;
    }

    if ( capturelimit->value && ( ctfgame.team1 >= capturelimit->value ||
                                  ctfgame.team2 >= capturelimit->value ) )
    {
        quake2::getInstance()->gi.bprintf( PRINT_HIGH, "Capturelimit hit.\n" );
        return true;
    }
    return false;
}

/*--------------------------------------------------------------------------
 * just here to help old map conversions
 *--------------------------------------------------------------------------*/

static void old_teleporter_touch( edict* self, edict* other, plane_s* plane,
                                  csurface_s* surf )
{
    edict* dest;
    int i;
    vec3_t forward;

    if ( !other->client )
        return;
    dest = G_Find( NULL, FOFS( targetname ), self->target );
    if ( !dest )
    {
        quake2::getInstance()->gi.dprintf( "Couldn't find destination\n" );
        return;
    }

    // ZOID
    CTFPlayerResetGrapple( other );
    // ZOID

    // unlink to make sure it can't possibly interfere with KillBox
    quake2::getInstance()->gi.unlinkentity( other );

    VectorCopy( dest->s.origin, other->s.origin );
    VectorCopy( dest->s.origin, other->s.old_origin );
    //    other->s.origin[2] += 10;

    // clear the velocity and hold them in place briefly
    VectorClear( other->velocity );
    other->client->ps.pmove.pm_time = 160 >> 3;  // hold time
    other->client->ps.pmove.pm_flags |= PMF_TIME_TELEPORT;

    // draw the teleport splash at source and on the player
    self->enemy->s.event = EV_PLAYER_TELEPORT;
    other->s.event = EV_PLAYER_TELEPORT;

    // set angles
    for ( i = 0; i < 3; i++ )
        other->client->ps.pmove.delta_angles[i] = ANGLE2SHORT(
            dest->s.angles[i] - other->client->resp.cmd_angles[i] );

    other->s.angles[PITCH] = 0;
    other->s.angles[YAW] = dest->s.angles[YAW];
    other->s.angles[ROLL] = 0;
    VectorCopy( dest->s.angles, other->client->ps.viewangles );
    VectorCopy( dest->s.angles, other->client->v_angle );

    // give a little forward velocity
    AngleVectors( other->client->v_angle, forward, NULL, NULL );
    VectorScale( forward, 200, other->velocity );

    // kill anything at the destination
    if ( !KillBox( other ) )
    {
    }

    quake2::getInstance()->gi.linkentity( other );
}

/*QUAKED trigger_teleport (0.5 0.5 0.5) ?
Players touching this will be teleported
*/
void SP_trigger_teleport( edict* ent )
{
    edict* s;
    int i;

    if ( !ent->target )
    {
        quake2::getInstance()->gi.dprintf( "teleporter without a target.\n" );
        G_FreeEdict( ent );
        return;
    }

    ent->svflags |= SVF_NOCLIENT;
    ent->solid = SOLID_TRIGGER;
    ent->touch = old_teleporter_touch;
    quake2::getInstance()->gi.setmodel( ent, ent->model );
    quake2::getInstance()->gi.linkentity( ent );

    // noise maker and splash effect dude
    s = G_Spawn();
    ent->enemy = s;
    for ( i = 0; i < 3; i++ )
        s->s.origin[i] = ent->mins[i] + ( ent->maxs[i] - ent->mins[i] ) / 2;
    s->s.sound = quake2::getInstance()->gi.soundindex( "world/hum1.wav" );
    quake2::getInstance()->gi.linkentity( s );
}

/*QUAKED info_teleport_destination (0.5 0.5 0.5) (-16 -16 -24) (16 16 32)
Point trigger_teleports at these.
*/
void SP_info_teleport_destination( edict* ent ) { ent->s.origin[2] += 16; }

/*----------------------------------------------------------------------------------*/
/* ADMIN */

typedef struct admin_settings_s
{
    int matchlen;
    int matchsetuplen;
    int matchstartlen;
    bool weaponsstay;
    bool instantitems;
    bool quaddrop;
    bool instantweap;
    bool matchlock;
} admin_settings_t;

#define SETMENU_SIZE ( 7 + 5 )

void CTFAdmin_UpdateSettings( edict* ent, pmenuhnd_t* setmenu );
void CTFOpenAdminMenu( edict* ent );

void CTFAdmin_SettingsApply( edict* ent, pmenuhnd_t* p )
{
    admin_settings_t* settings = p->arg;
    char st[80];
    int i;

    if ( settings->matchlen != matchtime->value )
    {
        quake2::getInstance()->gi.bprintf(
            PRINT_HIGH, "%s changed the match length to %d minutes.\n",
            ent->client->pers.netname, settings->matchlen );
        if ( ctfgame.match == MATCH_GAME )
        {
            // in the middle of a match, change it on the fly
            ctfgame.matchtime = ( ctfgame.matchtime - matchtime->value * 60 ) +
                                settings->matchlen * 60;
        }
        sprintf( st, "%d", settings->matchlen );
        quake2::getInstance()->gi.cvar_set( "matchtime", st );
    }

    if ( settings->matchsetuplen != matchsetuptime->value )
    {
        quake2::getInstance()->gi.bprintf(
            PRINT_HIGH, "%s changed the match setup time to %d minutes.\n",
            ent->client->pers.netname, settings->matchsetuplen );
        if ( ctfgame.match == MATCH_SETUP )
        {
            // in the middle of a match, change it on the fly
            ctfgame.matchtime =
                ( ctfgame.matchtime - matchsetuptime->value * 60 ) +
                settings->matchsetuplen * 60;
        }
        sprintf( st, "%d", settings->matchsetuplen );
        quake2::getInstance()->gi.cvar_set( "matchsetuptime", st );
    }

    if ( settings->matchstartlen != matchstarttime->value )
    {
        quake2::getInstance()->gi.bprintf(
            PRINT_HIGH, "%s changed the match start time to %d seconds.\n",
            ent->client->pers.netname, settings->matchstartlen );
        if ( ctfgame.match == MATCH_PREGAME )
        {
            // in the middle of a match, change it on the fly
            ctfgame.matchtime = ( ctfgame.matchtime - matchstarttime->value ) +
                                settings->matchstartlen;
        }
        sprintf( st, "%d", settings->matchstartlen );
        quake2::getInstance()->gi.cvar_set( "matchstarttime", st );
    }

    if ( settings->weaponsstay != !!( (int)dmflags->value & DF_WEAPONS_STAY ) )
    {
        quake2::getInstance()->gi.bprintf(
            PRINT_HIGH, "%s turned %s weapons stay.\n",
            ent->client->pers.netname, settings->weaponsstay ? "on" : "off" );
        i = (int)dmflags->value;
        if ( settings->weaponsstay )
            i |= DF_WEAPONS_STAY;
        else
            i &= ~DF_WEAPONS_STAY;
        sprintf( st, "%d", i );
        quake2::getInstance()->gi.cvar_set( "dmflags", st );
    }

    if ( settings->instantitems !=
         !!( (int)dmflags->value & DF_INSTANT_ITEMS ) )
    {
        quake2::getInstance()->gi.bprintf(
            PRINT_HIGH, "%s turned %s instant items.\n",
            ent->client->pers.netname, settings->instantitems ? "on" : "off" );
        i = (int)dmflags->value;
        if ( settings->instantitems )
            i |= DF_INSTANT_ITEMS;
        else
            i &= ~DF_INSTANT_ITEMS;
        sprintf( st, "%d", i );
        quake2::getInstance()->gi.cvar_set( "dmflags", st );
    }

    if ( settings->quaddrop != !!( (int)dmflags->value & DF_QUAD_DROP ) )
    {
        quake2::getInstance()->gi.bprintf(
            PRINT_HIGH, "%s turned %s quad drop.\n", ent->client->pers.netname,
            settings->quaddrop ? "on" : "off" );
        i = (int)dmflags->value;
        if ( settings->quaddrop )
            i |= DF_QUAD_DROP;
        else
            i &= ~DF_QUAD_DROP;
        sprintf( st, "%d", i );
        quake2::getInstance()->gi.cvar_set( "dmflags", st );
    }

    if ( settings->instantweap != !!( (int)instantweap->value ) )
    {
        quake2::getInstance()->gi.bprintf(
            PRINT_HIGH, "%s turned %s instant weapons.\n",
            ent->client->pers.netname, settings->instantweap ? "on" : "off" );
        sprintf( st, "%d", (int)settings->instantweap );
        quake2::getInstance()->gi.cvar_set( "instantweap", st );
    }

    if ( settings->matchlock != !!( (int)matchlock->value ) )
    {
        quake2::getInstance()->gi.bprintf(
            PRINT_HIGH, "%s turned %s match lock.\n", ent->client->pers.netname,
            settings->matchlock ? "on" : "off" );
        sprintf( st, "%d", (int)settings->matchlock );
        quake2::getInstance()->gi.cvar_set( "matchlock", st );
    }

    PMenu_Close( ent );
    CTFOpenAdminMenu( ent );
}

void CTFAdmin_SettingsCancel( edict* ent, pmenuhnd_t* p )
{
    admin_settings_t* settings = p->arg;

    PMenu_Close( ent );
    CTFOpenAdminMenu( ent );
}

void CTFAdmin_ChangeMatchLen( edict* ent, pmenuhnd_t* p )
{
    admin_settings_t* settings = p->arg;

    settings->matchlen = ( settings->matchlen % 60 ) + 5;
    if ( settings->matchlen < 5 )
        settings->matchlen = 5;

    CTFAdmin_UpdateSettings( ent, p );
}

void CTFAdmin_ChangeMatchSetupLen( edict* ent, pmenuhnd_t* p )
{
    admin_settings_t* settings = p->arg;

    settings->matchsetuplen = ( settings->matchsetuplen % 60 ) + 5;
    if ( settings->matchsetuplen < 5 )
        settings->matchsetuplen = 5;

    CTFAdmin_UpdateSettings( ent, p );
}

void CTFAdmin_ChangeMatchStartLen( edict* ent, pmenuhnd_t* p )
{
    admin_settings_t* settings = p->arg;

    settings->matchstartlen = ( settings->matchstartlen % 600 ) + 10;
    if ( settings->matchstartlen < 20 )
        settings->matchstartlen = 20;

    CTFAdmin_UpdateSettings( ent, p );
}

void CTFAdmin_ChangeWeapStay( edict* ent, pmenuhnd_t* p )
{
    admin_settings_t* settings = p->arg;

    settings->weaponsstay = !settings->weaponsstay;
    CTFAdmin_UpdateSettings( ent, p );
}

void CTFAdmin_ChangeInstantItems( edict* ent, pmenuhnd_t* p )
{
    admin_settings_t* settings = p->arg;

    settings->instantitems = !settings->instantitems;
    CTFAdmin_UpdateSettings( ent, p );
}

void CTFAdmin_ChangeQuadDrop( edict* ent, pmenuhnd_t* p )
{
    admin_settings_t* settings = p->arg;

    settings->quaddrop = !settings->quaddrop;
    CTFAdmin_UpdateSettings( ent, p );
}

void CTFAdmin_ChangeInstantWeap( edict* ent, pmenuhnd_t* p )
{
    admin_settings_t* settings = p->arg;

    settings->instantweap = !settings->instantweap;
    CTFAdmin_UpdateSettings( ent, p );
}

void CTFAdmin_ChangeMatchLock( edict* ent, pmenuhnd_t* p )
{
    admin_settings_t* settings = p->arg;

    settings->matchlock = !settings->matchlock;
    CTFAdmin_UpdateSettings( ent, p );
}

void CTFAdmin_UpdateSettings( edict* ent, pmenuhnd_t* setmenu )
{
    int i = 2;
    char text[64];
    admin_settings_t* settings = setmenu->arg;

    sprintf( text, "Match Len:       %2d mins", settings->matchlen );
    PMenu_UpdateEntry( setmenu->entries + i, text, PMENU_ALIGN_LEFT,
                       CTFAdmin_ChangeMatchLen );
    i++;

    sprintf( text, "Match Setup Len: %2d mins", settings->matchsetuplen );
    PMenu_UpdateEntry( setmenu->entries + i, text, PMENU_ALIGN_LEFT,
                       CTFAdmin_ChangeMatchSetupLen );
    i++;

    sprintf( text, "Match Start Len: %2d secs", settings->matchstartlen );
    PMenu_UpdateEntry( setmenu->entries + i, text, PMENU_ALIGN_LEFT,
                       CTFAdmin_ChangeMatchStartLen );
    i++;

    sprintf( text, "Weapons Stay:    %s",
             settings->weaponsstay ? "Yes" : "No" );
    PMenu_UpdateEntry( setmenu->entries + i, text, PMENU_ALIGN_LEFT,
                       CTFAdmin_ChangeWeapStay );
    i++;

    sprintf( text, "Instant Items:   %s",
             settings->instantitems ? "Yes" : "No" );
    PMenu_UpdateEntry( setmenu->entries + i, text, PMENU_ALIGN_LEFT,
                       CTFAdmin_ChangeInstantItems );
    i++;

    sprintf( text, "Quad Drop:       %s", settings->quaddrop ? "Yes" : "No" );
    PMenu_UpdateEntry( setmenu->entries + i, text, PMENU_ALIGN_LEFT,
                       CTFAdmin_ChangeQuadDrop );
    i++;

    sprintf( text, "Instant Weapons: %s",
             settings->instantweap ? "Yes" : "No" );
    PMenu_UpdateEntry( setmenu->entries + i, text, PMENU_ALIGN_LEFT,
                       CTFAdmin_ChangeInstantWeap );
    i++;

    sprintf( text, "Match Lock:      %s", settings->matchlock ? "Yes" : "No" );
    PMenu_UpdateEntry( setmenu->entries + i, text, PMENU_ALIGN_LEFT,
                       CTFAdmin_ChangeMatchLock );
    i++;

    PMenu_Update( ent );
}

pmenu_t def_setmenu[] = {
    { "*Settings Menu", PMENU_ALIGN_CENTER, NULL },
    { NULL, PMENU_ALIGN_CENTER, NULL },
    { NULL, PMENU_ALIGN_LEFT, NULL },  // int matchlen;
    { NULL, PMENU_ALIGN_LEFT, NULL },  // int matchsetuplen;
    { NULL, PMENU_ALIGN_LEFT, NULL },  // int matchstartlen;
    { NULL, PMENU_ALIGN_LEFT, NULL },  // bool weaponsstay;
    { NULL, PMENU_ALIGN_LEFT, NULL },  // bool instantitems;
    { NULL, PMENU_ALIGN_LEFT, NULL },  // bool quaddrop;
    { NULL, PMENU_ALIGN_LEFT, NULL },  // bool instantweap;
    { NULL, PMENU_ALIGN_LEFT, NULL },  // bool matchlock;
    { NULL, PMENU_ALIGN_LEFT, NULL },
    { "Apply", PMENU_ALIGN_LEFT, CTFAdmin_SettingsApply },
    { "Cancel", PMENU_ALIGN_LEFT, CTFAdmin_SettingsCancel } };

void CTFAdmin_Settings( edict* ent, pmenuhnd_t* p )
{
    admin_settings_t* settings;
    pmenuhnd_t* menu;

    PMenu_Close( ent );

    settings = malloc( sizeof( *settings ) );

    settings->matchlen = matchtime->value;
    settings->matchsetuplen = matchsetuptime->value;
    settings->matchstartlen = matchstarttime->value;
    settings->weaponsstay = !!( (int)dmflags->value & DF_WEAPONS_STAY );
    settings->instantitems = !!( (int)dmflags->value & DF_INSTANT_ITEMS );
    settings->quaddrop = !!( (int)dmflags->value & DF_QUAD_DROP );
    settings->instantweap = instantweap->value != 0;
    settings->matchlock = matchlock->value != 0;

    menu = PMenu_Open( ent, def_setmenu, -1,
                       sizeof( def_setmenu ) / sizeof( pmenu_t ), settings );
    CTFAdmin_UpdateSettings( ent, menu );
}

void CTFAdmin_MatchSet( edict* ent, pmenuhnd_t* p )
{
    PMenu_Close( ent );

    if ( ctfgame.match == MATCH_SETUP )
    {
        quake2::getInstance()->gi.bprintf(
            PRINT_CHAT, "Match has been forced to start.\n" );
        ctfgame.match = MATCH_PREGAME;
        ctfgame.matchtime = level.time + matchstarttime->value;
    }
    else if ( ctfgame.match == MATCH_GAME )
    {
        quake2::getInstance()->gi.bprintf(
            PRINT_CHAT, "Match has been forced to terminate.\n" );
        ctfgame.match = MATCH_SETUP;
        ctfgame.matchtime = level.time + matchsetuptime->value * 60;
        CTFResetAllPlayers();
    }
}

void CTFAdmin_MatchMode( edict* ent, pmenuhnd_t* p )
{
    PMenu_Close( ent );

    if ( ctfgame.match != MATCH_SETUP )
    {
        if ( competition->value < 3 )
            quake2::getInstance()->gi.cvar_set( "competition", "2" );
        ctfgame.match = MATCH_SETUP;
        CTFResetAllPlayers();
    }
}

void CTFAdmin_Cancel( edict* ent, pmenuhnd_t* p ) { PMenu_Close( ent ); }

pmenu_t adminmenu[] = {
    { "*Administration Menu", PMENU_ALIGN_CENTER, NULL },
    { NULL, PMENU_ALIGN_CENTER, NULL },  // blank
    { "Settings", PMENU_ALIGN_LEFT, CTFAdmin_Settings },
    { NULL, PMENU_ALIGN_LEFT, NULL },
    { NULL, PMENU_ALIGN_LEFT, NULL },
    { "Cancel", PMENU_ALIGN_LEFT, CTFAdmin_Cancel },
    { NULL, PMENU_ALIGN_CENTER, NULL },
};

void CTFOpenAdminMenu( edict* ent )
{
    adminmenu[3].text = NULL;
    adminmenu[3].SelectFunc = NULL;
    if ( ctfgame.match == MATCH_SETUP )
    {
        adminmenu[3].text = "Force start match";
        adminmenu[3].SelectFunc = CTFAdmin_MatchSet;
    }
    else if ( ctfgame.match == MATCH_GAME )
    {
        adminmenu[3].text = "Cancel match";
        adminmenu[3].SelectFunc = CTFAdmin_MatchSet;
    }
    else if ( ctfgame.match == MATCH_NONE && competition->value )
    {
        adminmenu[3].text = "Switch to match mode";
        adminmenu[3].SelectFunc = CTFAdmin_MatchMode;
    }

    //    if (ent->client->menu)
    //        PMenu_Close(ent->client->menu);

    PMenu_Open( ent, adminmenu, -1, sizeof( adminmenu ) / sizeof( pmenu_t ),
                NULL );
}

void CTFAdmin( edict* ent )
{
    char text[1024];

    if ( quake2::getInstance()->gi.argc() > 1 && admin_password->string &&
         *admin_password->string && !ent->client->resp.admin &&
         strcmp( admin_password->string,
                 quake2::getInstance()->gi.argv( 1 ) ) == 0 )
    {
        ent->client->resp.admin = true;
        quake2::getInstance()->gi.bprintf( PRINT_HIGH,
                                           "%s has become an admin.\n",
                                           ent->client->pers.netname );
        quake2::getInstance()->gi.cprintf(
            ent, PRINT_HIGH,
            "Type 'admin' to access the adminstration menu.\n" );
    }

    if ( !ent->client->resp.admin )
    {
        sprintf( text, "%s has requested admin rights.",
                 ent->client->pers.netname );
        CTFBeginElection( ent, ELECT_ADMIN, text );
        return;
    }

    if ( ent->client->menu )
        PMenu_Close( ent );

    CTFOpenAdminMenu( ent );
}

/*----------------------------------------------------------------*/

void CTFStats( edict* ent )
{
    int i, e;
    ghost_t* g;
    char st[80];
    char text[1400];
    edict* e2;

    *text = 0;
    if ( ctfgame.match == MATCH_SETUP )
    {
        for ( i = 1; i <= maxclients->value; i++ )
        {
            e2 = g_edicts + i;
            if ( !e2->inuse )
                continue;
            if ( !e2->client->resp.ready &&
                 e2->client->resp.ctf_team != CTF_NOTEAM )
            {
                sprintf( st, "%s is not ready.\n", e2->client->pers.netname );
                if ( strlen( text ) + strlen( st ) < sizeof( text ) - 50 )
                    strcat( text, st );
            }
        }
    }

    for ( i = 0, g = ctfgame.ghosts; i < MAX_CLIENTS; i++, g++ )
        if ( g->ent )
            break;

    if ( i == MAX_CLIENTS )
    {
        if ( *text )
            quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH, "%s", text );
        quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH,
                                           "No statistics available.\n" );
        return;
    }

    strcat( text,
            "  #|Name            |Score|Kills|Death|BasDf|CarDf|Effcy|\n" );

    for ( i = 0, g = ctfgame.ghosts; i < MAX_CLIENTS; i++, g++ )
    {
        if ( !*g->netname )
            continue;

        if ( g->deaths + g->kills == 0 )
            e = 50;
        else
            e = g->kills * 100 / ( g->kills + g->deaths );
        sprintf( st, "%3d|%-16.16s|%5d|%5d|%5d|%5d|%5d|%4d%%|\n", g->number,
                 g->netname, g->score, g->kills, g->deaths, g->basedef,
                 g->carrierdef, e );
        if ( strlen( text ) + strlen( st ) > sizeof( text ) - 50 )
        {
            sprintf( text + strlen( text ), "And more...\n" );
            quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH, "%s", text );
            return;
        }
        strcat( text, st );
    }
    quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH, "%s", text );
}

void CTFPlayerList( edict* ent )
{
    int i;
    char st[80];
    char text[1400];
    edict* e2;

    *text = 0;
    if ( ctfgame.match == MATCH_SETUP )
    {
        for ( i = 1; i <= maxclients->value; i++ )
        {
            e2 = g_edicts + i;
            if ( !e2->inuse )
                continue;
            if ( !e2->client->resp.ready &&
                 e2->client->resp.ctf_team != CTF_NOTEAM )
            {
                sprintf( st, "%s is not ready.\n", e2->client->pers.netname );
                if ( strlen( text ) + strlen( st ) < sizeof( text ) - 50 )
                    strcat( text, st );
            }
        }
    }

    // number, name, connect time, ping, score, admin

    *text = 0;
    for ( i = 0, e2 = g_edicts + 1; i < maxclients->value; i++, e2++ )
    {
        if ( !e2->inuse )
            continue;

        sprintf(
            st, "%3d %-16.16s %02d:%02d %4d %3d%s%s\n", i + 1,
            e2->client->pers.netname,
            ( level.framenum - e2->client->resp.enterframe ) / 600,
            ( ( level.framenum - e2->client->resp.enterframe ) % 600 ) / 10,
            e2->client->ping, e2->client->resp.score,
            ( ctfgame.match == MATCH_SETUP || ctfgame.match == MATCH_PREGAME )
                ? ( e2->client->resp.ready ? " (ready)" : " (notready)" )
                : "",
            e2->client->resp.admin ? " (admin)" : "" );
        if ( strlen( text ) + strlen( st ) > sizeof( text ) - 50 )
        {
            sprintf( text + strlen( text ), "And more...\n" );
            quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH, "%s", text );
            return;
        }
        strcat( text, st );
    }
    quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH, "%s", text );
}

void CTFWarp( edict* ent )
{
    char text[1024];
    char *mlist, *token;
    static const char* seps = " \t\n\r";

    if ( quake2::getInstance()->gi.argc() < 2 )
    {
        quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH,
                                           "Where do you want to warp to?\n" );
        quake2::getInstance()->gi.cprintf(
            ent, PRINT_HIGH, "Available levels are: %s\n", warp_list->string );
        return;
    }

    mlist = strdup( warp_list->string );

    token = strtok( mlist, seps );
    while ( token != NULL )
    {
        if ( Q_stricmp( token, quake2::getInstance()->gi.argv( 1 ) ) == 0 )
            break;
        token = strtok( NULL, seps );
    }

    if ( token == NULL )
    {
        quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH,
                                           "Unknown CTF level.\n" );
        quake2::getInstance()->gi.cprintf(
            ent, PRINT_HIGH, "Available levels are: %s\n", warp_list->string );
        free( mlist );
        return;
    }

    free( mlist );

    if ( ent->client->resp.admin )
    {
        quake2::getInstance()->gi.bprintf(
            PRINT_HIGH, "%s is warping to level %s.\n",
            ent->client->pers.netname, quake2::getInstance()->gi.argv( 1 ) );
        strncpy( level.forcemap, quake2::getInstance()->gi.argv( 1 ),
                 sizeof( level.forcemap ) - 1 );
        EndDMLevel();
        return;
    }

    sprintf( text, "%s has requested warping to level %s.",
             ent->client->pers.netname, quake2::getInstance()->gi.argv( 1 ) );
    if ( CTFBeginElection( ent, ELECT_MAP, text ) )
        strncpy( ctfgame.elevel, quake2::getInstance()->gi.argv( 1 ),
                 sizeof( ctfgame.elevel ) - 1 );
}

void CTFBoot( edict* ent )
{
    int i;
    edict* targ;
    char text[80];

    if ( !ent->client->resp.admin )
    {
        quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH,
                                           "You are not an admin.\n" );
        return;
    }

    if ( quake2::getInstance()->gi.argc() < 2 )
    {
        quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH,
                                           "Who do you want to kick?\n" );
        return;
    }

    if ( *quake2::getInstance()->gi.argv( 1 ) < '0' &&
         *quake2::getInstance()->gi.argv( 1 ) > '9' )
    {
        quake2::getInstance()->gi.cprintf(
            ent, PRINT_HIGH, "Specify the player number to kick.\n" );
        return;
    }

    i = atoi( quake2::getInstance()->gi.argv( 1 ) );
    if ( i < 1 || i > maxclients->value )
    {
        quake2::getInstance()->gi.cprintf( ent, PRINT_HIGH,
                                           "Invalid player number.\n" );
        return;
    }

    targ = g_edicts + i;
    if ( !targ->inuse )
    {
        quake2::getInstance()->gi.cprintf(
            ent, PRINT_HIGH, "That player number is not connected.\n" );
        return;
    }

    sprintf( text, "kick %d\n", i - 1 );
    quake2::getInstance()->gi.AddCommandString( text );
}
