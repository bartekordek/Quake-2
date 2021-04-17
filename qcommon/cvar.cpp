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

#include "qcommon.hpp"

cvar    *cvar_vars;

/*
============
Cvar_InfoValidate
============
*/
static bool Cvar_InfoValidate (char *s)
{
    if (strstr (s, "\\"))
        return false;
    if (strstr (s, "\""))
        return false;
    if (strstr (s, ";"))
        return false;
    return true;
}

/*
============
Cvar_FindVar
============
*/
static cvar *Cvar_FindVar(const std::string& var_name)
{
    cvar    *var;

    for (var=cvar_vars ; var ; var=var->next)
        if( var_name == var->name )
            return var;

    return nullptr;
}

/*
============
Cvar_VariableValue
============
*/
float Cvar_VariableValue (char *var_name)
{
    cvar    *var;

    var = Cvar_FindVar (var_name);
    if (!var)
        return 0;
    return std::stof (var->string);
}


/*
============
Cvar_VariableString
============
*/
char *Cvar_VariableString (char *var_name)
{
    cvar *var;

    var = Cvar_FindVar (var_name);
    if (!var)
        return "";
    return toChar( var->string );
}


/*
============
Cvar_CompleteVariable
============
*/
char *Cvar_CompleteVariable (char *partialVal)
{
    cvar        *cvar;
    int            len;

    len = strlen(partialVal);

    if (!len)
        return NULL;

    std::string partial = partialVal;
    // check exact match
    for (cvar=cvar_vars ; cvar ; cvar=cvar->next)
        if( partial == cvar->name )
            return toChar( cvar->name );

    // check partial match
    for (cvar=cvar_vars ; cvar ; cvar=cvar->next)
        if (!strncmp (partialVal, toChar(cvar->name ), len))
            return toChar( cvar->name );

    return NULL;
}


/*
============
Cvar_Get

If the variable already exists, the value will not be set
The flags will be or'ed in if the variable exists.
============
*/
cvar *Cvar_Get (char *var_name, char *var_value, int flags)
{
    cvar    *var;

    if (flags & (CVAR_USERINFO | CVAR_SERVERINFO))
    {
        if (!Cvar_InfoValidate (var_name))
        {
            Com_Printf_G("invalid info cvar name\n");
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
            Com_Printf_G("invalid info cvar value\n");
            return NULL;
        }
    }

    var = (cvar*) Z_Malloc (sizeof(*var));
    var->name = CopyString (var_name);
    var->string = CopyString (var_value);
    var->modified = true;
    var->value = std::stof (var->string);

    // link the variable in
    var->next = cvar_vars;
    cvar_vars = var;

    var->flags = flags;

    return var;
}

/*
============
Cvar_Set2
============
*/
cvar *Cvar_Set2(const std::string& var_name, const std::string& value, bool force)
{
    cvar    *var;

    var = Cvar_FindVar( toChar( var_name ) );
    if (!var)
    {    // create it
        return Cvar_Get( toChar( var_name ),  toChar(value), 0 );
    }

    if (var->flags & (CVAR_USERINFO | CVAR_SERVERINFO))
    {
        if (!Cvar_InfoValidate(toChar(value)))
        {
            Com_Printf_G("invalid info cvar value\n");
            return var;
        }
    }

    if (!force)
    {
        if (var->flags & CVAR_NOSET)
        {
            Com_Printf_G ("%s is write protected.\n", var_name);
            return var;
        }

        if (var->flags & CVAR_LATCH)
        {
            if (!var->latched_string.empty())
            {
                //if (strcmp(value, var->latched_string) == 0)
                if( value == var->latched_string )
                    return var;
                //Z_Free (var->latched_string);
                var->latched_string.clear();
            }
            else
            {
                //if (strcmp(value, var->string) == 0)
                if( value == var->string )
                    return var;
            }

            if (Com_ServerState())
            {
                Com_Printf_G ("%s will be changed for next game.\n", var_name);
                var->latched_string = value;
            }
            else
            {
                var->string = value;
                var->value = std::stof (var->string);
                //if (!strcmp(var->name, "game"))
                if( var->name == "game" )
                {
                    FS_SetGamedir (toChar( var->string ));
                    FS_ExecAutoexec ();
                }
            }
            return var;
        }
    }
    else
    {
        var->latched_string.clear();
    }

    if( var->string == value )
    {
        return var;
    }
    var->modified = true;

    if (var->flags & CVAR_USERINFO)
        userinfo_modified = true;    // transmit at next oportunity


    var->string = value;;
    var->value = std::stof (var->string);

    return var;
}

/*
============
Cvar_ForceSet
============
*/
cvar *Cvar_ForceSet (char* var_name, const std::string& value)
{
    return Cvar_Set2 (var_name, value, true);
}

/*
============
Cvar_Set
============
*/
cvar *Cvar_Set (char* var_name, char* value)
{
    return Cvar_Set2 (var_name, value, false);
}

/*
============
Cvar_FullSet
============
*/
cvar *Cvar_FullSet (const std::string& var_name, const std::string& value, int flags)
{
    cvar* var = Cvar_FindVar (var_name);
    if (!var)
    {    // create it
        return Cvar_Get (toChar( var_name ), toChar( value ), flags);
    }

    var->modified = true;

    if (var->flags & CVAR_USERINFO)
        userinfo_modified = true;    // transmit at next oportunity

    var->string = value;
    var->value = std::stof (var->string);
    var->flags = flags;

    return var;
}

/*
============
Cvar_SetValue
============
*/
void Cvar_SetValue ( char* var_name, float value)
{
    char    val[32];

    // if (value == (int)value)
    //     Com_sprintf (val, sizeof(val), "%i",(int)value);
    // else
    //     Com_sprintf (val, sizeof(val), "%f",value);
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
    cvar    *var;

    for (var = cvar_vars ; var ; var = var->next)
    {
        if( var->latched_string.empty() )
            continue;
        var->string = var->latched_string;
        var->value = std::stof(var->string);
        if (var->name=="game")
        {
            FS_SetGamedir (toChar( var->string ));
            FS_ExecAutoexec();
        }
    }
}

/*
============
Cvar_Command

Handles variable inspection and changing from the console
============
*/
bool Cvar_Command (void)
{
    cvar            *v;

// check variables
    v = Cvar_FindVar (Cmd_Argv(0));
    if (!v)
        return false;

// perform a variable print or set
    if (Cmd_Argc() == 1)
    {
        Com_Printf_G ("\"%s\" is \"%s\"\n", v->name, v->string);
        return true;
    }

    Cvar_Set( toChar( v->name ), Cmd_Argv( 1 ) );
    return true;
}


/*
============
Cvar_Set_f

Allows setting and defining of arbitrary cvars from console
============
*/
void Cvar_Set_f (void)
{
    int        c;
    int        flags;

    c = Cmd_Argc();
    if (c != 3 && c != 4)
    {
        Com_Printf_G ("usage: set <variable> <value> [u / s]\n");
        return;
    }

    if (c == 4)
    {
        if (!strcmp(Cmd_Argv(3), "u"))
            flags = CVAR_USERINFO;
        else if (!strcmp(Cmd_Argv(3), "s"))
            flags = CVAR_SERVERINFO;
        else
        {
            Com_Printf_G ("flags can only be 'u' or 's'\n");
            return;
        }
        Cvar_FullSet (Cmd_Argv(1), Cmd_Argv(2), flags);
    }
    else
        Cvar_Set (Cmd_Argv(1), Cmd_Argv(2));
}


/*
============
Cvar_WriteVariables

Appends lines containing "set variable value" for all variables
with the archive flag set to true.
============
*/
void Cvar_WriteVariables (const std::string& path)
{
    cvar    *var;
    char    buffer[1024];
    FILE    *f;

    f = fopen (path.c_str(), "a");
    for (var = cvar_vars ; var ; var = var->next)
    {
        if (var->flags & CVAR_ARCHIVE)
        {
            //Com_sprintf (buffer, sizeof(buffer), "set %s \"%s\"\n", var->name, var->string);
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
    cvar    *var;
    int        i;

    i = 0;
    for (var = cvar_vars ; var ; var = var->next, i++)
    {
        if (var->flags & CVAR_ARCHIVE)
            Com_Printf_G ("*");
        else
            Com_Printf_G (" ");
        if (var->flags & CVAR_USERINFO)
            Com_Printf_G ("U");
        else
            Com_Printf_G (" ");
        if (var->flags & CVAR_SERVERINFO)
            Com_Printf_G ("S");
        else
            Com_Printf_G (" ");
        if (var->flags & CVAR_NOSET)
            Com_Printf_G ("-");
        else if (var->flags & CVAR_LATCH)
            Com_Printf_G ("L");
        else
            Com_Printf_G (" ");
        Com_Printf_G (" %s \"%s\"\n", var->name, var->string);
    }
    Com_Printf_G ("%i cvars\n", i);
}


bool userinfo_modified;


char    *Cvar_BitInfo (int bit)
{
    static char    info[MAX_INFO_STRING];
    cvar    *var;

    info[0] = 0;

    for (var = cvar_vars ; var ; var = var->next)
    {
        if (var->flags & bit)
            Info_SetValueForKey (info, var->name, var->string);
    }
    return info;
}

// returns an info string containing all the CVAR_USERINFO cvars
char    *Cvar_Userinfo (void)
{
    return Cvar_BitInfo (CVAR_USERINFO);
}

// returns an info string containing all the CVAR_SERVERINFO cvars
char    *Cvar_Serverinfo (void)
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
