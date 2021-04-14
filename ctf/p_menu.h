#ifndef ____CTF_P_MENU_H__
#define ____CTF_P_MENU_H__

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

enum {
	PMENU_ALIGN_LEFT,
	PMENU_ALIGN_CENTER,
	PMENU_ALIGN_RIGHT
};

typedef struct pmenuhnd_s {
	struct pmenu_s *entries;
	int cur;
	int num;
	void *arg;
} pmenuhnd_t;

typedef void (*SelectFunc_t)(struct edict_s *ent, pmenuhnd_t *hnd);

typedef struct pmenu_s {
	char *text;
	int align;
	SelectFunc_t SelectFunc;
} pmenu_t;

pmenuhnd_t *PMenu_Open(struct edict_s *ent, pmenu_t *entries, int cur, int num, void *arg);
void PMenu_Close(struct edict_s *ent);
void PMenu_UpdateEntry(pmenu_t *entry, const char *text, int align, SelectFunc_t SelectFunc);
void PMenu_Do_Update(struct edict_s *ent);
void PMenu_Update(struct edict_s *ent);
void PMenu_Next(struct edict_s *ent);
void PMenu_Prev(struct edict_s *ent);
void PMenu_Select(struct edict_s *ent);

#endif // ____CTF_P_MENU_H__