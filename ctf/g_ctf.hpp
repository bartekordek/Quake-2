#ifndef ____CTF_G_CTF_H__
#define ____CTF_G_CTF_H__

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

#define CTF_VERSION			1.09b
#define CTF_VSTRING2(x) #x
#define CTF_VSTRING(x) CTF_VSTRING2(x)
#define CTF_STRING_VERSION  CTF_VSTRING(CTF_VERSION)

#define STAT_CTF_TEAM1_PIC			17
#define STAT_CTF_TEAM1_CAPS			18
#define STAT_CTF_TEAM2_PIC			19
#define STAT_CTF_TEAM2_CAPS			20
#define STAT_CTF_FLAG_PIC			21
#define STAT_CTF_JOINED_TEAM1_PIC	22
#define STAT_CTF_JOINED_TEAM2_PIC	23
#define STAT_CTF_TEAM1_HEADER		24
#define STAT_CTF_TEAM2_HEADER		25
#define STAT_CTF_TECH				26
#define STAT_CTF_ID_VIEW			27
#define STAT_CTF_MATCH				28

#define CONFIG_CTF_MATCH (CS_MAXCLIENTS-1)

typedef enum {
	CTF_NOTEAM,
	CTF_TEAM1,
	CTF_TEAM2
} ctfteam_t;

typedef enum {
	CTF_GRAPPLE_STATE_FLY,
	CTF_GRAPPLE_STATE_PULL,
	CTF_GRAPPLE_STATE_HANG
} ctfgrapplestate_t;

typedef struct ghost_s {
	char netname[16];
	int number;

	// stats
	int deaths;
	int kills;
	int caps;
	int basedef;
	int carrierdef;

	int code; // ghost code
	int team; // team
	int score; // frags at time of disconnect
	struct edict_s *ent;
} ghost_t;

extern cvar_t *ctf;

#define CTF_TEAM1_SKIN "ctf_r"
#define CTF_TEAM2_SKIN "ctf_b"

#define DF_CTF_FORCEJOIN	131072
#define DF_ARMOR_PROTECT	262144
#define DF_CTF_NO_TECH      524288

#define CTF_CAPTURE_BONUS		15	// what you get for capture
#define CTF_TEAM_BONUS			10	// what your team gets for capture
#define CTF_RECOVERY_BONUS		1	// what you get for recovery
#define CTF_FLAG_BONUS			0	// what you get for picking up enemy flag
#define CTF_FRAG_CARRIER_BONUS	2	// what you get for fragging enemy flag carrier
#define CTF_FLAG_RETURN_TIME	40	// seconds until auto return

#define CTF_CARRIER_DANGER_PROTECT_BONUS	2	// bonus for fraggin someone who has recently hurt your flag carrier
#define CTF_CARRIER_PROTECT_BONUS			1	// bonus for fraggin someone while either you or your target are near your flag carrier
#define CTF_FLAG_DEFENSE_BONUS				1	// bonus for fraggin someone while either you or your target are near your flag
#define CTF_RETURN_FLAG_ASSIST_BONUS		1	// awarded for returning a flag that causes a capture to happen almost immediately
#define CTF_FRAG_CARRIER_ASSIST_BONUS		2	// award for fragging a flag carrier if a capture happens almost immediately

#define CTF_TARGET_PROTECT_RADIUS			400	// the radius around an object being defended where a target will be worth extra frags
#define CTF_ATTACKER_PROTECT_RADIUS			400	// the radius around an object being defended where an attacker will get extra frags when making kills

#define CTF_CARRIER_DANGER_PROTECT_TIMEOUT	8
#define CTF_FRAG_CARRIER_ASSIST_TIMEOUT		10
#define CTF_RETURN_FLAG_ASSIST_TIMEOUT		10

#define CTF_AUTO_FLAG_RETURN_TIMEOUT		30	// number of seconds before dropped flag auto-returns

#define CTF_TECH_TIMEOUT					60  // seconds before techs spawn again

#define CTF_GRAPPLE_SPEED					650 // speed of grapple in flight
#define CTF_GRAPPLE_PULL_SPEED				650	// speed player is pulled at

void CTFInit(void);
void CTFSpawn(void);

void SP_info_player_team1(struct edict_s *self);
void SP_info_player_team2(struct edict_s *self);

char *CTFTeamName(int team);
char *CTFOtherTeamName(int team);
void CTFAssignSkin(struct edict_s *ent, char *s);
void CTFAssignTeam(gclient_t *who);
struct edict_s *SelectCTFSpawnPoint (struct edict_s *ent);
bool CTFPickup_Flag(struct edict_s *ent, struct edict_s *other);
bool CTFDrop_Flag(struct edict_s *ent, gitem_t *item);
void CTFEffects(struct edict_s *player);
void CTFCalcScores(void);
void SetCTFStats(struct edict_s *ent);
void CTFDeadDropFlag(struct edict_s *self);
void CTFScoreboardMessage (struct edict_s *ent, struct edict_s *killer);
void CTFTeam_f (struct edict_s *ent);
void CTFID_f (struct edict_s *ent);
void CTFSay_Team(struct edict_s *who, char *msg);
void CTFFlagSetup (struct edict_s *ent);
void CTFResetFlag(int ctf_team);
void CTFFragBonuses(struct edict_s *targ, struct edict_s *inflictor, struct edict_s *attacker);
void CTFCheckHurtCarrier(struct edict_s *targ, struct edict_s *attacker);

// GRAPPLE
void CTFWeapon_Grapple (struct edict_s *ent);
void CTFPlayerResetGrapple(struct edict_s *ent);
void CTFGrapplePull(struct edict_s *self);
void CTFResetGrapple(struct edict_s *self);

//TECH
gitem_t *CTFWhat_Tech(struct edict_s *ent);
bool CTFPickup_Tech (struct edict_s *ent, struct edict_s *other);
void CTFDrop_Tech(struct edict_s *ent, gitem_t *item);
void CTFDeadDropTech(struct edict_s *ent);
void CTFSetupTechSpawn(void);
int CTFApplyResistance(struct edict_s *ent, int dmg);
int CTFApplyStrength(struct edict_s *ent, int dmg);
bool CTFApplyStrengthSound(struct edict_s *ent);
bool CTFApplyHaste(struct edict_s *ent);
void CTFApplyHasteSound(struct edict_s *ent);
void CTFApplyRegeneration(struct edict_s *ent);
bool CTFHasRegeneration(struct edict_s *ent);
void CTFRespawnTech(struct edict_s *ent);
void CTFResetTech(void);

void CTFOpenJoinMenu(struct edict_s *ent);
bool CTFStartClient(struct edict_s *ent);
void CTFVoteYes(struct edict_s *ent);
void CTFVoteNo(struct edict_s *ent);
void CTFReady(struct edict_s *ent);
void CTFNotReady(struct edict_s *ent);
bool CTFNextMap(void);
bool CTFMatchSetup(void);
bool CTFMatchOn(void);
void CTFGhost(struct edict_s *ent);
void CTFAdmin(struct edict_s *ent);
bool CTFInMatch(void);
void CTFStats(struct edict_s *ent);
void CTFWarp(struct edict_s *ent);
void CTFBoot(struct edict_s *ent);
void CTFPlayerList(struct edict_s *ent);

bool CTFCheckRules(void);

void SP_misc_ctf_banner (struct edict_s *ent);
void SP_misc_ctf_small_banner (struct edict_s *ent);

extern char *ctf_statusbar;

void UpdateChaseCam(struct edict_s *ent);
void ChaseNext(struct edict_s *ent);
void ChasePrev(struct edict_s *ent);

void CTFObserver(struct edict_s *ent);

void SP_trigger_teleport (struct edict_s *ent);
void SP_info_teleport_destination (struct edict_s *ent);

#endif // ____CTF_G_CTF_H__