
#pragma once

#include "game/game.hpp"
#include "shared/clipplane.hpp"
#include "shared/defines.hpp"
#include "shared/entity.hpp"
#include "shared/viddef.hpp"
#include "client/ref.hpp"

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

    cvar* r_lightlevel = nullptr;// FIXME: This is a HACK to get the client's light level

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
    int r_visframecount = 0; // bumped when going to a new PVS
    int d_spanpixcount;
    int r_polycount;
    int r_drawnpolycount;
    int r_wholepolycount;

    int r_viewcluster = 0;
    int r_viewcluster2 = 0;
    int r_oldviewcluster = 0;
    int r_oldviewcluster2 = 0;

    bool r_dowarp = false;

    image_t gltextures[MAX_GLTEXTURES];
    int numgltextures;
    image_t* r_notexture = nullptr;        // use for bad textures
    image_t* r_particletexture = nullptr;  // little dot for particles
    model_t* currentmodel = nullptr;
    plane_s frustum[4];
    int c_brush_polys, c_alias_polys;
    int gl_filter_min, gl_filter_max;

    float v_blend[4];  // final blending color

    //
    // screen size info
    //
    refdef_t r_newrefdef;

    cvar* gl_vertex_arrays = nullptr;
    cvar* gl_ext_palettedtexture;
    cvar* gl_ext_multitexture = nullptr;
    cvar* gl_ext_pointparameters;
    cvar* gl_ext_compiled_vertex_array = nullptr;
    cvar* gl_particle_min_size = nullptr;
    cvar* gl_particle_max_size = nullptr;
    cvar* gl_particle_size = nullptr;
    cvar* gl_particle_att_a = nullptr;
    cvar* gl_particle_att_b = nullptr;
    cvar* gl_particle_att_c = nullptr;
    cvar* gl_nosubimage = nullptr;
    cvar* gl_allow_software = nullptr;
    cvar* gl_bitdepth = nullptr;
    cvar* gl_mode = nullptr;
    cvar* gl_log = nullptr;
    cvar* gl_lightmap = nullptr;
    cvar* gl_shadows = nullptr;
    cvar* gl_dynamic = nullptr;
    cvar* gl_monolightmap = nullptr;
    cvar* gl_nobind = nullptr;
    cvar* gl_round_down = nullptr;
    cvar* gl_picmip = nullptr;
    cvar* gl_skymip = nullptr;
    cvar* gl_showtris = nullptr;
    cvar* gl_finish = nullptr;
    cvar* gl_ztrick = nullptr;
    cvar* gl_clear = nullptr;
    cvar* gl_cull = nullptr;
    cvar* gl_poly = nullptr;
    cvar* gl_texsort = nullptr;
    cvar* gl_polyblend = nullptr;
    cvar* gl_flashblend = nullptr;
    cvar* gl_lightmaptype = nullptr;
    cvar* gl_modulate = nullptr;
    cvar* gl_playermip = nullptr;
    cvar* gl_drawbuffer = nullptr;
    cvar* gl_3dlabs_broken = nullptr;
    cvar* gl_driver = nullptr;
    cvar* gl_swapinterval = nullptr;
    cvar* gl_texturemode = nullptr;
    cvar* gl_texturealphamode = nullptr;
    cvar* gl_texturesolidmode = nullptr;
    cvar* gl_saturatelighting = nullptr;
    cvar* gl_lockpvs = nullptr;
    cvar* gl_3dlabs_broken = nullptr;
    cvar* vid_fullscreen = nullptr;
    cvar* vid_gamma = nullptr;
    cvar* vid_ref = nullptr;
    cvar* intensity = nullptr;
    int gl_lightmap_format = 0;
    int gl_solid_format = 0;
    int gl_alpha_format = 0;
    int gl_tex_solid_format = 0;
    int gl_tex_alpha_format = 0;
    int c_visible_lightmaps = 0;
    int c_visible_textures = 0;
    float r_world_matrix[16];
    float r_base_world_matrix[16];

    model_t* r_worldmodel = nullptr;

    float gldepthmin = 0.f;
    float gldepthmax = 0.f;

    glconfig_t gl_config;
    glstate_t gl_state;

    ~quake2();

protected:
private:
    quake2();

    static quake2* s_instance;
};