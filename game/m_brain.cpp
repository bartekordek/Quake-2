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

brain

==============================================================================
*/

#include "g_local.hpp"
#include "m_brain.hpp"


static int    sound_chest_open;
static int    sound_tentacles_extend;
static int    sound_tentacles_retract;
static int    sound_death;
static int    sound_idle1;
static int    sound_idle2;
static int    sound_idle3;
static int    sound_pain1;
static int    sound_pain2;
static int    sound_sight;
static int    sound_search;
static int    sound_melee1;
static int    sound_melee2;
static int    sound_melee3;


void brain_sight (edict *self, edict *other)
{
    quake2::getInstance()->gi.sound (self, CHAN_VOICE, sound_sight, 1, ATTN_NORM, 0);
}

void brain_search (edict *self)
{
    quake2::getInstance()->gi.sound (self, CHAN_VOICE, sound_search, 1, ATTN_NORM, 0);
}


void brain_run (edict *self);
void brain_dead (edict *self);


//
// STAND
//

mframe_t brain_frames_stand [] =
{
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,

    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,

    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL
};
mmove_t brain_move_stand = {FRAME_stand01, FRAME_stand30, brain_frames_stand, NULL};

void brain_stand (edict *self)
{
    self->monsterinfoVal.currentmove = &brain_move_stand;
}


//
// IDLE
//

mframe_t brain_frames_idle [] =
{
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,

    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,

    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL,
    ai_stand,    0,    NULL
};
mmove_t brain_move_idle = {FRAME_stand31, FRAME_stand60, brain_frames_idle, brain_stand};

void brain_idle (edict *self)
{
    quake2::getInstance()->gi.sound (self, CHAN_AUTO, sound_idle3, 1, ATTN_IDLE, 0);
    self->monsterinfoVal.currentmove = &brain_move_idle;
}


//
// WALK
//
mframe_t brain_frames_walk1 [] =
{
    ai_walk,    7,    NULL,
    ai_walk,    2,    NULL,
    ai_walk,    3,    NULL,
    ai_walk,    3,    NULL,
    ai_walk,    1,    NULL,
    ai_walk,    0,    NULL,
    ai_walk,    0,    NULL,
    ai_walk,    9,    NULL,
    ai_walk,    -4,    NULL,
    ai_walk,    -1,    NULL,
    ai_walk,    2,    NULL
};
mmove_t brain_move_walk1 = {FRAME_walk101, FRAME_walk111, brain_frames_walk1, NULL};

// walk2 is FUBAR, do not use
#if 0
void brain_walk2_cycle (edict *self)
{
    if (random() > 0.1)
        self->monsterinfoVal.nextframe = FRAME_walk220;
}

mframe_t brain_frames_walk2 [] =
{
    ai_walk,    3,    NULL,
    ai_walk,    -2,    NULL,
    ai_walk,    -4,    NULL,
    ai_walk,    -3,    NULL,
    ai_walk,    0,    NULL,
    ai_walk,    1,    NULL,
    ai_walk,    12,    NULL,
    ai_walk,    0,    NULL,
    ai_walk,    -3,    NULL,
    ai_walk,    0,    NULL,

    ai_walk,    -2,    NULL,
    ai_walk,    0,    NULL,
    ai_walk,    0,    NULL,
    ai_walk,    1,    NULL,
    ai_walk,    0,    NULL,
    ai_walk,    0,    NULL,
    ai_walk,    0,    NULL,
    ai_walk,    0,    NULL,
    ai_walk,    0,    NULL,
    ai_walk,    10,    NULL,        // Cycle Start

    ai_walk,    -1,    NULL,
    ai_walk,    7,    NULL,
    ai_walk,    0,    NULL,
    ai_walk,    3,    NULL,
    ai_walk,    -3,    NULL,
    ai_walk,    2,    NULL,
    ai_walk,    4,    NULL,
    ai_walk,    -3,    NULL,
    ai_walk,    2,    NULL,
    ai_walk,    0,    NULL,

    ai_walk,    4,    brain_walk2_cycle,
    ai_walk,    -1,    NULL,
    ai_walk,    -1,    NULL,
    ai_walk,    -8,    NULL,
    ai_walk,    0,    NULL,
    ai_walk,    1,    NULL,
    ai_walk,    5,    NULL,
    ai_walk,    2,    NULL,
    ai_walk,    -1,    NULL,
    ai_walk,    -5,    NULL
};
mmove_t brain_move_walk2 = {FRAME_walk201, FRAME_walk240, brain_frames_walk2, NULL};
#endif

void brain_walk (edict *self)
{
//    if (random() <= 0.5)
        self->monsterinfoVal.currentmove = &brain_move_walk1;
//    else
//        self->monsterinfoVal.currentmove = &brain_move_walk2;
}



mframe_t brain_frames_defense [] =
{
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL
};
mmove_t brain_move_defense = {FRAME_defens01, FRAME_defens08, brain_frames_defense, NULL};

mframe_t brain_frames_pain3 [] =
{
    ai_move,    -2,    NULL,
    ai_move,    2,    NULL,
    ai_move,    1,    NULL,
    ai_move,    3,    NULL,
    ai_move,    0,    NULL,
    ai_move,    -4,    NULL
};
mmove_t brain_move_pain3 = {FRAME_pain301, FRAME_pain306, brain_frames_pain3, brain_run};

mframe_t brain_frames_pain2 [] =
{
    ai_move,    -2,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    3,    NULL,
    ai_move,    1,    NULL,
    ai_move,    -2,    NULL
};
mmove_t brain_move_pain2 = {FRAME_pain201, FRAME_pain208, brain_frames_pain2, brain_run};

mframe_t brain_frames_pain1 [] =
{
    ai_move,    -6,    NULL,
    ai_move,    -2,    NULL,
    ai_move,    -6,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    2,    NULL,
    ai_move,    0,    NULL,
    ai_move,    2,    NULL,
    ai_move,    1,    NULL,
    ai_move,    7,    NULL,
    ai_move,    0,    NULL,
    ai_move,    3,    NULL,
    ai_move,    -1,    NULL
};
mmove_t brain_move_pain1 = {FRAME_pain101, FRAME_pain121, brain_frames_pain1, brain_run};


//
// DUCK
//

void brain_duck_down (edict *self)
{
    if (self->monsterinfoVal.aiflags & AI_DUCKED)
        return;
    self->monsterinfoVal.aiflags |= AI_DUCKED;
    self->maxs[2] -= 32;
    self->takedamage = DAMAGE_YES;
    quake2::getInstance()->gi.linkentity (self);
}

void brain_duck_hold (edict *self)
{
    if (level.time >= self->monsterinfoVal.pausetime)
        self->monsterinfoVal.aiflags &= ~AI_HOLD_FRAME;
    else
        self->monsterinfoVal.aiflags |= AI_HOLD_FRAME;
}

void brain_duck_up (edict *self)
{
    self->monsterinfoVal.aiflags &= ~AI_DUCKED;
    self->maxs[2] += 32;
    self->takedamage = DAMAGE_AIM;
    quake2::getInstance()->gi.linkentity (self);
}

mframe_t brain_frames_duck [] =
{
    ai_move,    0,    NULL,
    ai_move,    -2,    brain_duck_down,
    ai_move,    17,    brain_duck_hold,
    ai_move,    -3,    NULL,
    ai_move,    -1,    brain_duck_up,
    ai_move,    -5,    NULL,
    ai_move,    -6,    NULL,
    ai_move,    -6,    NULL
};
mmove_t brain_move_duck = {FRAME_duck01, FRAME_duck08, brain_frames_duck, brain_run};

void brain_dodge (edict *self, edict *attacker, float eta)
{
    if (random() > 0.25)
        return;

    if (!self->enemy)
        self->enemy = attacker;

    self->monsterinfoVal.pausetime = level.time + eta + 0.5;
    self->monsterinfoVal.currentmove = &brain_move_duck;
}


mframe_t brain_frames_death2 [] =
{
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    9,    NULL,
    ai_move,    0,    NULL
};
mmove_t brain_move_death2 = {FRAME_death201, FRAME_death205, brain_frames_death2, brain_dead};

mframe_t brain_frames_death1 [] =
{
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    -2,    NULL,
    ai_move,    9,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL,
    ai_move,    0,    NULL
};
mmove_t brain_move_death1 = {FRAME_death101, FRAME_death118, brain_frames_death1, brain_dead};


//
// MELEE
//

void brain_swing_right (edict *self)
{
    quake2::getInstance()->gi.sound (self, CHAN_BODY, sound_melee1, 1, ATTN_NORM, 0);
}

void brain_hit_right (edict *self)
{
    vec3_t    aim;

    VectorSet (aim, MELEE_DISTANCE, self->maxs[0], 8);
    if (fire_hit (self, aim, (15 + (rand() %5)), 40))
        quake2::getInstance()->gi.sound (self, CHAN_WEAPON, sound_melee3, 1, ATTN_NORM, 0);
}

void brain_swing_left (edict *self)
{
    quake2::getInstance()->gi.sound (self, CHAN_BODY, sound_melee2, 1, ATTN_NORM, 0);
}

void brain_hit_left (edict *self)
{
    vec3_t    aim;

    VectorSet (aim, MELEE_DISTANCE, self->mins[0], 8);
    if (fire_hit (self, aim, (15 + (rand() %5)), 40))
        quake2::getInstance()->gi.sound (self, CHAN_WEAPON, sound_melee3, 1, ATTN_NORM, 0);
}

mframe_t brain_frames_attack1 [] =
{
    ai_charge,    8,    NULL,
    ai_charge,    3,    NULL,
    ai_charge,    5,    NULL,
    ai_charge,    0,    NULL,
    ai_charge,    -3,    brain_swing_right,
    ai_charge,    0,    NULL,
    ai_charge,    -5,    NULL,
    ai_charge,    -7,    brain_hit_right,
    ai_charge,    0,    NULL,
    ai_charge,    6,    brain_swing_left,
    ai_charge,    1,    NULL,
    ai_charge,    2,    brain_hit_left,
    ai_charge,    -3,    NULL,
    ai_charge,    6,    NULL,
    ai_charge,    -1,    NULL,
    ai_charge,    -3,    NULL,
    ai_charge,    2,    NULL,
    ai_charge,    -11,NULL
};
mmove_t brain_move_attack1 = {FRAME_attak101, FRAME_attak118, brain_frames_attack1, brain_run};

void brain_chest_open (edict *self)
{
    self->spawnflags &= ~65536;
    self->monsterinfoVal.power_armor_type = POWER_ARMOR_NONE;
    quake2::getInstance()->gi.sound (self, CHAN_BODY, sound_chest_open, 1, ATTN_NORM, 0);
}

void brain_tentacle_attack (edict *self)
{
    vec3_t    aim;

    VectorSet (aim, MELEE_DISTANCE, 0, 8);
    if (fire_hit (self, aim, (10 + (rand() %5)), -600) && skill->value > 0)
        self->spawnflags |= 65536;
    quake2::getInstance()->gi.sound (self, CHAN_WEAPON, sound_tentacles_retract, 1, ATTN_NORM, 0);
}

void brain_chest_closed (edict *self)
{
    self->monsterinfoVal.power_armor_type = POWER_ARMOR_SCREEN;
    if (self->spawnflags & 65536)
    {
        self->spawnflags &= ~65536;
        self->monsterinfoVal.currentmove = &brain_move_attack1;
    }
}

mframe_t brain_frames_attack2 [] =
{
    ai_charge,    5,    NULL,
    ai_charge,    -4,    NULL,
    ai_charge,    -4,    NULL,
    ai_charge,    -3,    NULL,
    ai_charge,    0,    brain_chest_open,
    ai_charge,    0,    NULL,
    ai_charge,    13,    brain_tentacle_attack,
    ai_charge,    0,    NULL,
    ai_charge,    2,    NULL,
    ai_charge,    0,    NULL,
    ai_charge,    -9,    brain_chest_closed,
    ai_charge,    0,    NULL,
    ai_charge,    4,    NULL,
    ai_charge,    3,    NULL,
    ai_charge,    2,    NULL,
    ai_charge,    -3,    NULL,
    ai_charge,    -6,    NULL
};
mmove_t brain_move_attack2 = {FRAME_attak201, FRAME_attak217, brain_frames_attack2, brain_run};

void brain_melee(edict *self)
{
    if (random() <= 0.5)
        self->monsterinfoVal.currentmove = &brain_move_attack1;
    else
        self->monsterinfoVal.currentmove = &brain_move_attack2;
}


//
// RUN
//

mframe_t brain_frames_run [] =
{
    ai_run,    9,    NULL,
    ai_run,    2,    NULL,
    ai_run,    3,    NULL,
    ai_run,    3,    NULL,
    ai_run,    1,    NULL,
    ai_run,    0,    NULL,
    ai_run,    0,    NULL,
    ai_run,    10,    NULL,
    ai_run,    -4,    NULL,
    ai_run,    -1,    NULL,
    ai_run,    2,    NULL
};
mmove_t brain_move_run = {FRAME_walk101, FRAME_walk111, brain_frames_run, NULL};

void brain_run (edict *self)
{
    self->monsterinfoVal.power_armor_type = POWER_ARMOR_SCREEN;
    if (self->monsterinfoVal.aiflags & AI_STAND_GROUND)
        self->monsterinfoVal.currentmove = &brain_move_stand;
    else
        self->monsterinfoVal.currentmove = &brain_move_run;
}


void brain_pain (edict *self, edict *other, float kick, int damage)
{
    float    r;

    if (self->health < (self->max_health / 2))
        self->s.skinnum = 1;

    if (level.time < self->pain_debounce_time)
        return;

    self->pain_debounce_time = level.time + 3;
    if (skill->value == 3)
        return;        // no pain anims in nightmare

    r = random();
    if (r < 0.33)
    {
        quake2::getInstance()->gi.sound (self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0);
        self->monsterinfoVal.currentmove = &brain_move_pain1;
    }
    else if (r < 0.66)
    {
        quake2::getInstance()->gi.sound (self, CHAN_VOICE, sound_pain2, 1, ATTN_NORM, 0);
        self->monsterinfoVal.currentmove = &brain_move_pain2;
    }
    else
    {
        quake2::getInstance()->gi.sound (self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0);
        self->monsterinfoVal.currentmove = &brain_move_pain3;
    }
}

void brain_dead (edict *self)
{
    VectorSet (self->mins, -16, -16, -24);
    VectorSet (self->maxs, 16, 16, -8);
    self->movetype = MOVETYPE_TOSS;
    self->svflags |= SVF_DEADMONSTER;
    self->nextthink = 0;
    quake2::getInstance()->gi.linkentity (self);
}



void brain_die (edict *self, edict *inflictor, edict *attacker, int damage, vec3_t point)
{
    int        n;

    self->s.effects = 0;
    self->monsterinfoVal.power_armor_type = POWER_ARMOR_NONE;

// check for gib
    if (self->health <= self->gib_health)
    {
        quake2::getInstance()->gi.sound (self, CHAN_VOICE, quake2::getInstance()->gi.soundindex ("misc/udeath.wav"), 1, ATTN_NORM, 0);
        for (n= 0; n < 2; n++)
            ThrowGib (self, "models/objects/gibs/bone/tris.md2", damage, GIB_ORGANIC);
        for (n= 0; n < 4; n++)
            ThrowGib (self, "models/objects/gibs/sm_meat/tris.md2", damage, GIB_ORGANIC);
        ThrowHead (self, "models/objects/gibs/head2/tris.md2", damage, GIB_ORGANIC);
        self->deadflag = DEAD_DEAD;
        return;
    }

    if (self->deadflag == DEAD_DEAD)
        return;

// regular death
    quake2::getInstance()->gi.sound (self, CHAN_VOICE, sound_death, 1, ATTN_NORM, 0);
    self->deadflag = DEAD_DEAD;
    self->takedamage = DAMAGE_YES;
    if (random() <= 0.5)
        self->monsterinfoVal.currentmove = &brain_move_death1;
    else
        self->monsterinfoVal.currentmove = &brain_move_death2;
}

/*QUAKED monster_brain (1 .5 0) (-16 -16 -24) (16 16 32) Ambush Trigger_Spawn Sight
*/
void SP_monster_brain (edict *self)
{
    if (deathmatch->value)
    {
        G_FreeEdict (self);
        return;
    }

    sound_chest_open = quake2::getInstance()->gi.soundindex ("brain/brnatck1.wav");
    sound_tentacles_extend = quake2::getInstance()->gi.soundindex ("brain/brnatck2.wav");
    sound_tentacles_retract = quake2::getInstance()->gi.soundindex ("brain/brnatck3.wav");
    sound_death = quake2::getInstance()->gi.soundindex ("brain/brndeth1.wav");
    sound_idle1 = quake2::getInstance()->gi.soundindex ("brain/brnidle1.wav");
    sound_idle2 = quake2::getInstance()->gi.soundindex ("brain/brnidle2.wav");
    sound_idle3 = quake2::getInstance()->gi.soundindex ("brain/brnlens1.wav");
    sound_pain1 = quake2::getInstance()->gi.soundindex ("brain/brnpain1.wav");
    sound_pain2 = quake2::getInstance()->gi.soundindex ("brain/brnpain2.wav");
    sound_sight = quake2::getInstance()->gi.soundindex ("brain/brnsght1.wav");
    sound_search = quake2::getInstance()->gi.soundindex ("brain/brnsrch1.wav");
    sound_melee1 = quake2::getInstance()->gi.soundindex ("brain/melee1.wav");
    sound_melee2 = quake2::getInstance()->gi.soundindex ("brain/melee2.wav");
    sound_melee3 = quake2::getInstance()->gi.soundindex ("brain/melee3.wav");

    self->movetype = MOVETYPE_STEP;
    self->solid = SOLID_BBOX;
    self->s.modelindex = quake2::getInstance()->gi.modelindex ("models/monsters/brain/tris.md2");
    VectorSet (self->mins, -16, -16, -24);
    VectorSet (self->maxs, 16, 16, 32);

    self->health = 300;
    self->gib_health = -150;
    self->mass = 400;

    self->pain = brain_pain;
    self->die = brain_die;

    self->monsterinfoVal.stand = brain_stand;
    self->monsterinfoVal.walk = brain_walk;
    self->monsterinfoVal.run = brain_run;
    self->monsterinfoVal.dodge = brain_dodge;
//    self->monsterinfoVal.attack = brain_attack;
    self->monsterinfoVal.melee = brain_melee;
    self->monsterinfoVal.sight = brain_sight;
    self->monsterinfoVal.search = brain_search;
    self->monsterinfoVal.idle = brain_idle;

    self->monsterinfoVal.power_armor_type = POWER_ARMOR_SCREEN;
    self->monsterinfoVal.power_armor_power = 100;

    quake2::getInstance()->gi.linkentity (self);

    self->monsterinfoVal.currentmove = &brain_move_stand;
    self->monsterinfoVal.scale = MODEL_SCALE;

    walkmonster_start (self);
}
