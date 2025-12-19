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
// cvar.c -- dynamic variable tracking

#include "qcommon/qcommon.h"
#include "shared/common.h"
#include <unordered_map>
#include <memory>
#include <string>

std::unordered_map<std::string, std::unique_ptr<cvar_t>> cvar_vars;

/*
============
Cvar_InfoValidate
============
*/
static qboolean Cvar_InfoValidate (const char *s)
{
	if (strstr (s, "\\"))
		return e_false;
	if (strstr (s, "\""))
		return e_false;
	if (strstr (s, ";"))
		return e_false;
	return e_true;
}

/*
============
Cvar_FindVar
============
*/
static cvar_t *Cvar_FindVar (const char *var_name)
{
	auto it = cvar_vars.find (var_name);
	if (it != cvar_vars.end ())
	{
		return it->second.get ();
	}

	return nullptr;
}

/*
============
Cvar_VariableValue
============
*/
float Cvar_VariableValue (const char *var_name)
{
	cvar_t *var = Cvar_FindVar (var_name);
	if (!var)
		return 0;
	return atof (var->string);
}


/*
============
Cvar_VariableString
============
*/
const char *Cvar_VariableString (const char *var_name)
{
	cvar_t *var = Cvar_FindVar (var_name);
	if (!var)
		return "";
	return var->string;
}


/*
============
Cvar_CompleteVariable
============
*/
const char *Cvar_CompleteVariable (const char *partial)
{
	if (!partial)
		return NULL;

	// check exact match
	auto it = cvar_vars.find (partial);
	if (it != cvar_vars.end())
	{
		return it->first.c_str();
	}

	// check partial match

	for (const auto&[key,_]  : cvar_vars)
	{
		if (key.contains(partial))
		{
			return key.c_str ();
		}
	}

	return nullptr;
}


/*
============
Cvar_Get

If the variable already exists, the value will not be set
The flags will be or'ed in if the variable exists.
============
*/
cvar_t *Cvar_Get (const char *var_name, const char *var_value, int flags)
{
	cvar_t *var{nullptr};

	if (flags & (CVAR_USERINFO | CVAR_SERVERINFO))
	{
		if (!Cvar_InfoValidate (var_name))
		{
			Com_Printf ("invalid info cvar name\n");
			return NULL;
		}
	}

	var = Cvar_FindVar (var_name);
	if (var)
	{
		var->flags |= flags;
		return var;
	}

	if (!var_value)
		return NULL;

	if (flags & (CVAR_USERINFO | CVAR_SERVERINFO))
	{
		if (!Cvar_InfoValidate (var_value))
		{
			Com_Printf ("invalid info cvar value\n");
			return NULL;
		}
	}

	cvar_vars.insert ({var_name, std::make_unique<cvar_t> ()});
	const std::unique_ptr<cvar_t> &varPtr = cvar_vars.at (var_name);
	var									  = varPtr.get ();

	var->name							  = CopyString (var_name);
	var->string							  = CopyString (var_value);
	var->modified						  = e_true;
	var->value							  = atof (var->string);

	var->flags							  = flags;

	return var;
}

/*
============
Cvar_Set2
============
*/
cvar_t *Cvar_Set2 (const char *var_name, const char *value, qboolean force)
{
	cvar_t	*var;

	var = Cvar_FindVar (var_name);
	if (!var)
	{	// create it
		return Cvar_Get (var_name, value, 0);
	}

	if (var->flags & (CVAR_USERINFO | CVAR_SERVERINFO))
	{
		if (!Cvar_InfoValidate (value))
		{
			Com_Printf("invalid info cvar value\n");
			return var;
		}
	}

	if (!force)
	{
		if (var->flags & CVAR_NOSET)
		{
			Com_Printf ("%s is write protected.\n", var_name);
			return var;
		}

		if (var->flags & CVAR_LATCH)
		{
			if (var->latched_string)
			{
				if (strcmp(value, var->latched_string) == 0)
					return var;
				Z_Free (var->latched_string);
			}
			else
			{
				if (strcmp(value, var->string) == 0)
					return var;
			}

			if (Com_ServerState())
			{
				Com_Printf ("%s will be changed for next game.\n", var_name);
				var->latched_string = CopyString(value);
			}
			else
			{
				var->string = CopyString(value);
				var->value = atof (var->string);
				if (!strcmp(var->name, "game"))
				{
					FS_SetGamedir (var->string);
					FS_ExecAutoexec ();
				}
			}
			return var;
		}
	}
	else
	{
		if (var->latched_string)
		{
			Z_Free (var->latched_string);
			var->latched_string = NULL;
		}
	}

	if (!strcmp(value, var->string))
		return var;		// not changed

	var->modified = e_true;

	if (var->flags & CVAR_USERINFO)
		userinfo_modified = e_true;	// transmit at next oportunity

	Z_Free (var->string);	// free the old value string

	var->string = CopyString(value);
	var->value = atof (var->string);

	return var;
}

/*
============
Cvar_ForceSet
============
*/
cvar_t *Cvar_ForceSet (char *var_name, char *value)
{
	return Cvar_Set2 (var_name, value, e_true);
}

/*
============
Cvar_Set
============
*/
cvar_t *Cvar_Set (const char *var_name, const char *value)
{
	return Cvar_Set2 (var_name, value, e_false);
}

/*
============
Cvar_FullSet
============
*/
cvar_t *Cvar_FullSet (const char *var_name, const char *value, int flags)
{
	cvar_t	*var;

	var = Cvar_FindVar (var_name);
	if (!var)
	{	// create it
		return Cvar_Get (var_name, value, flags);
	}

	var->modified = e_true;

	if (var->flags & CVAR_USERINFO)
		userinfo_modified = e_true;	// transmit at next oportunity

	Z_Free (var->string);	// free the old value string

	var->string = CopyString(value);
	var->value = atof (var->string);
	var->flags = flags;

	return var;
}

/*
============
Cvar_SetValue
============
*/
void Cvar_SetValue (char *var_name, float value)
{
	char	val[32];

	if (value == (int)value)
		Com_sprintf (val, sizeof(val), "%i",(int)value);
	else
		Com_sprintf (val, sizeof(val), "%f",value);
	Cvar_Set (var_name, val);
}


/*
============
Cvar_GetLatchedVars

Any variables with latched values will now be updated
============
*/
void Cvar_GetLatchedVars (void)
{
	for (auto &[_, var] : cvar_vars)
	{
		if (!var->latched_string)
			continue;
		Z_Free (var->string);
		var->string			= var->latched_string;
		var->latched_string = NULL;
		var->value			= atof (var->string);
		if (!strcmp (var->name, "game"))
		{
			FS_SetGamedir (var->string);
			FS_ExecAutoexec ();
		}
	}
}

/*
============
Cvar_Command

Handles variable inspection and changing from the console
============
*/
qboolean Cvar_Command (void)
{
	cvar_t			*v;

// check variables
	v = Cvar_FindVar (Cmd_Argv(0));
	if (!v)
		return e_false;

// perform a variable print or set
	if (Cmd_Argc() == 1)
	{
		Com_Printf ("\"%s\" is \"%s\"\n", v->name, v->string);
		return e_true;
	}

	Cvar_Set (v->name, Cmd_Argv(1));
	return e_true;
}


/*
============
Cvar_Set_f

Allows setting and defining of arbitrary cvars from console
============
*/
void Cvar_Set_f (void)
{
	int c;
	int flags;

	c = Cmd_Argc ();
	if (c != 3 && c != 4)
	{
		Com_Printf ("usage: set <variable> <value> [u / s]\n");
		return;
	}

	if (c == 4)
	{
		if (!strcmp (Cmd_Argv (3), "u"))
			flags = CVAR_USERINFO;
		else if (!strcmp (Cmd_Argv (3), "s"))
			flags = CVAR_SERVERINFO;
		else
		{
			Com_Printf ("flags can only be 'u' or 's'\n");
			return;
		}
		Cvar_FullSet (Cmd_Argv (1), Cmd_Argv (2), flags);
	}
	else
	{
		Cvar_Set (Cmd_Argv (1), Cmd_Argv (2));
	}
}


/*
============
Cvar_WriteVariables

Appends lines containing "set variable value" for all variables
with the archive flag set to e_true.
============
*/
void Cvar_WriteVariables (char *path)
{
	char  buffer[1024];
	FILE *f = fopen (path, "a");
	for (auto &[_, var] : cvar_vars)
	{
		if (var->flags & CVAR_ARCHIVE)
		{
			Com_sprintf (buffer, sizeof (buffer), "set %s \"%s\"\n", var->name, var->string);
			fprintf (f, "%s", buffer);
		}
	}
	fclose (f);
}

/*
============
Cvar_List_f

============
*/
void Cvar_List_f (void)
{
	std::size_t i{0u};
	for (auto &[_, var] : cvar_vars)
	{
		if (var->flags & CVAR_ARCHIVE)
			Com_Printf ("*");
		else
			Com_Printf (" ");
		if (var->flags & CVAR_USERINFO)
			Com_Printf ("U");
		else
			Com_Printf (" ");
		if (var->flags & CVAR_SERVERINFO)
			Com_Printf ("S");
		else
			Com_Printf (" ");
		if (var->flags & CVAR_NOSET)
			Com_Printf ("-");
		else if (var->flags & CVAR_LATCH)
			Com_Printf ("L");
		else
			Com_Printf (" ");
		Com_Printf (" %s \"%s\"\n", var->name, var->string);
		++i;
	}
	Com_Printf ("%i cvars\n", i);
}


qboolean userinfo_modified;


char	*Cvar_BitInfo (int bit)
{
	static char	info[MAX_INFO_STRING];

	info[0] = 0;

	for (auto &[_, var] : cvar_vars)
	{
		if (var->flags & bit)
			Info_SetValueForKey (info, var->name, var->string);
	}
	return info;
}

// returns an info string containing all the CVAR_USERINFO cvars
char *Cvar_Userinfo (void)
{
	return Cvar_BitInfo (CVAR_USERINFO);
}

// returns an info string containing all the CVAR_SERVERINFO cvars
char *Cvar_Serverinfo (void)
{
	return Cvar_BitInfo (CVAR_SERVERINFO);
}

/*
============
Cvar_Init

Reads in all archived cvars
============
*/
void Cvar_Init (void)
{
	Cmd_AddCommand ("set", Cvar_Set_f);
	Cmd_AddCommand ("cvarlist", Cvar_List_f);
}
