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


void InitTrigger (edict *self)
{
    if (!VectorCompare (self->s.angles, vec3_origin))
        G_SetMovedir (self->s.angles, self->movedir);

    self->solid = SOLID_TRIGGER;
    self->movetype = MOVETYPE_NONE;
    quake2::getInstance()->gi.setmodel (self, self->model);
    self->svflags = SVF_NOCLIENT;
}


// the wait time has passed, so set back up for another activation
void multi_wait (edict *ent)
{
    ent->nextthink = 0;
}


// the trigger was just activated
// ent->activator should be set to the activator so it can be held through a delay
// so wait for the delay time before firing
void multi_trigger (edict *ent)
{
    if (ent->nextthink)
        return;        // already been triggered

    G_UseTargets (ent, ent->activator);

    if (ent->wait > 0)
    {
        ent->think = multi_wait;
        ent->nextthink = level.time + ent->wait;
    }
    else
    {    // we can't just remove (self) here, because this is a touch function
        // called while looping through area links...
        ent->touch = NULL;
        ent->nextthink = level.time + FRAMETIME;
        ent->think = G_FreeEdict;
    }
}

void Use_Multi (edict *ent, edict *other, edict *activator)
{
    ent->activator = activator;
    multi_trigger (ent);
}

void Touch_Multi (edict *self, edict *other, plane_s *plane, csurface_s *surf)
{
    if(other->client)
    {
        if (self->spawnflags & 2)
            return;
    }
    else if (other->svflags & SVF_MONSTER)
    {
        if (!(self->spawnflags & 1))
            return;
    }
    else
        return;

    if (!VectorCompare(self->movedir, vec3_origin))
    {
        vec3_t    forward;

        AngleVectors(other->s.angles, forward, NULL, NULL);
        if (_DotProduct(forward, self->movedir) < 0)
            return;
    }

    self->activator = other;
    multi_trigger (self);
}

/*QUAKED trigger_multiple (.5 .5 .5) ? MONSTER NOT_PLAYER TRIGGERED
Variable sized repeatable trigger.  Must be targeted at one or more entities.
If "delay" is set, the trigger waits some time after activating before firing.
"wait" : Seconds between triggerings. (.2 default)
sounds
1)    secret
2)    beep beep
3)    large switch
4)
set "message" to text string
*/
void trigger_enable (edict *self, edict *other, edict *activator)
{
    self->solid = SOLID_TRIGGER;
    self->use = Use_Multi;
    quake2::getInstance()->gi.linkentity (self);
}

void SP_trigger_multiple (edict *ent)
{
    if (ent->sounds == 1)
        ent->noise_index = quake2::getInstance()->gi.soundindex ("misc/secret.wav");
    else if (ent->sounds == 2)
        ent->noise_index = quake2::getInstance()->gi.soundindex ("misc/talk.wav");
    else if (ent->sounds == 3)
        ent->noise_index = quake2::getInstance()->gi.soundindex ("misc/trigger1.wav");

    if (!ent->wait)
        ent->wait = 0.2;
    ent->touch = Touch_Multi;
    ent->movetype = MOVETYPE_NONE;
    ent->svflags |= SVF_NOCLIENT;


    if (ent->spawnflags & 4)
    {
        ent->solid = SOLID_NOT;
        ent->use = trigger_enable;
    }
    else
    {
        ent->solid = SOLID_TRIGGER;
        ent->use = Use_Multi;
    }

    if (!VectorCompare(ent->s.angles, vec3_origin))
        G_SetMovedir (ent->s.angles, ent->movedir);

    quake2::getInstance()->gi.setmodel (ent, ent->model);
    quake2::getInstance()->gi.linkentity (ent);
}


/*QUAKED trigger_once (.5 .5 .5) ? x x TRIGGERED
Triggers once, then removes itself.
You must set the key "target" to the name of another object in the level that has a matching "targetname".

If TRIGGERED, this trigger must be triggered before it is live.

sounds
 1)    secret
 2)    beep beep
 3)    large switch
 4)

"message"    string to be displayed when triggered
*/

void SP_trigger_once(edict *ent)
{
    // make old maps work because I messed up on flag assignments here
    // triggered was on bit 1 when it should have been on bit 4
    if (ent->spawnflags & 1)
    {
        vec3_t    v;

        VectorMA (ent->mins, 0.5, ent->size, v);
        ent->spawnflags &= ~1;
        ent->spawnflags |= 4;
        quake2::getInstance()->gi.dprintf("fixed TRIGGERED flag on %s at %s\n", ent->classname, vtos(v));
    }

    ent->wait = -1;
    SP_trigger_multiple (ent);
}

/*QUAKED trigger_relay (.5 .5 .5) (-8 -8 -8) (8 8 8)
This fixed size trigger cannot be touched, it can only be fired by other events.
*/
void trigger_relay_use (edict *self, edict *other, edict *activator)
{
    G_UseTargets (self, activator);
}

void SP_trigger_relay (edict *self)
{
    self->use = trigger_relay_use;
}


/*
==============================================================================

trigger_key

==============================================================================
*/

/*QUAKED trigger_key (.5 .5 .5) (-8 -8 -8) (8 8 8)
A relay trigger that only fires it's targets if player has the proper key.
Use "item" to specify the required key, for example "key_data_cd"
*/
void trigger_key_use (edict *self, edict *other, edict *activator)
{
    int            index;

    if (!self->item)
        return;
    if (!activator->client)
        return;

    index = ITEM_INDEX(self->item);
    if (!activator->client->pers.inventory[index])
    {
        if (level.time < self->touch_debounce_time)
            return;
        self->touch_debounce_time = level.time + 5.0;
        quake2::getInstance()->gi.centerprintf (activator, "You need the %s", self->item->pickup_name);
        quake2::getInstance()->gi.sound (activator, CHAN_AUTO, quake2::getInstance()->gi.soundindex ("misc/keytry.wav"), 1, ATTN_NORM, 0);
        return;
    }

    quake2::getInstance()->gi.sound (activator, CHAN_AUTO, quake2::getInstance()->gi.soundindex ("misc/keyuse.wav"), 1, ATTN_NORM, 0);
    if (coop->value)
    {
        int        player;
        edict    *ent;

        if (strcmp(self->item->classname, "key_power_cube") == 0)
        {
            int    cube;

            for (cube = 0; cube < 8; cube++)
                if (activator->client->pers.power_cubes & (1 << cube))
                    break;
            for (player = 1; player <= game.maxclients; player++)
            {
                ent = &g_edicts[player];
                if (!ent->inuse)
                    continue;
                if (!ent->client)
                    continue;
                if (ent->client->pers.power_cubes & (1 << cube))
                {
                    ent->client->pers.inventory[index]--;
                    ent->client->pers.power_cubes &= ~(1 << cube);
                }
            }
        }
        else
        {
            for (player = 1; player <= game.maxclients; player++)
            {
                ent = &g_edicts[player];
                if (!ent->inuse)
                    continue;
                if (!ent->client)
                    continue;
                ent->client->pers.inventory[index] = 0;
            }
        }
    }
    else
    {
        activator->client->pers.inventory[index]--;
    }

    G_UseTargets (self, activator);

    self->use = NULL;
}

void SP_trigger_key (edict *self)
{
    if (!st.item)
    {
        quake2::getInstance()->gi.dprintf("no key item for trigger_key at %s\n", vtos(self->s.origin));
        return;
    }
    self->item = FindItemByClassname (st.item);

    if (!self->item)
    {
        quake2::getInstance()->gi.dprintf("item %s not found for trigger_key at %s\n", st.item, vtos(self->s.origin));
        return;
    }

    if (!self->target)
    {
        quake2::getInstance()->gi.dprintf("%s at %s has no target\n", self->classname, vtos(self->s.origin));
        return;
    }

    quake2::getInstance()->gi.soundindex ("misc/keytry.wav");
    quake2::getInstance()->gi.soundindex ("misc/keyuse.wav");

    self->use = trigger_key_use;
}


/*
==============================================================================

trigger_counter

==============================================================================
*/

/*QUAKED trigger_counter (.5 .5 .5) ? nomessage
Acts as an intermediary for an action that takes multiple inputs.

If nomessage is not set, t will print "1 more.. " etc when triggered and "sequence complete" when finished.

After the counter has been triggered "count" times (default 2), it will fire all of it's targets and remove itself.
*/

void trigger_counter_use(edict *self, edict *other, edict *activator)
{
    if (self->count == 0)
        return;

    self->count--;

    if (self->count)
    {
        if (! (self->spawnflags & 1))
        {
            quake2::getInstance()->gi.centerprintf(activator, "%i more to go...", self->count);
            quake2::getInstance()->gi.sound (activator, CHAN_AUTO, quake2::getInstance()->gi.soundindex ("misc/talk1.wav"), 1, ATTN_NORM, 0);
        }
        return;
    }

    if (! (self->spawnflags & 1))
    {
        quake2::getInstance()->gi.centerprintf(activator, "Sequence completed!");
        quake2::getInstance()->gi.sound (activator, CHAN_AUTO, quake2::getInstance()->gi.soundindex ("misc/talk1.wav"), 1, ATTN_NORM, 0);
    }
    self->activator = activator;
    multi_trigger (self);
}

void SP_trigger_counter (edict *self)
{
    self->wait = -1;
    if (!self->count)
        self->count = 2;

    self->use = trigger_counter_use;
}


/*
==============================================================================

trigger_always

==============================================================================
*/

/*QUAKED trigger_always (.5 .5 .5) (-8 -8 -8) (8 8 8)
This trigger will always fire.  It is activated by the world.
*/
void SP_trigger_always (edict *ent)
{
    // we must have some delay to make sure our use targets are present
    if (ent->delay < 0.2)
        ent->delay = 0.2;
    G_UseTargets(ent, ent);
}


/*
==============================================================================

trigger_push

==============================================================================
*/

#define PUSH_ONCE        1

static int windsound;

void trigger_push_touch (edict *self, edict *other, plane_s *plane, csurface_s *surf)
{
    if (strcmp(other->classname, "grenade") == 0)
    {
        VectorScale (self->movedir, self->speed * 10, other->velocity);
    }
    else if (other->health > 0)
    {
        VectorScale (self->movedir, self->speed * 10, other->velocity);

        if (other->client)
        {
            // don't take falling damage immediately from this
            VectorCopy (other->velocity, other->client->oldvelocity);
            if (other->fly_sound_debounce_time < level.time)
            {
                other->fly_sound_debounce_time = level.time + 1.5;
                quake2::getInstance()->gi.sound (other, CHAN_AUTO, windsound, 1, ATTN_NORM, 0);
            }
        }
    }
    if (self->spawnflags & PUSH_ONCE)
        G_FreeEdict (self);
}


/*QUAKED trigger_push (.5 .5 .5) ? PUSH_ONCE
Pushes the player
"speed"        defaults to 1000
*/
void SP_trigger_push (edict *self)
{
    InitTrigger (self);
    windsound = quake2::getInstance()->gi.soundindex ("misc/windfly.wav");
    self->touch = trigger_push_touch;
    if (!self->speed)
        self->speed = 1000;
    quake2::getInstance()->gi.linkentity (self);
}


/*
==============================================================================

trigger_hurt

==============================================================================
*/

/*QUAKED trigger_hurt (.5 .5 .5) ? START_OFF TOGGLE SILENT NO_PROTECTION SLOW
Any entity that touches this will be hurt.

It does dmg points of damage each server frame

SILENT            supresses playing the sound
SLOW            changes the damage rate to once per second
NO_PROTECTION    *nothing* stops the damage

"dmg"            default 5 (whole numbers only)

*/
void hurt_use (edict *self, edict *other, edict *activator)
{
    if (self->solid == SOLID_NOT)
        self->solid = SOLID_TRIGGER;
    else
        self->solid = SOLID_NOT;
    quake2::getInstance()->gi.linkentity (self);

    if (!(self->spawnflags & 2))
        self->use = NULL;
}


void hurt_touch (edict *self, edict *other, plane_s *plane, csurface_s *surf)
{
    int        dflags;

    if (!other->takedamage)
        return;

    if (self->timestamp > level.time)
        return;

    if (self->spawnflags & 16)
        self->timestamp = level.time + 1;
    else
        self->timestamp = level.time + FRAMETIME;

    if (!(self->spawnflags & 4))
    {
        if ((level.framenum % 10) == 0)
            quake2::getInstance()->gi.sound (other, CHAN_AUTO, self->noise_index, 1, ATTN_NORM, 0);
    }

    if (self->spawnflags & 8)
        dflags = DAMAGE_NO_PROTECTION;
    else
        dflags = 0;
    T_Damage (other, self, self, vec3_origin, other->s.origin, vec3_origin, self->dmg, self->dmg, dflags, MOD_TRIGGER_HURT);
}

void SP_trigger_hurt (edict *self)
{
    InitTrigger (self);

    self->noise_index = quake2::getInstance()->gi.soundindex ("world/electro.wav");
    self->touch = hurt_touch;

    if (!self->dmg)
        self->dmg = 5;

    if (self->spawnflags & 1)
        self->solid = SOLID_NOT;
    else
        self->solid = SOLID_TRIGGER;

    if (self->spawnflags & 2)
        self->use = hurt_use;

    quake2::getInstance()->gi.linkentity (self);
}


/*
==============================================================================

trigger_gravity

==============================================================================
*/

/*QUAKED trigger_gravity (.5 .5 .5) ?
Changes the touching entites gravity to
the value of "gravity".  1.0 is standard
gravity for the level.
*/

void trigger_gravity_touch (edict *self, edict *other, plane_s *plane, csurface_s *surf)
{
    other->gravity = self->gravity;
}

void SP_trigger_gravity (edict *self)
{
    if (st.gravity == 0)
    {
        quake2::getInstance()->gi.dprintf("trigger_gravity without gravity set at %s\n", vtos(self->s.origin));
        G_FreeEdict  (self);
        return;
    }

    InitTrigger (self);
    self->gravity = atoi(st.gravity);
    self->touch = trigger_gravity_touch;
}


/*
==============================================================================

trigger_monsterjump

==============================================================================
*/

/*QUAKED trigger_monsterjump (.5 .5 .5) ?
Walking monsters that touch this will jump in the direction of the trigger's angle
"speed" default to 200, the speed thrown forward
"height" default to 200, the speed thrown upwards
*/

void trigger_monsterjump_touch (edict *self, edict *other, plane_s *plane, csurface_s *surf)
{
    if (other->flags & (FL_FLY | FL_SWIM) )
        return;
    if (other->svflags & SVF_DEADMONSTER)
        return;
    if ( !(other->svflags & SVF_MONSTER))
        return;

// set XY even if not on ground, so the jump will clear lips
    other->velocity[0] = self->movedir[0] * self->speed;
    other->velocity[1] = self->movedir[1] * self->speed;

    if (!other->groundentity)
        return;

    other->groundentity = NULL;
    other->velocity[2] = self->movedir[2];
}

void SP_trigger_monsterjump (edict *self)
{
    if (!self->speed)
        self->speed = 200;
    if (!st.height)
        st.height = 200;
    if (self->s.angles[YAW] == 0)
        self->s.angles[YAW] = 360;
    InitTrigger (self);
    self->touch = trigger_monsterjump_touch;
    self->movedir[2] = st.height;
}

