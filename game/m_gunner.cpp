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

GUNNER

==============================================================================
*/

#include "g_local.hpp"
#include "m_gunner.hpp"


static int    sound_pain;
static int    sound_pain2;
static int    sound_death;
static int    sound_idle;
static int    sound_open;
static int    sound_search;
static int    sound_sight;


void gunner_idlesound (edict *self)
{
    quake2::getInstance()->gi.sound (self, CHAN_VOICE, sound_idle, 1, ATTN_IDLE, 0);
}

void gunner_sight (edict *self, edict *other)
{
    quake2::getInstance()->gi.sound (self, CHAN_VOICE, sound_sight, 1, ATTN_NORM, 0);
}

void gunner_search (edict *self)
{
    quake2::getInstance()->gi.sound (self, CHAN_VOICE, sound_search, 1, ATTN_NORM, 0);
}


bool visible (edict *self, edict *other);
void GunnerGrenade (edict *self);
void GunnerFire (edict *self);
void gunner_fire_chain(edict *self);
void gunner_refire_chain(edict *self);


void gunner_stand (edict *self);

mframe_t gunner_frames_fidget [] =
{
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, gunner_idlesound,
    ai_stand, 0, NULL,

    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,

    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,

    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,

    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL
};
mmove_t    gunner_move_fidget = {FRAME_stand31, FRAME_stand70, gunner_frames_fidget, gunner_stand};

void gunner_fidget (edict *self)
{
    if (self->monsterinfoVal.aiflags & AI_STAND_GROUND)
        return;
    if (random() <= 0.05)
        self->monsterinfoVal.currentmove = &gunner_move_fidget;
}

mframe_t gunner_frames_stand [] =
{
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, gunner_fidget,

    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, gunner_fidget,

    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, NULL,
    ai_stand, 0, gunner_fidget
};
mmove_t    gunner_move_stand = {FRAME_stand01, FRAME_stand30, gunner_frames_stand, NULL};

void gunner_stand (edict *self)
{
        self->monsterinfoVal.currentmove = &gunner_move_stand;
}


mframe_t gunner_frames_walk [] =
{
    ai_walk, 0, NULL,
    ai_walk, 3, NULL,
    ai_walk, 4, NULL,
    ai_walk, 5, NULL,
    ai_walk, 7, NULL,
    ai_walk, 2, NULL,
    ai_walk, 6, NULL,
    ai_walk, 4, NULL,
    ai_walk, 2, NULL,
    ai_walk, 7, NULL,
    ai_walk, 5, NULL,
    ai_walk, 7, NULL,
    ai_walk, 4, NULL
};
mmove_t gunner_move_walk = {FRAME_walk07, FRAME_walk19, gunner_frames_walk, NULL};

void gunner_walk (edict *self)
{
    self->monsterinfoVal.currentmove = &gunner_move_walk;
}

mframe_t gunner_frames_run [] =
{
    ai_run, 26, NULL,
    ai_run, 9,  NULL,
    ai_run, 9,  NULL,
    ai_run, 9,  NULL,
    ai_run, 15, NULL,
    ai_run, 10, NULL,
    ai_run, 13, NULL,
    ai_run, 6,  NULL
};

mmove_t gunner_move_run = {FRAME_run01, FRAME_run08, gunner_frames_run, NULL};

void gunner_run (edict *self)
{
    if (self->monsterinfoVal.aiflags & AI_STAND_GROUND)
        self->monsterinfoVal.currentmove = &gunner_move_stand;
    else
        self->monsterinfoVal.currentmove = &gunner_move_run;
}

mframe_t gunner_frames_runandshoot [] =
{
    ai_run, 32, NULL,
    ai_run, 15, NULL,
    ai_run, 10, NULL,
    ai_run, 18, NULL,
    ai_run, 8,  NULL,
    ai_run, 20, NULL
};

mmove_t gunner_move_runandshoot = {FRAME_runs01, FRAME_runs06, gunner_frames_runandshoot, NULL};

void gunner_runandshoot (edict *self)
{
    self->monsterinfoVal.currentmove = &gunner_move_runandshoot;
}

mframe_t gunner_frames_pain3 [] =
{
    ai_move, -3, NULL,
    ai_move, 1,     NULL,
    ai_move, 1,     NULL,
    ai_move, 0,     NULL,
    ai_move, 1,     NULL
};
mmove_t gunner_move_pain3 = {FRAME_pain301, FRAME_pain305, gunner_frames_pain3, gunner_run};

mframe_t gunner_frames_pain2 [] =
{
    ai_move, -2, NULL,
    ai_move, 11, NULL,
    ai_move, 6,     NULL,
    ai_move, 2,     NULL,
    ai_move, -1, NULL,
    ai_move, -7, NULL,
    ai_move, -2, NULL,
    ai_move, -7, NULL
};
mmove_t gunner_move_pain2 = {FRAME_pain201, FRAME_pain208, gunner_frames_pain2, gunner_run};

mframe_t gunner_frames_pain1 [] =
{
    ai_move, 2,     NULL,
    ai_move, 0,     NULL,
    ai_move, -5, NULL,
    ai_move, 3,     NULL,
    ai_move, -1, NULL,
    ai_move, 0,     NULL,
    ai_move, 0,     NULL,
    ai_move, 0,     NULL,
    ai_move, 0,     NULL,
    ai_move, 1,     NULL,
    ai_move, 1,     NULL,
    ai_move, 2,     NULL,
    ai_move, 1,     NULL,
    ai_move, 0,     NULL,
    ai_move, -2, NULL,
    ai_move, -2, NULL,
    ai_move, 0,     NULL,
    ai_move, 0,     NULL
};
mmove_t gunner_move_pain1 = {FRAME_pain101, FRAME_pain118, gunner_frames_pain1, gunner_run};

void gunner_pain (edict *self, edict *other, float kick, int damage)
{
    if (self->health < (self->max_health / 2))
        self->s.skinnum = 1;

    if (level.time < self->pain_debounce_time)
        return;

    self->pain_debounce_time = level.time + 3;

    if (rand()&1)
        quake2::getInstance()->gi.sound (self, CHAN_VOICE, sound_pain, 1, ATTN_NORM, 0);
    else
        quake2::getInstance()->gi.sound (self, CHAN_VOICE, sound_pain2, 1, ATTN_NORM, 0);

    if (skill->value == 3)
        return;        // no pain anims in nightmare

    if (damage <= 10)
        self->monsterinfoVal.currentmove = &gunner_move_pain3;
    else if (damage <= 25)
        self->monsterinfoVal.currentmove = &gunner_move_pain2;
    else
        self->monsterinfoVal.currentmove = &gunner_move_pain1;
}

void gunner_dead (edict *self)
{
    VectorSet (self->mins, -16, -16, -24);
    VectorSet (self->maxs, 16, 16, -8);
    self->movetype = MOVETYPE_TOSS;
    self->svflags |= SVF_DEADMONSTER;
    self->nextthink = 0;
    quake2::getInstance()->gi.linkentity (self);
}

mframe_t gunner_frames_death [] =
{
    ai_move, 0,     NULL,
    ai_move, 0,     NULL,
    ai_move, 0,     NULL,
    ai_move, -7, NULL,
    ai_move, -3, NULL,
    ai_move, -5, NULL,
    ai_move, 8,     NULL,
    ai_move, 6,     NULL,
    ai_move, 0,     NULL,
    ai_move, 0,     NULL,
    ai_move, 0,     NULL
};
mmove_t gunner_move_death = {FRAME_death01, FRAME_death11, gunner_frames_death, gunner_dead};

void gunner_die (edict *self, edict *inflictor, edict *attacker, int damage, vec3_t point)
{
    int        n;

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
    self->monsterinfoVal.currentmove = &gunner_move_death;
}


void gunner_duck_down (edict *self)
{
    if (self->monsterinfoVal.aiflags & AI_DUCKED)
        return;
    self->monsterinfoVal.aiflags |= AI_DUCKED;
    if (skill->value >= 2)
    {
        if (random() > 0.5)
            GunnerGrenade (self);
    }

    self->maxs[2] -= 32;
    self->takedamage = DAMAGE_YES;
    self->monsterinfoVal.pausetime = level.time + 1;
    quake2::getInstance()->gi.linkentity (self);
}

void gunner_duck_hold (edict *self)
{
    if (level.time >= self->monsterinfoVal.pausetime)
        self->monsterinfoVal.aiflags &= ~AI_HOLD_FRAME;
    else
        self->monsterinfoVal.aiflags |= AI_HOLD_FRAME;
}

void gunner_duck_up (edict *self)
{
    self->monsterinfoVal.aiflags &= ~AI_DUCKED;
    self->maxs[2] += 32;
    self->takedamage = DAMAGE_AIM;
    quake2::getInstance()->gi.linkentity (self);
}

mframe_t gunner_frames_duck [] =
{
    ai_move, 1,  gunner_duck_down,
    ai_move, 1,  NULL,
    ai_move, 1,  gunner_duck_hold,
    ai_move, 0,  NULL,
    ai_move, -1, NULL,
    ai_move, -1, NULL,
    ai_move, 0,  gunner_duck_up,
    ai_move, -1, NULL
};
mmove_t    gunner_move_duck = {FRAME_duck01, FRAME_duck08, gunner_frames_duck, gunner_run};

void gunner_dodge (edict *self, edict *attacker, float eta)
{
    if (random() > 0.25)
        return;

    if (!self->enemy)
        self->enemy = attacker;

    self->monsterinfoVal.currentmove = &gunner_move_duck;
}


void gunner_opengun (edict *self)
{
    quake2::getInstance()->gi.sound (self, CHAN_VOICE, sound_open, 1, ATTN_IDLE, 0);
}

void GunnerFire (edict *self)
{
    vec3_t    start;
    vec3_t    forward, right;
    vec3_t    target;
    vec3_t    aim;
    int        flash_number;

    flash_number = MZ2_GUNNER_MACHINEGUN_1 + (self->s.frame - FRAME_attak216);

    AngleVectors (self->s.angles, forward, right, NULL);
    G_ProjectSource (self->s.origin, monster_flash_offset[flash_number], forward, right, start);

    // project enemy back a bit and target there
    VectorCopy (self->enemy->s.origin, target);
    VectorMA (target, -0.2, self->enemy->velocity, target);
    target[2] += self->enemy->viewheight;

    VectorSubtract (target, start, aim);
    VectorNormalize (aim);
    monster_fire_bullet (self, start, aim, 3, 4, DEFAULT_BULLET_HSPREAD, DEFAULT_BULLET_VSPREAD, flash_number);
}

void GunnerGrenade (edict *self)
{
    vec3_t    start;
    vec3_t    forward, right;
    vec3_t    aim;
    int        flash_number;

    if (self->s.frame == FRAME_attak105)
        flash_number = MZ2_GUNNER_GRENADE_1;
    else if (self->s.frame == FRAME_attak108)
        flash_number = MZ2_GUNNER_GRENADE_2;
    else if (self->s.frame == FRAME_attak111)
        flash_number = MZ2_GUNNER_GRENADE_3;
    else // (self->s.frame == FRAME_attak114)
        flash_number = MZ2_GUNNER_GRENADE_4;

    AngleVectors (self->s.angles, forward, right, NULL);
    G_ProjectSource (self->s.origin, monster_flash_offset[flash_number], forward, right, start);

    //FIXME : do a spread -225 -75 75 225 degrees around forward
    VectorCopy (forward, aim);

    monster_fire_grenade (self, start, aim, 50, 600, flash_number);
}

mframe_t gunner_frames_attack_chain [] =
{
    /*
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    */
    ai_charge, 0, gunner_opengun,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL
};
mmove_t gunner_move_attack_chain = {FRAME_attak209, FRAME_attak215, gunner_frames_attack_chain, gunner_fire_chain};

mframe_t gunner_frames_fire_chain [] =
{
    ai_charge,   0, GunnerFire,
    ai_charge,   0, GunnerFire,
    ai_charge,   0, GunnerFire,
    ai_charge,   0, GunnerFire,
    ai_charge,   0, GunnerFire,
    ai_charge,   0, GunnerFire,
    ai_charge,   0, GunnerFire,
    ai_charge,   0, GunnerFire
};
mmove_t gunner_move_fire_chain = {FRAME_attak216, FRAME_attak223, gunner_frames_fire_chain, gunner_refire_chain};

mframe_t gunner_frames_endfire_chain [] =
{
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL
};
mmove_t gunner_move_endfire_chain = {FRAME_attak224, FRAME_attak230, gunner_frames_endfire_chain, gunner_run};

mframe_t gunner_frames_attack_grenade [] =
{
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, GunnerGrenade,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, GunnerGrenade,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, GunnerGrenade,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, GunnerGrenade,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL,
    ai_charge, 0, NULL
};
mmove_t gunner_move_attack_grenade = {FRAME_attak101, FRAME_attak121, gunner_frames_attack_grenade, gunner_run};

void gunner_attack(edict *self)
{
    if (range (self, self->enemy) == RANGE_MELEE)
    {
        self->monsterinfoVal.currentmove = &gunner_move_attack_chain;
    }
    else
    {
        if (random() <= 0.5)
            self->monsterinfoVal.currentmove = &gunner_move_attack_grenade;
        else
            self->monsterinfoVal.currentmove = &gunner_move_attack_chain;
    }
}

void gunner_fire_chain(edict *self)
{
    self->monsterinfoVal.currentmove = &gunner_move_fire_chain;
}

void gunner_refire_chain(edict *self)
{
    if (self->enemy->health > 0)
        if ( visible (self, self->enemy) )
            if (random() <= 0.5)
            {
                self->monsterinfoVal.currentmove = &gunner_move_fire_chain;
                return;
            }
    self->monsterinfoVal.currentmove = &gunner_move_endfire_chain;
}

/*QUAKED monster_gunner (1 .5 0) (-16 -16 -24) (16 16 32) Ambush Trigger_Spawn Sight
*/
void SP_monster_gunner (edict *self)
{
    if (deathmatch->value)
    {
        G_FreeEdict (self);
        return;
    }

    sound_death = quake2::getInstance()->gi.soundindex ("gunner/death1.wav");
    sound_pain = quake2::getInstance()->gi.soundindex ("gunner/gunpain2.wav");
    sound_pain2 = quake2::getInstance()->gi.soundindex ("gunner/gunpain1.wav");
    sound_idle = quake2::getInstance()->gi.soundindex ("gunner/gunidle1.wav");
    sound_open = quake2::getInstance()->gi.soundindex ("gunner/gunatck1.wav");
    sound_search = quake2::getInstance()->gi.soundindex ("gunner/gunsrch1.wav");
    sound_sight = quake2::getInstance()->gi.soundindex ("gunner/sight1.wav");

    quake2::getInstance()->gi.soundindex ("gunner/gunatck2.wav");
    quake2::getInstance()->gi.soundindex ("gunner/gunatck3.wav");

    self->movetype = MOVETYPE_STEP;
    self->solid = SOLID_BBOX;
    self->s.modelindex = quake2::getInstance()->gi.modelindex ("models/monsters/gunner/tris.md2");
    VectorSet (self->mins, -16, -16, -24);
    VectorSet (self->maxs, 16, 16, 32);

    self->health = 175;
    self->gib_health = -70;
    self->mass = 200;

    self->pain = gunner_pain;
    self->die = gunner_die;

    self->monsterinfoVal.stand = gunner_stand;
    self->monsterinfoVal.walk = gunner_walk;
    self->monsterinfoVal.run = gunner_run;
    self->monsterinfoVal.dodge = gunner_dodge;
    self->monsterinfoVal.attack = gunner_attack;
    self->monsterinfoVal.melee = NULL;
    self->monsterinfoVal.sight = gunner_sight;
    self->monsterinfoVal.search = gunner_search;

    quake2::getInstance()->gi.linkentity (self);

    self->monsterinfoVal.currentmove = &gunner_move_stand;
    self->monsterinfoVal.scale = MODEL_SCALE;

    walkmonster_start (self);
}
