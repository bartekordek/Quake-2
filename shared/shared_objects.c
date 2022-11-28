#include "shared/shared_objects.h"

#include "game/g_main.h"
#include "game/game.h"
#include "server/sv_game.h"
#include "shared/edict.h"

#include "win32/sys_win.h"

volatile game_export_t ge;
game_import_t gi;

void initGame()
{
    if( ge.initialized != true )
    {
        ge.apiversion = GAME_API_VERSION;
        ge.Init = InitGame;
        ge.Shutdown = ShutdownGame;
        ge.SpawnEntities = SpawnEntities;

        ge.WriteGame = WriteGame;
        ge.ReadGame = ReadGame;
        ge.WriteLevel = WriteLevel;
        ge.ReadLevel = ReadLevel;

        ge.ClientThink = ClientThink;
        ge.ClientConnect = ClientConnect;
        ge.ClientUserinfoChanged = ClientUserinfoChanged;
        ge.ClientDisconnect = ClientDisconnect;
        ge.ClientBegin = ClientBegin;
        ge.ClientCommand = ClientCommand;

        ge.RunFrame = G_RunFrame;

        ge.ServerCommand = ServerCommand;

        int edictSize = sizeof( edict_t );
        ge.edict_size = edictSize;
        ge.edicts = NULL;

        ge.initialized = true;
    }

    if( gi.initialized != true )
    {
        if( ge.initialized != true )
            SV_ShutdownGameProgs();

        // load a new game dll
        gi.multicast = SV_Multicast;
        gi.unicast = PF_Unicast;
        gi.bprintf = SV_BroadcastPrintf;
        gi.dprintf = PF_dprintf;
        gi.cprintf = PF_cprintf;
        gi.centerprintf = PF_centerprintf;
        gi.error = PF_error;

        gi.linkentity = SV_LinkEdict;
        gi.unlinkentity = SV_UnlinkEdict;
        gi.BoxEdicts = SV_AreaEdicts;
        gi.trace = SV_Trace;
        gi.pointcontents = SV_PointContents;
        gi.setmodel = PF_setmodel;
        gi.inPVS = PF_inPVS;
        gi.inPHS = PF_inPHS;
        gi.Pmove = Pmove;

        gi.modelindex = SV_ModelIndex;
        gi.soundindex = SV_SoundIndex;
        gi.imageindex = SV_ImageIndex;

        gi.configstring = PF_Configstring;
        gi.sound = PF_StartSound;
        gi.positioned_sound = SV_StartSound;

        gi.WriteChar = PF_WriteChar;
        gi.WriteByte = PF_WriteByte;
        gi.WriteShort = PF_WriteShort;
        gi.WriteLong = PF_WriteLong;
        gi.WriteFloat = PF_WriteFloat;
        gi.WriteString = PF_WriteString;
        gi.WritePosition = PF_WritePos;
        gi.WriteDir = PF_WriteDir;
        gi.WriteAngle = PF_WriteAngle;

        gi.TagMalloc = Z_TagMalloc;
        gi.TagFree = Z_Free;
        gi.FreeTags = Z_FreeTags;

        gi.cvar = Cvar_Get;
        gi.cvar_set = Cvar_Set;
        gi.cvar_forceset = Cvar_ForceSet;

        gi.argc = Cmd_Argc;
        gi.argv = Cmd_Argv;
        gi.args = Cmd_Args;
        gi.AddCommandString = Cbuf_AddText;

        gi.DebugGraph = SCR_DebugGraph;
        gi.SetAreaPortalState = CM_SetAreaPortalState;
        gi.AreasConnected = CM_AreasConnected;

        //ge = (game_export_t *)sys_getgameapi (&import);
        //ge = *(game_export_t*) Sys_GetGameAPI( NULL );

        if( ge.initialized != true )
            Com_Error( ERR_DROP, "failed to load game DLL" );
        if( ge.apiversion != GAME_API_VERSION )
            Com_Error( ERR_DROP, "game is version %i, not %i", ge.apiversion,
                       GAME_API_VERSION );

        ge.Init();
    }
}