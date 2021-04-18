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
// r_main.c

#include "ref_soft/r_local.hpp"
#include "shared/shared_functions.hpp"
#include "win32/q_shwin.hpp"

refimport_t ri;

unsigned d_8to24table[256];

entity_s r_worldentity;

char skyname[MAX_QPATH];
float skyrotate;
vec3_t skyaxis;
image_t* sky_images[6];

refdef_t r_newrefdef;
model_t* currentmodel;

model_t* r_worldmodel;

byte r_warpbuffer[WARP_WIDTH * WARP_HEIGHT];

swstate_t sw_state;

void* colormap;
vec3_t viewlightvec;
alight_t r_viewlighting = { 128, 192, viewlightvec };
float r_time1;
int r_numallocatededges;
float r_aliasuvscale = 1.0;
int r_outofsurfaces;
int r_outofedges;

mvertex_t* r_pcurrentvertbase;

int c_surf;
int r_maxsurfsseen, r_maxedgesseen, r_cnumsurfs;
bool r_surfsonstack;
int r_clipflags;

//
// view origin
//
vec3_t vup, base_vup;
vec3_t vpn, base_vpn;
vec3_t vright, base_vright;
vec3_t r_origin;

//
// screen size info
//
oldrefdef_t r_refdef;
float xcenter, ycenter;
float xscale, yscale;
float xscaleinv, yscaleinv;
float xscaleshrink, yscaleshrink;
float aliasxscale, aliasyscale, aliasxcenter, aliasycenter;

int r_screenwidth;

float verticalFieldOfView;
float xOrigin, yOrigin;

plane_s screenedge[4];

mleaf_t* r_viewleaf;
int r_viewcluster, r_oldviewcluster;

image_t* r_notexture_mip;

float da_time1, da_time2, dp_time1, dp_time2, db_time1, db_time2, rw_time1,
    rw_time2;
float se_time1, se_time2, de_time1, de_time2;

void ref_soft_R_MarkLeaves( void );

#define STRINGER( x ) "x"

#if !id386

// r_vars.c

// all global and static refresh variables are collected in a contiguous block
// to avoid cache conflicts.

//-------------------------------------------------------
// global refresh variables
//-------------------------------------------------------

// FIXME: make into one big structure, like cl or sv
// FIXME: do separately for refresh engine and driver

// d_vars.c

// all global and static refresh variables are collected in a contiguous block
// to avoid cache conflicts.

//-------------------------------------------------------
// global refresh variables
//-------------------------------------------------------

// FIXME: make into one big structure, like cl or sv
// FIXME: do separately for refresh engine and driver

float d_sdivzstepu, d_tdivzstepu, d_zistepu;
float d_sdivzstepv, d_tdivzstepv, d_zistepv;
float d_sdivzorigin, d_tdivzorigin, d_ziorigin;

fixed16_t sadjust, tadjust, bbextents, bbextentt;

pixel_t* cacheblock;
int cachewidth;
pixel_t* d_viewbuffer;
short* d_pzbuffer;
unsigned int d_zrowbytes;
unsigned int d_zwidth;

#endif  // !id386

byte r_notexture_buffer[1024];

/*
==================
R_InitTextures
==================
*/
void R_InitTextures( void )
{
    int x, y, m;
    byte* dest;

    // create a simple checkerboard texture for the default
    r_notexture_mip = (image_t*)&r_notexture_buffer;

    r_notexture_mip->width = r_notexture_mip->height = 16;
    r_notexture_mip->pixels[0] = &r_notexture_buffer[sizeof( image_t )];
    r_notexture_mip->pixels[1] = r_notexture_mip->pixels[0] + 16 * 16;
    r_notexture_mip->pixels[2] = r_notexture_mip->pixels[1] + 8 * 8;
    r_notexture_mip->pixels[3] = r_notexture_mip->pixels[2] + 4 * 4;

    for ( m = 0; m < 4; m++ )
    {
        dest = r_notexture_mip->pixels[m];
        for ( y = 0; y < ( 16 >> m ); y++ )
            for ( x = 0; x < ( 16 >> m ); x++ )
            {
                if ( ( y < ( 8 >> m ) ) ^ ( x < ( 8 >> m ) ) )

                    *dest++ = 0;
                else
                    *dest++ = 0xff;
            }
    }
}

void ref_soft_R_InitTurb( void )
{
    int i;

    for ( i = 0; i < 1280; i++ )
    {
        sintable[i] = AMP + sin( i * 3.14159 * 2 / CYCLE ) * AMP;
        intsintable[i] =
            AMP2 + sin( i * 3.14159 * 2 / CYCLE ) * AMP2;  // AMP2, not 20
        blanktable[i] = 0;                                 // PGM
    }
}

void ref_soft_R_ImageList_f( void );

void ref_soft_R_Register( void )
{
    quake2::getInstance()->sw_aliasstats =
        ri.Cvar_Get( "sw_polymodelstats", "0", 0 );
    quake2::getInstance()->sw_allow_modex =
        ri.Cvar_Get( "sw_allow_modex", "1", CVAR_ARCHIVE );
    quake2::getInstance()->sw_clearcolor =
        ri.Cvar_Get( "sw_clearcolor", "2", 0 );
    quake2::getInstance()->sw_drawflat = ri.Cvar_Get( "sw_drawflat", "0", 0 );
    quake2::getInstance()->sw_draworder = ri.Cvar_Get( "sw_draworder", "0", 0 );
    quake2::getInstance()->sw_maxedges =
        ri.Cvar_Get( "sw_maxedges", STRINGER( MAXSTACKSURFACES ), 0 );
    quake2::getInstance()->sw_maxsurfs = ri.Cvar_Get( "sw_maxsurfs", "0", 0 );
    quake2::getInstance()->sw_mipcap = ri.Cvar_Get( "sw_mipcap", "0", 0 );
    quake2::getInstance()->sw_mipscale = ri.Cvar_Get( "sw_mipscale", "1", 0 );
    quake2::getInstance()->sw_reportedgeout =
        ri.Cvar_Get( "sw_reportedgeout", "0", 0 );
    quake2::getInstance()->sw_reportsurfout =
        ri.Cvar_Get( "sw_reportsurfout", "0", 0 );
    quake2::getInstance()->sw_stipplealpha =
        ri.Cvar_Get( "sw_stipplealpha", "0", CVAR_ARCHIVE );
    quake2::getInstance()->sw_surfcacheoverride =
        ri.Cvar_Get( "sw_surfcacheoverride", "0", 0 );
    quake2::getInstance()->sw_waterwarp = ri.Cvar_Get( "sw_waterwarp", "1", 0 );
    quake2::getInstance()->sw_mode =
        ri.Cvar_Get( "sw_mode", "0", CVAR_ARCHIVE );

    quake2::getInstance()->r_lefthand =
        ri.Cvar_Get( "hand", "0", CVAR_USERINFO | CVAR_ARCHIVE );
    quake2::getInstance()->r_speeds = ri.Cvar_Get( "r_speeds", "0", 0 );
    quake2::getInstance()->r_fullbright = ri.Cvar_Get( "r_fullbright", "0", 0 );
    quake2::getInstance()->r_drawentities =
        ri.Cvar_Get( "r_drawentities", "1", 0 );
    quake2::getInstance()->r_drawworld = ri.Cvar_Get( "r_drawworld", "1", 0 );
    quake2::getInstance()->r_dspeeds = ri.Cvar_Get( "r_dspeeds", "0", 0 );
    quake2::getInstance()->r_lightlevel = ri.Cvar_Get( "r_lightlevel", "0", 0 );
    quake2::getInstance()->r_lerpmodels = ri.Cvar_Get( "r_lerpmodels", "1", 0 );
    quake2::getInstance()->r_novis = ri.Cvar_Get( "r_novis", "0", 0 );

    vid_fullscreen = ri.Cvar_Get( "vid_fullscreen", "0", CVAR_ARCHIVE );
    vid_gamma = ri.Cvar_Get( "vid_gamma", "1.0", CVAR_ARCHIVE );

    ri.Cmd_AddCommand( "modellist", ref_soft_Mod_Modellist_f );
    ri.Cmd_AddCommand( "screenshot", ref_soft_R_ScreenShot_f );
    ri.Cmd_AddCommand( "imagelist", ref_soft_R_ImageList_f );

    sw_mode->modified = true;    // force us to do mode specific stuff later
    vid_gamma->modified = true;  // force us to rebuild the gamma table later

    // PGM
    quake2::getInstance()->sw_lockpvs = ri.Cvar_Get( "sw_lockpvs", "0", 0 );
    // PGM
}

void ref_soft_R_UnRegister( void )
{
    ri.Cmd_RemoveCommand( "screenshot" );
    ri.Cmd_RemoveCommand( "modellist" );
    ri.Cmd_RemoveCommand( "imagelist" );
}

int ref_soft_R_Init( void* hInstance, void* wndProc )
{
    auto& view_clipplanes = quake2::getInstance()->view_clipplanes;

    ref_soft_R_InitImages();
    ref_soft_Mod_Init();
    ref_soft_Draw_InitLocal();
    R_InitTextures();

    ref_soft_R_InitTurb();

    view_clipplanes[0].leftedge = true;
    view_clipplanes[1].rightedge = true;
    view_clipplanes[1].leftedge = view_clipplanes[2].leftedge =
        view_clipplanes[3].leftedge = false;
    view_clipplanes[0].rightedge = view_clipplanes[2].rightedge =
        view_clipplanes[3].rightedge = false;

    r_refdef.xOrigin = XCENTERING;
    r_refdef.yOrigin = YCENTERING;

// TODO: collect 386-specific code in one place
#if id386
    Sys_MakeCodeWriteable( (long)R_EdgeCodeStart,
                           (long)R_EdgeCodeEnd - (long)R_EdgeCodeStart );
    Sys_SetFPCW();  // get bit masks for FPCW    (FIXME: is this id386?)
#endif              // id386

    r_aliasuvscale = 1.0;

    ref_soft_R_Register();
    ref_soft_Draw_GetPalette();
    SWimp_Init( hInstance, wndProc );

    // create the window
    ref_soft_R_BeginFrame( 0 );

    ri.Con_Printf( PRINT_ALL, "ref_soft version: " REF_VERSION "\n" );

    return true;
}

void ref_soft_R_Shutdown( void )
{
    // free z buffer
    if ( d_pzbuffer )
    {
        free( d_pzbuffer );
        d_pzbuffer = NULL;
    }
    // free surface cache
    if ( sc_base )
    {
        ref_soft_D_FlushCaches();
        free( sc_base );
        sc_base = NULL;
    }

    // free colormap
    if ( quake2::getInstance()->vid.colormap )
    {
        free( quake2::getInstance()->vid.colormap );
        quake2::getInstance()->vid.colormap = NULL;
    }
    ref_soft_R_UnRegister();
    ref_soft_Mod_FreeAll();
    ref_soft_R_ShutdownImages();

    SWimp_Shutdown();
}

void ref_soft_R_NewMap( void )
{
    r_viewcluster = -1;

    r_cnumsurfs = quake2::getInstance()->sw_maxsurfs->value;

    if ( r_cnumsurfs <= MINSURFACES )
        r_cnumsurfs = MINSURFACES;

    if ( r_cnumsurfs > NUMSTACKSURFACES )
    {
        surfaces = (surf_t*)malloc( r_cnumsurfs * sizeof( surf_t ) );
        surface_p = surfaces;
        surf_max = &surfaces[r_cnumsurfs];
        r_surfsonstack = false;
        // surface 0 doesn't really exist; it's just a dummy because index 0
        // is used to indicate no edge attached to surface
        surfaces--;
        ref_soft_R_SurfacePatch();
    }
    else
    {
        r_surfsonstack = true;
    }

    r_maxedgesseen = 0;
    r_maxsurfsseen = 0;

    r_numallocatededges = quake2::getInstance()->sw_maxedges->value;

    if ( r_numallocatededges < MINEDGES )
        r_numallocatededges = MINEDGES;

    if ( r_numallocatededges <= NUMSTACKEDGES )
    {
        auxedges = NULL;
    }
    else
    {
        auxedges = (edge_t*)malloc( r_numallocatededges * sizeof( edge_t ) );
    }
}

/*
Mark the leaves and nodes that are in the PVS for the current
cluster
*/
void ref_soft_R_MarkLeaves( void )
{
    byte* vis;
    mnode_s* node;
    int i;
    mleaf_t* leaf;
    int cluster;

    if ( r_oldviewcluster == r_viewcluster && !quake2::getInstance()->r_novis->value &&
         quake2::getInstance()->r_viewcluster != -1 )
        return;

    // development aid to let you run around and see exactly where
    // the pvs ends
    if ( quake2::getInstance()->sw_lockpvs->value )
        return;

    r_visframecount++;
    r_oldviewcluster = r_viewcluster;

    if ( quake2::getInstance()->r_novis->value || r_viewcluster == -1 || !r_worldmodel->vis )
    {
        // mark everything
        for ( i = 0; i < r_worldmodel->numleafs; i++ )
            r_worldmodel->leafs[i].visframe = r_visframecount;
        for ( i = 0; i < r_worldmodel->numnodes; i++ )
            r_worldmodel->nodes[i].visframe = r_visframecount;
        return;
    }

    vis = ref_soft_Mod_ClusterPVS( r_viewcluster, r_worldmodel );

    for ( i = 0, leaf = r_worldmodel->leafs; i < r_worldmodel->numleafs;
          i++, leaf++ )
    {
        cluster = leaf->cluster;
        if ( cluster == -1 )
            continue;
        if ( vis[cluster >> 3] & ( 1 << ( cluster & 7 ) ) )
        {
            node = (mnode_s*)leaf;
            do
            {
                if ( node->visframe == r_visframecount )
                    break;
                node->visframe = r_visframecount;
                node = node->parent;
            } while ( node );
        }
    }

#if 0
    for (i=0 ; i<r_worldmodel->vis->numclusters ; i++)
    {
        if (vis[i>>3] & (1<<(i&7)))
        {
            node = (mnode_s *)&r_worldmodel->leafs[i];    // FIXME: cluster
            do
            {
                if (node->visframe == r_visframecount)
                    break;
                node->visframe = r_visframecount;
                node = node->parent;
            } while (node);
        }
    }
#endif
}

/*
** ref_soft_R_DrawNullModel
**
** IMPLEMENT THIS!
*/
void ref_soft_R_DrawNullModel( void ) {}

void ref_soft_R_DrawEntitiesOnList( void )
{
    int i;
    bool translucent_entities = false;

    if ( !quake2::getInstance()->r_drawentities->value )
        return;

    // all bmodels have already been drawn by the edge list
    for ( i = 0; i < r_newrefdef.num_entities; i++ )
    {
        quake2::getInstance()->currententity = &r_newrefdef.entities[i];

        if ( quake2::getInstance()->currententity->flags & RF_TRANSLUCENT )
        {
            translucent_entities = true;
            continue;
        }

        if ( quake2::getInstance()->currententity->flags & RF_BEAM )
        {
            modelorg[0] = -quake2::getInstance()->r_origin[0];
            modelorg[1] = -quake2::getInstance()->r_origin[1];
            modelorg[2] = -quake2::getInstance()->r_origin[2];
            VectorCopy( vec3_origin, r_entorigin );
            ref_soft_R_DrawBeam( quake2::getInstance()->currententity );
        }
        else
        {
            currentmodel = quake2::getInstance()->currententity->model;
            if ( !currentmodel )
            {
                ref_soft_R_DrawNullModel();
                continue;
            }
            VectorCopy( quake2::getInstance()->currententity->origin,
                        r_entorigin );
            VectorSubtract( quake2::getInstance()->r_origin, r_entorigin,
                            modelorg );

            switch ( currentmodel->type )
            {
                case mod_sprite:
                    ref_soft_R_DrawSprite();
                    break;

                case mod_alias:
                    ref_soft_R_AliasDrawModel();
                    break;

                default:
                    break;
            }
        }
    }

    if ( !translucent_entities )
        return;

    for ( i = 0; i < r_newrefdef.num_entities; i++ )
    {
        quake2::getInstance()->currententity = &r_newrefdef.entities[i];

        if ( !( quake2::getInstance()->currententity->flags & RF_TRANSLUCENT ) )
            continue;

        if ( quake2::getInstance()->currententity->flags & RF_BEAM )
        {
            modelorg[0] = -quake2::getInstance()->r_origin[0];
            modelorg[1] = -quake2::getInstance()->r_origin[1];
            modelorg[2] = -quake2::getInstance()->r_origin[2];
            VectorCopy( vec3_origin, r_entorigin );
            ref_soft_R_DrawBeam( quake2::getInstance()->currententity );
        }
        else
        {
            currentmodel = quake2::getInstance()->currententity->model;
            if ( !currentmodel )
            {
                ref_soft_R_DrawNullModel();
                continue;
            }
            VectorCopy( quake2::getInstance()->currententity->origin,
                        r_entorigin );
            VectorSubtract( quake2::getInstance()->r_origin, r_entorigin,
                            modelorg );

            switch ( currentmodel->type )
            {
                case mod_sprite:
                    ref_soft_R_DrawSprite();
                    break;

                case mod_alias:
                    ref_soft_R_AliasDrawModel();
                    break;

                default:
                    break;
            }
        }
    }
}

/*
=============
R_BmodelCheckBBox
=============
*/
int ref_soft_R_BmodelCheckBBox( float* minmaxs )
{
    int i, *pindex, clipflags;
    vec3_t acceptpt, rejectpt;
    float d;

    clipflags = 0;

    for ( i = 0; i < 4; i++ )
    {
        // generate accept and reject points
        // FIXME: do with fast look-ups or integer tests based on the sign bit
        // of the floating point values

        pindex = quake2::getInstance()->pfrustum_indexes[i];

        rejectpt[0] = minmaxs[pindex[0]];
        rejectpt[1] = minmaxs[pindex[1]];
        rejectpt[2] = minmaxs[pindex[2]];

        d = DotProduct( rejectpt, quake2::getInstance()->view_clipplanes[i].normal );
        d -= quake2::getInstance()->view_clipplanes[i].dist;

        if ( d <= 0 )
            return BMODEL_FULLY_CLIPPED;

        acceptpt[0] = minmaxs[pindex[3 + 0]];
        acceptpt[1] = minmaxs[pindex[3 + 1]];
        acceptpt[2] = minmaxs[pindex[3 + 2]];

        d = DotProduct( acceptpt, quake2::getInstance()->view_clipplanes[i].normal );
        d -= quake2::getInstance()->view_clipplanes[i].dist;

        if ( d <= 0 )
            clipflags |= ( 1 << i );
    }

    return clipflags;
}

/*
Find the first node that splits the given box
*/
mnode_s* ref_soft_R_FindTopnode( vec3_t mins, vec3_t maxs )
{
    plane_s* splitplane;
    int sides;
    mnode_s* node;

    node = r_worldmodel->nodes;

    while ( 1 )
    {
        if ( node->visframe != r_visframecount )
            return NULL;  // not visible at all

        if ( node->contents != CONTENTS_NODE )
        {
            if ( node->contents != CONTENTS_SOLID )
                return node;  // we've reached a non-solid leaf, so it's
                              //  visible and not BSP clipped
            return NULL;      // in solid, so not visible
        }

        splitplane = node->plane;
        sides = BOX_ON_PLANE_SIDE( mins, maxs, (plane_s*)splitplane );

        if ( sides == 3 )
            return node;  // this is the splitter

        // not split yet; recurse down the contacted side
        if ( sides & 1 )
            node = node->children[0];
        else
            node = node->children[1];
    }
}

/*
=============
RotatedBBox

Returns an axially aligned box that contains the input box at the given rotation
=============
*/
void RotatedBBox( vec3_t mins, vec3_t maxs, vec3_t angles, vec3_t tmins,
                  vec3_t tmaxs )
{
    vec3_t tmp, v;
    int i, j;
    vec3_t forward, right, up;

    if ( !angles[0] && !angles[1] && !angles[2] )
    {
        VectorCopy( mins, tmins );
        VectorCopy( maxs, tmaxs );
        return;
    }

    for ( i = 0; i < 3; i++ )
    {
        tmins[i] = 99999;
        tmaxs[i] = -99999;
    }

    AngleVectors( angles, forward, right, up );

    for ( i = 0; i < 8; i++ )
    {
        if ( i & 1 )
            tmp[0] = mins[0];
        else
            tmp[0] = maxs[0];

        if ( i & 2 )
            tmp[1] = mins[1];
        else
            tmp[1] = maxs[1];

        if ( i & 4 )
            tmp[2] = mins[2];
        else
            tmp[2] = maxs[2];

        VectorScale( forward, tmp[0], v );
        VectorMA( v, -tmp[1], right, v );
        VectorMA( v, tmp[2], up, v );

        for ( j = 0; j < 3; j++ )
        {
            if ( v[j] < tmins[j] )
                tmins[j] = v[j];
            if ( v[j] > tmaxs[j] )
                tmaxs[j] = v[j];
        }
    }
}

void ref_soft_R_DrawBEntitiesOnList( void )
{
    int i, clipflags;
    vec3_t oldorigin;
    vec3_t mins, maxs;
    float minmaxs[6];
    mnode_s* topnode;

    if ( !quake2::getInstance()->r_drawentities->value )
        return;

    VectorCopy( modelorg, oldorigin );
    insubmodel = true;
    quake2::getInstance()->r_dlightframecount =
        quake2::getInstance()->r_framecount;

    for ( i = 0; i < r_newrefdef.num_entities; i++ )
    {
        quake2::getInstance()->currententity = &r_newrefdef.entities[i];
        currentmodel = quake2::getInstance()->currententity->model;
        if ( !currentmodel )
            continue;
        if ( currentmodel->nummodelsurfaces == 0 )
            continue;  // clip brush only
        if ( quake2::getInstance()->currententity->flags & RF_BEAM )
            continue;
        if ( currentmodel->type != mod_brush )
            continue;
        // see if the bounding box lets us trivially reject, also sets
        // trivial accept status
        RotatedBBox( currentmodel->mins, currentmodel->maxs,
                     quake2::getInstance()->currententity->angles, mins, maxs );
        VectorAdd( mins, quake2::getInstance()->currententity->origin,
                   minmaxs );
        VectorAdd( maxs, quake2::getInstance()->currententity->origin,
                   ( minmaxs + 3 ) );

        clipflags = ref_soft_R_BmodelCheckBBox( minmaxs );
        if ( clipflags == BMODEL_FULLY_CLIPPED )
            continue;  // off the edge of the screen

        topnode = ref_soft_R_FindTopnode( minmaxs, minmaxs + 3 );
        if ( !topnode )
            continue;  // no part in a visible leaf

        VectorCopy( quake2::getInstance()->currententity->origin, r_entorigin );
        VectorSubtract( quake2::getInstance()->r_origin, r_entorigin,
                        modelorg );

        r_pcurrentvertbase = currentmodel->vertexes;

        // FIXME: stop transforming twice
        ref_soft_R_RotateBmodel();

        // calculate dynamic lighting for bmodel
        ref_soft_R_PushDlights( currentmodel );

        if ( topnode->contents == CONTENTS_NODE )
        {
            // not a leaf; has to be clipped to the world BSP
            r_clipflags = clipflags;
            ref_soft_R_DrawSolidClippedSubmodelPolygons( currentmodel,
                                                         topnode );
        }
        else
        {
            // falls entirely in one leaf, so we just put all the
            // edges in the edge list and let 1/z sorting handle
            // drawing order
            ref_soft_R_DrawSubmodelPolygons( currentmodel, clipflags, topnode );
        }

        // put back world rotation and frustum clipping
        // FIXME: ref_soft_R_RotateBmodel should just work off base_vxx
        VectorCopy( base_vpn, quake2::getInstance()->vpn );
        VectorCopy( base_vup, quake2::getInstance()->vup );
        VectorCopy( base_vright, quake2::getInstance()->vright );
        VectorCopy( oldorigin, modelorg );
        ref_soft_R_TransformFrustum();
    }

    insubmodel = false;
}

void ref_soft_R_EdgeDrawing( void )
{
    auto& r_dspeeds = quake2::getInstance()->r_dspeeds;
    edge_t
        ledges[NUMSTACKEDGES + ( ( CACHE_SIZE - 1 ) / sizeof( edge_t ) ) + 1];
    surf_t lsurfs[NUMSTACKSURFACES + ( ( CACHE_SIZE - 1 ) / sizeof( surf_t ) ) +
                  1];

    if ( r_newrefdef.rdflags & RDF_NOWORLDMODEL )
        return;

    if ( auxedges )
    {
        r_edges = auxedges;
    }
    else
    {
        r_edges = (edge_t*)( ( (long)&ledges[0] + CACHE_SIZE - 1 ) &
                             ~( CACHE_SIZE - 1 ) );
    }

    if ( r_surfsonstack )
    {
        surfaces = (surf_t*)( ( (long)&lsurfs[0] + CACHE_SIZE - 1 ) &
                              ~( CACHE_SIZE - 1 ) );
        surf_max = &surfaces[r_cnumsurfs];
        // surface 0 doesn't really exist; it's just a dummy because index 0
        // is used to indicate no edge attached to surface
        surfaces--;
        ref_soft_R_SurfacePatch();
    }

    ref_soft_R_BeginEdgeFrame();

    if ( r_dspeeds->value )
    {
        rw_time1 = win32_Sys_Milliseconds();
    }

    ref_soft_R_RenderWorld();

    if ( r_dspeeds->value )
    {
        rw_time2 = win32_Sys_Milliseconds();
        db_time1 = rw_time2;
    }

    ref_soft_R_DrawBEntitiesOnList();

    if ( r_dspeeds->value )
    {
        db_time2 = win32_Sys_Milliseconds();
        se_time1 = db_time2;
    }

    ref_soft_R_ScanEdges();
}

void ref_soft_R_CalcPalette( void )
{
    static bool modified;
    byte palette[256][4], *in, *out;
    int i, j;
    float alpha, one_minus_alpha;
    vec3_t premult;
    int v;

    alpha = r_newrefdef.blend[3];
    if ( alpha <= 0 )
    {
        if ( modified )
        {  // set back to default
            modified = false;
            ref_soft_R_GammaCorrectAndSetPalette(
                (const unsigned char*)d_8to24table );
            return;
        }
        return;
    }

    modified = true;
    if ( alpha > 1 )
        alpha = 1;

    premult[0] = r_newrefdef.blend[0] * alpha * 255;
    premult[1] = r_newrefdef.blend[1] * alpha * 255;
    premult[2] = r_newrefdef.blend[2] * alpha * 255;

    one_minus_alpha = ( 1.0 - alpha );

    in = (byte*)d_8to24table;
    out = palette[0];
    for ( i = 0; i < 256; i++, in += 4, out += 4 )
    {
        for ( j = 0; j < 3; j++ )
        {
            v = premult[j] + one_minus_alpha * in[j];
            if ( v > 255 )
                v = 255;
            out[j] = v;
        }
        out[3] = 255;
    }

    ref_soft_R_GammaCorrectAndSetPalette( (const unsigned char*)palette[0] );
    //    SWimp_SetPalette( palette[0] );
}

//=======================================================================

void ref_soft_R_SetLightLevel( void )
{
    vec3_t light;

    if ( ( r_newrefdef.rdflags & RDF_NOWORLDMODEL ) ||
         ( !quake2::getInstance()->r_drawentities->value ) ||
         ( !quake2::getInstance()->currententity ) )
    {
        r_lightlevel->value = 150.0;
        return;
    }

    // save off light value for server to look at (BIG HACK!)
    ref_soft_R_LightPoint( r_newrefdef.vieworg, light );
    r_lightlevel->value = 150.0 * light[0];
}

void ref_soft_R_RenderFrame( refdef_t* fd )
{
    r_newrefdef = *fd;

    if ( !r_worldmodel && !( r_newrefdef.rdflags & RDF_NOWORLDMODEL ) )
        ri.Sys_Error( ERR_FATAL, "ref_gl_R_RenderView: NULL worldmodel" );

    VectorCopy( fd->vieworg, r_refdef.vieworg );
    VectorCopy( fd->viewangles, r_refdef.viewangles );

    if ( quake2::getInstance()->r_speeds->value || quake2::getInstance()->r_dspeeds->value )
        r_time1 = win32_Sys_Milliseconds();

    ref_soft_R_SetupFrame();

    ref_soft_R_MarkLeaves();  // done here so we know if we're in water

    ref_soft_R_PushDlights( r_worldmodel );

    ref_soft_R_EdgeDrawing();

    if ( quake2::getInstance()->r_dspeeds->value )
    {
        se_time2 = win32_Sys_Milliseconds();
        de_time1 = se_time2;
    }

    ref_soft_R_DrawEntitiesOnList();

    if ( quake2::getInstance()->r_dspeeds->value )
    {
        de_time2 = win32_Sys_Milliseconds();
        dp_time1 = win32_Sys_Milliseconds();
    }

    ref_soft_R_DrawParticles();

    if ( quake2::getInstance()->r_dspeeds->value )
        dp_time2 = win32_Sys_Milliseconds();

    ref_soft_R_DrawAlphaSurfaces();

    ref_soft_R_SetLightLevel();

    if ( quake2::getInstance()->r_dowarp )
        ref_soft_D_WarpScreen();

    if ( quake2::getInstance()->r_dspeeds->value )
        da_time1 = win32_Sys_Milliseconds();

    if ( quake2::getInstance()->r_dspeeds->value )
        da_time2 = win32_Sys_Milliseconds();

    ref_soft_R_CalcPalette();

    if ( quake2::getInstance()->sw_aliasstats->value )
        ref_soft_R_PrintAliasStats();

    if ( quake2::getInstance()->r_speeds->value )
        ref_soft_R_PrintTimes();

    if ( quake2::getInstance()->r_dspeeds->value )
        ref_soft_R_PrintDSpeeds();

    if ( quake2::getInstance()->sw_reportsurfout->value && r_outofsurfaces )
        ri.Con_Printf( PRINT_ALL, "Short %d surfaces\n", r_outofsurfaces );

    if ( quake2::getInstance()->sw_reportedgeout->value && r_outofedges )
        ri.Con_Printf( PRINT_ALL, "Short roughly %d edges\n",
                       r_outofedges * 2 / 3 );
}

/*
** ref_soft_R_InitGraphics
*/
void ref_soft_R_InitGraphics( int width, int height )
{
    quake2::getInstance()->vid.width = width;
    quake2::getInstance()->vid.height = height;

    // free z buffer
    if ( d_pzbuffer )
    {
        free( d_pzbuffer );
        d_pzbuffer = NULL;
    }

    // free surface cache
    if ( sc_base )
    {
        ref_soft_D_FlushCaches();
        free( sc_base );
        sc_base = NULL;
    }

    d_pzbuffer = (short*)malloc( quake2::getInstance()->vid.width *
                                 quake2::getInstance()->vid.height * 2 );

    ref_soft_R_InitCaches();

    ref_soft_R_GammaCorrectAndSetPalette( (const unsigned char*)d_8to24table );
}

/*
** ref_soft_R_BeginFrame
*/
void ref_soft_R_BeginFrame( float camera_separation )
{
    extern void Draw_BuildGammaTable( void );

    /*
     * * rebuild the gamma correction palette if necessary
     */
    if ( vid_gamma->modified )
    {
        Draw_BuildGammaTable();
        ref_soft_R_GammaCorrectAndSetPalette(
            (const unsigned char*)d_8to24table );

        vid_gamma->modified = false;
    }

    while ( sw_mode->modified || vid_fullscreen->modified )
    {
        rserr_t err;

        /*
         * * if this returns rserr_invalid_fullscreen then it set the mode but
         * not as a
         * * fullscreen mode, e.g. 320x200 on a system that doesn't support that
         * res
         */
        if ( ( err = SWimp_SetMode( &quake2::getInstance()->vid.width,
                                    &quake2::getInstance()->vid.height,
                                    sw_mode->value, vid_fullscreen->value ) ) ==
             rserr_ok )
        {
            ref_soft_R_InitGraphics( quake2::getInstance()->vid.width,
                                     quake2::getInstance()->vid.height );

            sw_state.prev_mode = sw_mode->value;
            vid_fullscreen->modified = false;
            sw_mode->modified = false;
        }
        else
        {
            if ( err == rserr_invalid_mode )
            {
                ri.Cvar_SetValue( "sw_mode", sw_state.prev_mode );
                ri.Con_Printf(
                    PRINT_ALL,
                    "ref_soft::R_BeginFrame() - could not set mode\n" );
            }
            else if ( err == rserr_invalid_fullscreen )
            {
                ref_soft_R_InitGraphics( quake2::getInstance()->vid.width,
                                         quake2::getInstance()->vid.height );

                ri.Cvar_SetValue( "vid_fullscreen", 0 );
                ri.Con_Printf( PRINT_ALL,
                               "ref_soft::R_BeginFrame() - fullscreen "
                               "unavailable in this mode\n" );
                sw_state.prev_mode = sw_mode->value;
                //                vid_fullscreen->modified = false;
                //                sw_mode->modified = false;
            }
            else
            {
                ri.Sys_Error( ERR_FATAL,
                              "ref_soft::R_BeginFrame() - catastrophic mode "
                              "change failure\n" );
            }
        }
    }
}

/*
** ref_soft_R_GammaCorrectAndSetPalette
*/
void ref_soft_R_GammaCorrectAndSetPalette( const unsigned char* palette )
{
    int i;

    for ( i = 0; i < 256; i++ )
    {
        sw_state.currentpalette[i * 4 + 0] =
            sw_state.gammatable[palette[i * 4 + 0]];
        sw_state.currentpalette[i * 4 + 1] =
            sw_state.gammatable[palette[i * 4 + 1]];
        sw_state.currentpalette[i * 4 + 2] =
            sw_state.gammatable[palette[i * 4 + 2]];
    }

    SWimp_SetPalette( sw_state.currentpalette );
}

/*
** ref_soft_R_CinematicSetPalette
*/
void ref_soft_R_CinematicSetPalette( const unsigned char* palette )
{
    byte palette32[1024];
    int i, j, w;
    int* d;

    // clear screen to black to avoid any palette flash
    w = abs( quake2::getInstance()->vid.rowbytes ) >>
        2;  // stupid negative pitch win32 stuff...
    for ( i = 0; i < quake2::getInstance()->vid.height; i++, d += w )
    {
        d = (int*)( quake2::getInstance()->vid.buffer +
                    i * quake2::getInstance()->vid.rowbytes );
        for ( j = 0; j < w; j++ ) d[j] = 0;
    }
    // flush it to the screen
    SWimp_EndFrame();

    if ( palette )
    {
        for ( i = 0; i < 256; i++ )
        {
            palette32[i * 4 + 0] = palette[i * 3 + 0];
            palette32[i * 4 + 1] = palette[i * 3 + 1];
            palette32[i * 4 + 2] = palette[i * 3 + 2];
            palette32[i * 4 + 3] = 0xFF;
        }

        ref_soft_R_GammaCorrectAndSetPalette( palette32 );
    }
    else
    {
        ref_soft_R_GammaCorrectAndSetPalette(
            (const unsigned char*)d_8to24table );
    }
}

/*
================
Draw_BuildGammaTable
================
*/
void Draw_BuildGammaTable( void )
{
    int i, inf;
    float g;

    g = vid_gamma->value;

    if ( g == 1.0 )
    {
        for ( i = 0; i < 256; i++ ) sw_state.gammatable[i] = i;
        return;
    }

    for ( i = 0; i < 256; i++ )
    {
        inf = 255 * pow( ( i + 0.5 ) / 255.5, g ) + 0.5;
        if ( inf < 0 )
            inf = 0;
        if ( inf > 255 )
            inf = 255;
        sw_state.gammatable[i] = inf;
    }
}

/*
** ref_soft_R_DrawBeam
*/
void ref_soft_R_DrawBeam( entity_s* e )
{
#define NUM_BEAM_SEGS 6

    int i;

    vec3_t perpvec;
    vec3_t direction, normalized_direction;
    vec3_t start_points[NUM_BEAM_SEGS], end_points[NUM_BEAM_SEGS];
    vec3_t oldorigin, origin;

    oldorigin[0] = e->oldorigin[0];
    oldorigin[1] = e->oldorigin[1];
    oldorigin[2] = e->oldorigin[2];

    origin[0] = e->origin[0];
    origin[1] = e->origin[1];
    origin[2] = e->origin[2];

    normalized_direction[0] = direction[0] = oldorigin[0] - origin[0];
    normalized_direction[1] = direction[1] = oldorigin[1] - origin[1];
    normalized_direction[2] = direction[2] = oldorigin[2] - origin[2];

    if ( VectorNormalize( normalized_direction ) == 0 )
        return;

    PerpendicularVector( perpvec, normalized_direction );
    VectorScale( perpvec, e->frame / 2, perpvec );

    for ( i = 0; i < NUM_BEAM_SEGS; i++ )
    {
        RotatePointAroundVector( start_points[i], normalized_direction, perpvec,
                                 ( 360.0 / NUM_BEAM_SEGS ) * i );
        VectorAdd( start_points[i], origin, start_points[i] );
        VectorAdd( start_points[i], direction, end_points[i] );
    }

    for ( i = 0; i < NUM_BEAM_SEGS; i++ )
    {
        R_IMFlatShadedQuad( start_points[i], end_points[i],
                            end_points[( i + 1 ) % NUM_BEAM_SEGS],
                            start_points[( i + 1 ) % NUM_BEAM_SEGS],
                            e->skinnum & 0xFF, e->alpha );
    }
}

//===================================================================

// 3dstudio environment map names

int r_skysideimage[6] = { 5, 2, 4, 1, 0, 3 };
extern mtexinfo_t r_skytexinfo[6];
void ref_soft_R_SetSky( char* name, float rotate, vec3_t axis )
{
    int i;
    char pathname[MAX_QPATH];

    strncpy( skyname, name, sizeof( skyname ) - 1 );
    skyrotate = rotate;
    VectorCopy( axis, skyaxis );

    for ( i = 0; i < 6; i++ )
    {
        // Com_sprintf (pathname, sizeof(pathname), "env/%s%s.pcx", skyname,
        // suf[r_skysideimage[i]]);
        r_skytexinfo[i].image = ref_soft_R_FindImage( pathname, it_sky );
    }
}

struct image_s* ref_soft_R_RegisterSkin( char* name );

/*
@@@@@@@@@@@@@@@@@@@@@
GetRefAPI

@@@@@@@@@@@@@@@@@@@@@
*/
refexport_t ref_soft_GetRefAPI( refimport_t rimp )
{
    refexport_t re;

    ri = rimp;

    re.api_version = API_VERSION;

    re.BeginRegistration = ref_soft_R_BeginRegistration;
    re.RegisterModel = ref_soft_R_RegisterModel;
    re.RegisterSkin = ref_soft_R_RegisterSkin;
    re.RegisterPic = ref_gl_Draw_FindPic;
    re.SetSky = ref_soft_R_SetSky;
    re.EndRegistration = ref_soft_R_EndRegistration;

    re.RenderFrame = ref_soft_R_RenderFrame;

    re.DrawGetPicSize = ref_soft_Draw_GetPicSize;
    re.DrawPic = ref_soft_Draw_Pic;
    re.DrawStretchPic = ref_soft_Draw_StretchPic;
    re.DrawChar = ref_soft_Draw_Char;
    re.DrawTileClear = ref_soft_Draw_TileClear;
    re.DrawFill = ref_soft_Draw_Fill;
    re.DrawFadeScreen = ref_soft_Draw_FadeScreen;

    re.DrawStretchRaw = ref_soft_Draw_StretchRaw;

    re.Init = ref_soft_R_Init;
    re.Shutdown = ref_soft_R_Shutdown;

    re.CinematicSetPalette = ref_soft_R_CinematicSetPalette;
    re.BeginFrame = ref_soft_R_BeginFrame;
    re.EndFrame = SWimp_EndFrame;

    re.AppActivate = SWimp_AppActivate;

    Swap_Init();

    return re;
}

#ifndef REF_HARD_LINKED
// this is only here so the functions in q_shared.c and q_shwin.c can link
void Sys_Error( char* error, ... )
{
    va_list argptr;
    char text[1024];

    va_start( argptr, error );
    vsprintf( text, error, argptr );
    va_end( argptr );

    ri.Sys_Error( ERR_FATAL, "%s", text );
}

#endif
