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
==============================================================================

MEDIC

==============================================================================
*/

#include "m_medic.hpp"

#include "g_local.hpp"
#include "shared/defines.hpp"

bool visible( edict* self, edict* other );

static int sound_idle1;
static int sound_pain1;
static int sound_pain2;
static int sound_die;
static int sound_sight;
static int sound_search;
static int sound_hook_launch;
static int sound_hook_hit;
static int sound_hook_heal;
static int sound_hook_retract;

edict* medic_FindDeadMonster( edict* self )
{
    edict* ent = NULL;
    edict* best = NULL;

    while ( ( ent = findradius( ent, self->s.origin, 1024 ) ) != NULL )
    {
        if ( ent == self )
            continue;
        if ( !( ent->svflags & SVF_MONSTER ) )
            continue;
        if ( ent->monsterinfoVal.aiflags & AI_GOOD_GUY )
            continue;
        if ( ent->owner )
            continue;
        if ( ent->health > 0 )
            continue;
        if ( ent->nextthink )
            continue;
        if ( !visible( self, ent ) )
            continue;
        if ( !best )
        {
            best = ent;
            continue;
        }
        if ( ent->max_health <= best->max_health )
            continue;
        best = ent;
    }

    return best;
}

void medic_idle( edict* self )
{
    edict* ent;

    quake2::getInstance()->quake2::getInstance()->gi.sound(
        self, CHAN_VOICE, sound_idle1, 1, ATTN_IDLE, 0 );

    ent = medic_FindDeadMonster( self );
    if ( ent )
    {
        self->enemy = ent;
        self->enemy->owner = self;
        self->monsterinfoVal.aiflags |= AI_MEDIC;
        FoundTarget( self );
    }
}

void medic_search( edict* self )
{
    edict* ent;

    quake2::getInstance()->quake2::getInstance()->gi.sound(
        self, CHAN_VOICE, sound_search, 1, ATTN_IDLE, 0 );

    if ( !self->oldenemy )
    {
        ent = medic_FindDeadMonster( self );
        if ( ent )
        {
            self->oldenemy = self->enemy;
            self->enemy = ent;
            self->enemy->owner = self;
            self->monsterinfoVal.aiflags |= AI_MEDIC;
            FoundTarget( self );
        }
    }
}

void medic_sight( edict* self, edict* other )
{
    quake2::getInstance()->quake2::getInstance()->gi.sound(
        self, CHAN_VOICE, sound_sight, 1, ATTN_NORM, 0 );
}

mframe_t medic_frames_stand[] = {
    ai_stand, 0, medic_idle, ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,
    ai_stand, 0, NULL,       ai_stand, 0, NULL, ai_stand, 0, NULL,

};
mmove_t medic_move_stand = { FRAME_wait1, FRAME_wait90, medic_frames_stand,
                             NULL };

void medic_stand( edict* self )
{
    self->monsterinfoVal.currentmove = &medic_move_stand;
}

mframe_t medic_frames_walk[] = {
    ai_walk, 6.2, NULL, ai_walk, 18.1, NULL, ai_walk, 1,   NULL,
    ai_walk, 9,   NULL, ai_walk, 10,   NULL, ai_walk, 9,   NULL,
    ai_walk, 11,  NULL, ai_walk, 11.6, NULL, ai_walk, 2,   NULL,
    ai_walk, 9.9, NULL, ai_walk, 14,   NULL, ai_walk, 9.3, NULL };
mmove_t medic_move_walk = { FRAME_walk1, FRAME_walk12, medic_frames_walk,
                            NULL };

void medic_walk( edict* self )
{
    self->monsterinfoVal.currentmove = &medic_move_walk;
}

mframe_t medic_frames_run[] = {
    ai_run, 18,   NULL,   ai_run, 22.5, NULL,   ai_run, 25.4, NULL, ai_run,
    23.4,   NULL, ai_run, 24,     NULL, ai_run, 35.6,   NULL

};
mmove_t medic_move_run = { FRAME_run1, FRAME_run6, medic_frames_run, NULL };

void medic_run( edict* self )
{
    if ( !( self->monsterinfoVal.aiflags & AI_MEDIC ) )
    {
        edict* ent;

        ent = medic_FindDeadMonster( self );
        if ( ent )
        {
            self->oldenemy = self->enemy;
            self->enemy = ent;
            self->enemy->owner = self;
            self->monsterinfoVal.aiflags |= AI_MEDIC;
            FoundTarget( self );
            return;
        }
    }

    if ( self->monsterinfoVal.aiflags & AI_STAND_GROUND )
        self->monsterinfoVal.currentmove = &medic_move_stand;
    else
        self->monsterinfoVal.currentmove = &medic_move_run;
}

mframe_t medic_frames_pain1[] = {
    ai_move, 0, NULL, ai_move, 0, NULL, ai_move, 0, NULL, ai_move, 0, NULL,
    ai_move, 0, NULL, ai_move, 0, NULL, ai_move, 0, NULL, ai_move, 0, NULL };
mmove_t medic_move_pain1 = { FRAME_paina1, FRAME_paina8, medic_frames_pain1,
                             medic_run };

mframe_t medic_frames_pain2[] = {
    ai_move, 0, NULL, ai_move, 0, NULL, ai_move, 0, NULL, ai_move, 0, NULL,
    ai_move, 0, NULL, ai_move, 0, NULL, ai_move, 0, NULL, ai_move, 0, NULL,
    ai_move, 0, NULL, ai_move, 0, NULL, ai_move, 0, NULL, ai_move, 0, NULL,
    ai_move, 0, NULL, ai_move, 0, NULL, ai_move, 0, NULL };
mmove_t medic_move_pain2 = { FRAME_painb1, FRAME_painb15, medic_frames_pain2,
                             medic_run };

void medic_pain( edict* self, edict* other, float kick, int damage )
{
    if ( self->health < ( self->max_health / 2 ) )
        self->s.skinnum = 1;

    if ( level.time < self->pain_debounce_time )
        return;

    self->pain_debounce_time = level.time + 3;

    if ( skill->value == 3 )
        return;  // no pain anims in nightmare

    if ( random() < 0.5 )
    {
        self->monsterinfoVal.currentmove = &medic_move_pain1;
        quake2::getInstance()->quake2::getInstance()->gi.sound(
            self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0 );
    }
    else
    {
        self->monsterinfoVal.currentmove = &medic_move_pain2;
        quake2::getInstance()->quake2::getInstance()->gi.sound(
            self, CHAN_VOICE, sound_pain2, 1, ATTN_NORM, 0 );
    }
}

void medic_fire_blaster( edict* self )
{
    vec3_t start;
    vec3_t forward, right;
    vec3_t end;
    vec3_t dir;
    int effect;

    if ( ( self->s.frame == FRAME_attack9 ) ||
         ( self->s.frame == FRAME_attack12 ) )
        effect = EF_BLASTER;
    else if ( ( self->s.frame == FRAME_attack19 ) ||
              ( self->s.frame == FRAME_attack22 ) ||
              ( self->s.frame == FRAME_attack25 ) ||
              ( self->s.frame == FRAME_attack28 ) )
        effect = EF_HYPERBLASTER;
    else
        effect = 0;

    AngleVectors( self->s.angles, forward, right, NULL );
    G_ProjectSource( self->s.origin, monster_flash_offset[MZ2_MEDIC_BLASTER_1],
                     forward, right, start );

    VectorCopy( self->enemy->s.origin, end );
    end[2] += self->enemy->viewheight;
    VectorSubtract( end, start, dir );

    monster_fire_blaster( self, start, dir, 2, 1000, MZ2_MEDIC_BLASTER_1,
                          effect );
}

void medic_dead( edict* self )
{
    VectorSet( self->mins, -16, -16, -24 );
    VectorSet( self->maxs, 16, 16, -8 );
    self->movetype = MOVETYPE_TOSS;
    self->svflags |= SVF_DEADMONSTER;
    self->nextthink = 0;
    quake2::getInstance()->quake2::getInstance()->gi.linkentity( self );
}

mframe_t medic_frames_death[] = {
    ai_move, 0, NULL, ai_move, 0, NULL, ai_move, 0, NULL, ai_move, 0, NULL,
    ai_move, 0, NULL, ai_move, 0, NULL, ai_move, 0, NULL, ai_move, 0, NULL,
    ai_move, 0, NULL, ai_move, 0, NULL, ai_move, 0, NULL, ai_move, 0, NULL,
    ai_move, 0, NULL, ai_move, 0, NULL, ai_move, 0, NULL, ai_move, 0, NULL,
    ai_move, 0, NULL, ai_move, 0, NULL, ai_move, 0, NULL, ai_move, 0, NULL,
    ai_move, 0, NULL, ai_move, 0, NULL, ai_move, 0, NULL, ai_move, 0, NULL,
    ai_move, 0, NULL, ai_move, 0, NULL, ai_move, 0, NULL, ai_move, 0, NULL,
    ai_move, 0, NULL, ai_move, 0, NULL };
mmove_t medic_move_death = { FRAME_death1, FRAME_death30, medic_frames_death,
                             medic_dead };

void medic_die( edict* self, edict* inflictor, edict* attacker, int damage,
                vec3_t point )
{
    int n;

    // if we had a pending patient, free him up for another medic
    if ( ( self->enemy ) && ( self->enemy->owner == self ) )
        self->enemy->owner = NULL;

    // check for gib
    if ( self->health <= self->gib_health )
    {
        quake2::getInstance()->quake2::getInstance()->gi.sound(
            self, CHAN_VOICE,
            quake2::getInstance()->quake2::getInstance()->gi.soundindex(
                "misc/udeath.wav" ),
            1, ATTN_NORM, 0 );
        for ( n = 0; n < 2; n++ )
            ThrowGib( self, "models/objects/gibs/bone/tris.md2", damage,
                      GIB_ORGANIC );
        for ( n = 0; n < 4; n++ )
            ThrowGib( self, "models/objects/gibs/sm_meat/tris.md2", damage,
                      GIB_ORGANIC );
        ThrowHead( self, "models/objects/gibs/head2/tris.md2", damage,
                   GIB_ORGANIC );
        self->deadflag = DEAD_DEAD;
        return;
    }

    if ( self->deadflag == DEAD_DEAD )
        return;

    // regular death
    quake2::getInstance()->quake2::getInstance()->gi.sound(
        self, CHAN_VOICE, sound_die, 1, ATTN_NORM, 0 );
    self->deadflag = DEAD_DEAD;
    self->takedamage = DAMAGE_YES;

    self->monsterinfoVal.currentmove = &medic_move_death;
}

void medic_duck_down( edict* self )
{
    if ( self->monsterinfoVal.aiflags & AI_DUCKED )
        return;
    self->monsterinfoVal.aiflags |= AI_DUCKED;
    self->maxs[2] -= 32;
    self->takedamage = DAMAGE_YES;
    self->monsterinfoVal.pausetime = level.time + 1;
    quake2::getInstance()->quake2::getInstance()->gi.linkentity( self );
}

void medic_duck_hold( edict* self )
{
    if ( level.time >= self->monsterinfoVal.pausetime )
        self->monsterinfoVal.aiflags &= ~AI_HOLD_FRAME;
    else
        self->monsterinfoVal.aiflags |= AI_HOLD_FRAME;
}

void medic_duck_up( edict* self )
{
    self->monsterinfoVal.aiflags &= ~AI_DUCKED;
    self->maxs[2] += 32;
    self->takedamage = DAMAGE_AIM;
    quake2::getInstance()->quake2::getInstance()->gi.linkentity( self );
}

mframe_t medic_frames_duck[] = { ai_move, -1, NULL,
                                 ai_move, -1, NULL,
                                 ai_move, -1, medic_duck_down,
                                 ai_move, -1, medic_duck_hold,
                                 ai_move, -1, NULL,
                                 ai_move, -1, NULL,
                                 ai_move, -1, medic_duck_up,
                                 ai_move, -1, NULL,
                                 ai_move, -1, NULL,
                                 ai_move, -1, NULL,
                                 ai_move, -1, NULL,
                                 ai_move, -1, NULL,
                                 ai_move, -1, NULL,
                                 ai_move, -1, NULL,
                                 ai_move, -1, NULL,
                                 ai_move, -1, NULL };
mmove_t medic_move_duck = { FRAME_duck1, FRAME_duck16, medic_frames_duck,
                            medic_run };

void medic_dodge( edict* self, edict* attacker, float eta )
{
    if ( random() > 0.25 )
        return;

    if ( !self->enemy )
        self->enemy = attacker;

    self->monsterinfoVal.currentmove = &medic_move_duck;
}

mframe_t medic_frames_attackHyperBlaster[] = {
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, medic_fire_blaster,
    ai_charge, 0, medic_fire_blaster,
    ai_charge, 0, medic_fire_blaster,
    ai_charge, 0, medic_fire_blaster,
    ai_charge, 0, medic_fire_blaster,
    ai_charge, 0, medic_fire_blaster,
    ai_charge, 0, medic_fire_blaster,
    ai_charge, 0, medic_fire_blaster,
    ai_charge, 0, medic_fire_blaster,
    ai_charge, 0, medic_fire_blaster,
    ai_charge, 0, medic_fire_blaster,
    ai_charge, 0, medic_fire_blaster };
mmove_t medic_move_attackHyperBlaster = { FRAME_attack15, FRAME_attack30,
                                          medic_frames_attackHyperBlaster,
                                          medic_run };

void medic_continue( edict* self )
{
    if ( visible( self, self->enemy ) )
        if ( random() <= 0.95 )
            self->monsterinfoVal.currentmove = &medic_move_attackHyperBlaster;
}

mframe_t medic_frames_attackBlaster[] = {
    ai_charge,     0, NULL,
    ai_charge,     5, NULL,
    ai_charge,     5, NULL,
    ai_charge,     3, NULL,
    ai_charge,     2, NULL,
    ai_charge,     0, NULL,
    ai_charge,     0, NULL,
    ai_charge,     0, NULL,
    ai_charge,     0, medic_fire_blaster,
    ai_charge,     0, NULL,
    ai_charge,     0, NULL,
    ai_charge,     0, medic_fire_blaster,
    ai_charge,     0, NULL,
    ai_charge,     0,
    medic_continue  // Change to medic_continue... Else, go to frame 32
};
mmove_t medic_move_attackBlaster = { FRAME_attack1, FRAME_attack14,
                                     medic_frames_attackBlaster, medic_run };

void medic_hook_launch( edict* self )
{
    quake2::getInstance()->quake2::getInstance()->gi.sound(
        self, CHAN_WEAPON, sound_hook_launch, 1, ATTN_NORM, 0 );
}

void ED_CallSpawn( edict* ent );

static vec3_t medic_cable_offsets[] = {
    45.0, -9.2, 15.5,  48.4,  -9.7, 15.2,  47.8,  -9.8, 15.8,  47.3,
    -9.3, 14.3, 45.4,  -10.1, 13.1, 41.9,  -12.7, 12.0, 37.8,  -15.8,
    11.2, 34.3, -18.4, 10.7,  32.7, -19.7, 10.4,  32.7, -19.7, 10.4 };

void medic_cable_attack( edict* self )
{
    vec3_t offset, start, end, f, r;
    trace_t tr;
    vec3_t dir, angles;
    float distance;

    if ( !self->enemy->inuse )
        return;

    AngleVectors( self->s.angles, f, r, NULL );
    VectorCopy( medic_cable_offsets[self->s.frame - FRAME_attack42], offset );
    G_ProjectSource( self->s.origin, offset, f, r, start );

    // check for max distance
    VectorSubtract( start, self->enemy->s.origin, dir );
    distance = VectorLength( dir );
    if ( distance > 256 )
        return;

    // check for min/max pitch
    vectoangles( dir, angles );
    if ( angles[0] < -180 )
        angles[0] += 360;
    if ( fabs( angles[0] ) > 45 )
        return;

    tr = quake2::getInstance()->quake2::getInstance()->gi.trace(
        start, NULL, NULL, self->enemy->s.origin, self, MASK_SHOT );
    if ( tr.fraction != 1.0 && tr.ent != self->enemy )
        return;

    if ( self->s.frame == FRAME_attack43 )
    {
        quake2::getInstance()->quake2::getInstance()->gi.sound(
            self->enemy, CHAN_AUTO, sound_hook_hit, 1, ATTN_NORM, 0 );
        self->enemy->monsterinfoVal.aiflags |= AI_RESURRECTING;
    }
    else if ( self->s.frame == FRAME_attack50 )
    {
        self->enemy->spawnflags = 0;
        self->enemy->monsterinfoVal.aiflags = 0;
        self->enemy->target = NULL;
        self->enemy->targetname = NULL;
        self->enemy->combattarget = NULL;
        self->enemy->deathtarget = NULL;
        self->enemy->owner = self;
        ED_CallSpawn( self->enemy );
        self->enemy->owner = NULL;
        if ( self->enemy->think )
        {
            self->enemy->nextthink = level.time;
            self->enemy->think( self->enemy );
        }
        self->enemy->monsterinfoVal.aiflags |= AI_RESURRECTING;
        if ( self->oldenemy && self->oldenemy->client )
        {
            self->enemy->enemy = self->oldenemy;
            FoundTarget( self->enemy );
        }
    }
    else
    {
        if ( self->s.frame == FRAME_attack44 )
            quake2::getInstance()->quake2::getInstance()->gi.sound(
                self, CHAN_WEAPON, sound_hook_heal, 1, ATTN_NORM, 0 );
    }

    // adjust start for beam origin being in middle of a segment
    VectorMA( start, 8, f, start );

    // adjust end z for end spot since the monster is currently dead
    VectorCopy( self->enemy->s.origin, end );
    end[2] = self->enemy->absmin[2] + self->enemy->size[2] / 2;

    quake2::getInstance()->quake2::getInstance()->gi.WriteByte(
        svc_temp_entity );
    quake2::getInstance()->quake2::getInstance()->gi.WriteByte(
        TE_MEDIC_CABLE_ATTACK );
    quake2::getInstance()->quake2::getInstance()->gi.WriteShort( self -
                                                                   g_edicts );
    quake2::getInstance()->quake2::getInstance()->gi.WritePosition( start );
    quake2::getInstance()->quake2::getInstance()->gi.WritePosition( end );
    quake2::getInstance()->quake2::getInstance()->gi.multicast(
        self->s.origin, multicast_t::MULTICAST_PVS );
}

void medic_hook_retract( edict* self )
{
    quake2::getInstance()->quake2::getInstance()->gi.sound(
        self, CHAN_WEAPON, sound_hook_retract, 1, ATTN_NORM, 0 );
    self->enemy->monsterinfoVal.aiflags &= ~AI_RESURRECTING;
}

mframe_t medic_frames_attackCable[] = { ai_move,   2,    NULL,
                                        ai_move,   3,    NULL,
                                        ai_move,   5,    NULL,
                                        ai_move,   4.4,  NULL,
                                        ai_charge, 4.7,  NULL,
                                        ai_charge, 5,    NULL,
                                        ai_charge, 6,    NULL,
                                        ai_charge, 4,    NULL,
                                        ai_charge, 0,    NULL,
                                        ai_move,   0,    medic_hook_launch,
                                        ai_move,   0,    medic_cable_attack,
                                        ai_move,   0,    medic_cable_attack,
                                        ai_move,   0,    medic_cable_attack,
                                        ai_move,   0,    medic_cable_attack,
                                        ai_move,   0,    medic_cable_attack,
                                        ai_move,   0,    medic_cable_attack,
                                        ai_move,   0,    medic_cable_attack,
                                        ai_move,   0,    medic_cable_attack,
                                        ai_move,   0,    medic_cable_attack,
                                        ai_move,   -15,  medic_hook_retract,
                                        ai_move,   -1.5, NULL,
                                        ai_move,   -1.2, NULL,
                                        ai_move,   -3,   NULL,
                                        ai_move,   -2,   NULL,
                                        ai_move,   0.3,  NULL,
                                        ai_move,   0.7,  NULL,
                                        ai_move,   1.2,  NULL,
                                        ai_move,   1.3,  NULL };
mmove_t medic_move_attackCable = { FRAME_attack33, FRAME_attack60,
                                   medic_frames_attackCable, medic_run };

void medic_attack( edict* self )
{
    if ( self->monsterinfoVal.aiflags & AI_MEDIC )
        self->monsterinfoVal.currentmove = &medic_move_attackCable;
    else
        self->monsterinfoVal.currentmove = &medic_move_attackBlaster;
}

bool medic_checkattack( edict* self )
{
    if ( self->monsterinfoVal.aiflags & AI_MEDIC )
    {
        medic_attack( self );
        return true;
    }

    return M_CheckAttack( self );
}

/*QUAKED monster_medic (1 .5 0) (-16 -16 -24) (16 16 32) Ambush Trigger_Spawn
 * Sight
 */
void SP_monster_medic( edict* self )
{
    if ( deathmatch->value )
    {
        G_FreeEdict( self );
        return;
    }

    sound_idle1 = quake2::getInstance()->quake2::getInstance()->gi.soundindex(
        "medic/idle.wav" );
    sound_pain1 = quake2::getInstance()->quake2::getInstance()->gi.soundindex(
        "medic/medpain1.wav" );
    sound_pain2 = quake2::getInstance()->quake2::getInstance()->gi.soundindex(
        "medic/medpain2.wav" );
    sound_die = quake2::getInstance()->quake2::getInstance()->gi.soundindex(
        "medic/meddeth1.wav" );
    sound_sight = quake2::getInstance()->quake2::getInstance()->gi.soundindex(
        "medic/medsght1.wav" );
    sound_search =
        quake2::getInstance()->quake2::getInstance()->gi.soundindex(
            "medic/medsrch1.wav" );
    sound_hook_launch =
        quake2::getInstance()->quake2::getInstance()->gi.soundindex(
            "medic/medatck2.wav" );
    sound_hook_hit =
        quake2::getInstance()->quake2::getInstance()->gi.soundindex(
            "medic/medatck3.wav" );
    sound_hook_heal =
        quake2::getInstance()->quake2::getInstance()->gi.soundindex(
            "medic/medatck4.wav" );
    sound_hook_retract =
        quake2::getInstance()->quake2::getInstance()->gi.soundindex(
            "medic/medatck5.wav" );

    quake2::getInstance()->quake2::getInstance()->gi.soundindex(
        "medic/medatck1.wav" );

    self->movetype = MOVETYPE_STEP;
    self->solid = SOLID_BBOX;
    self->s.modelindex =
        quake2::getInstance()->quake2::getInstance()->gi.modelindex(
            "models/monsters/medic/tris.md2" );
    VectorSet( self->mins, -24, -24, -24 );
    VectorSet( self->maxs, 24, 24, 32 );

    self->health = 300;
    self->gib_health = -130;
    self->mass = 400;

    self->pain = medic_pain;
    self->die = medic_die;

    self->monsterinfoVal.stand = medic_stand;
    self->monsterinfoVal.walk = medic_walk;
    self->monsterinfoVal.run = medic_run;
    self->monsterinfoVal.dodge = medic_dodge;
    self->monsterinfoVal.attack = medic_attack;
    self->monsterinfoVal.melee = NULL;
    self->monsterinfoVal.sight = medic_sight;
    self->monsterinfoVal.idle = medic_idle;
    self->monsterinfoVal.search = medic_search;
    self->monsterinfoVal.checkattack = medic_checkattack;

    quake2::getInstance()->quake2::getInstance()->gi.linkentity( self );

    self->monsterinfoVal.currentmove = &medic_move_stand;
    self->monsterinfoVal.scale = MODEL_SCALE;

    walkmonster_start( self );
}
