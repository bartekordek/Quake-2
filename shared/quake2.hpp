
#pragma once

#include "game/game.hpp"
#include "shared/clipplane.hpp"
#include "shared/defines.hpp"
#include "shared/entity.hpp"
#include "shared/viddef.hpp"

class quake2 final
{
public:
    static quake2* getInstance();

    cvar* allow_download_players = nullptr;
    cvar* allow_download_models = nullptr;
    cvar* allow_download_sounds = nullptr;
    cvar* allow_download_maps = nullptr;
    cvar* allow_download = nullptr;
    cvar* dedicated = nullptr;
    game_import gi;
    game_export_t ge;
    entity_s* currententity = nullptr;
    image_t* draw_chars = nullptr;

    int r_dlightframecount;
    vec3_t pointcolor;
    plane_s* lightplane;  // used as shadow plane

    vec3_t lightspot;

    viddef_t vid;     // video driver defs
    viddef_t viddef;  // global video state

    // sequence # of current frame since Quake
    //  started
    // so frame counts initialized to 0 don't match
    int r_framecount = 0;  // used for dlight push checking

    //
    // view origin
    //
    vec3_t vup;
    vec3_t vpn;
    vec3_t vright;
    vec3_t r_origin;

    cvar* r_fullbright;

    cvar* r_norefresh = nullptr;
    cvar* r_lefthand = nullptr;
    cvar* r_drawentities = nullptr;
    cvar* r_drawworld = nullptr;
    cvar* r_speeds = nullptr;
    cvar* r_novis = nullptr;
    cvar* r_nocull = nullptr;
    cvar* r_lerpmodels = nullptr;

    cvar* sw_aliasstats = nullptr;
    cvar* sw_allow_modex = nullptr;
    cvar* sw_clearcolor = nullptr;
    cvar* sw_drawflat = nullptr;
    cvar* sw_draworder = nullptr;
    cvar* sw_maxedges = nullptr;
    cvar* sw_maxsurfs = nullptr;
    cvar* sw_mode = nullptr;
    cvar* sw_reportedgeout = nullptr;
    cvar* sw_reportsurfout = nullptr;
    cvar* sw_stipplealpha = nullptr;
    cvar* sw_surfcacheoverride = nullptr;
    cvar* sw_waterwarp = nullptr;

    cvar* r_dspeeds = nullptr;

    cvar* r_lightlevel = nullptr;  // FIXME HACK

    cvar* vid_fullscreen = nullptr;
    cvar* vid_gamma = nullptr;

    // PGM
    cvar* sw_lockpvs = nullptr;
    // PGM

    cvar* sw_mipcap = nullptr;
    cvar* sw_mipscale = nullptr;
    clipplane_s view_clipplanes[4];
    int* pfrustum_indexes[4];
    int r_frustum_indexes[4 * 6];

    //
    // refresh flags
    //
    int r_visframecount;
    int d_spanpixcount;
    int r_polycount;
    int r_drawnpolycount;
    int r_wholepolycount;

    int r_viewcluster = 0;
    int r_viewcluster2 = 0;
    int r_oldviewcluster = 0;
    int r_oldviewcluster2 = 0;

    bool r_dowarp = false;

    ~quake2();

protected:
private:
    quake2();

    static quake2* s_instance;
};