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
#include "ref_gl/gl_local.hpp"
#include "shared/enum_rserr.hpp"

void ref_gl_R_Clear( void );
void ref_gl_GL_Strings_f( void );

/*
Returns true if the box is completely outside the frustom
*/
bool ref_gl_R_CullBox( vec3_t mins, vec3_t maxs )
{
    int i;

    if ( quake2::getInstance()->r_nocull->value )
        return false;

    for ( int i = 0; i < 4; i++ )
        if ( BOX_ON_PLANE_SIDE( mins, maxs, &frustum[i] ) == 2 )
            return true;
    return false;
}

void ref_gl_R_RotateForEntity( entity_s* e )
{
    qglTranslatef( e->origin[0], e->origin[1], e->origin[2] );

    qglRotatef( e->angles[1], 0, 0, 1 );
    qglRotatef( -e->angles[0], 0, 1, 0 );
    qglRotatef( -e->angles[2], 1, 0, 0 );
}

/*
=============================================================

  SPRITE MODELS

=============================================================
*/

/*
=================
ref_gl_R_DrawSpriteModel

=================
*/
void ref_gl_R_DrawSpriteModel( entity_s* e )
{
    float alpha = 1.0F;
    vec3_t point;
    dsprframe_t* frame;
    float *up, *right;
    dsprite_t* psprite;

    // don't even bother culling, because it's just a single
    // polygon without a surface cache

    psprite = (dsprite_t*)currentmodel->extradata;

#if 0
    if (e->frame < 0 || e->frame >= psprite->numframes)
    {
        ri.Con_Printf (PRINT_ALL, "no such sprite frame %i\n", e->frame);
        e->frame = 0;
    }
#endif
    e->frame %= psprite->numframes;

    frame = &psprite->frames[e->frame];

#if 0
    if (psprite->type == SPR_ORIENTED)
    {    // bullet marks on walls
    vec3_t        v_forward, v_right, v_up;

    AngleVectors (quake2::getInstance()->currententity->angles, v_forward, v_right, v_up);
        up = v_up;
        right = v_right;
    }
    else
#endif
    {  // normal sprite
        up = quake2::getInstance()->vup;
        right = quake2::getInstance()->vright;
    }

    if ( e->flags & RF_TRANSLUCENT )
        alpha = e->alpha;

    if ( alpha != 1.0F )
        qglEnable( GL_BLEND );

    qglColor4f( 1, 1, 1, alpha );

    ref_gl_GL_TexEnv( currentmodel->skins[e->frame]->texnum );

    ref_gl_GL_TexEnv( GL_MODULATE );

    if ( alpha == 1.0 )
        qglEnable( GL_ALPHA_TEST );
    else
        qglDisable( GL_ALPHA_TEST );

    qglBegin( GL_QUADS );

    qglTexCoord2f( 0, 1 );
    VectorMA( e->origin, -frame->origin_y, up, point );
    VectorMA( point, -frame->origin_x, right, point );
    qglVertex3fv( point );

    qglTexCoord2f( 0, 0 );
    VectorMA( e->origin, frame->height - frame->origin_y, up, point );
    VectorMA( point, -frame->origin_x, right, point );
    qglVertex3fv( point );

    qglTexCoord2f( 1, 0 );
    VectorMA( e->origin, frame->height - frame->origin_y, up, point );
    VectorMA( point, frame->width - frame->origin_x, right, point );
    qglVertex3fv( point );

    qglTexCoord2f( 1, 1 );
    VectorMA( e->origin, -frame->origin_y, up, point );
    VectorMA( point, frame->width - frame->origin_x, right, point );
    qglVertex3fv( point );

    qglEnd();

    qglDisable( GL_ALPHA_TEST );
    ref_gl_GL_TexEnv( GL_REPLACE );

    if ( alpha != 1.0F )
        qglDisable( GL_BLEND );

    qglColor4f( 1, 1, 1, 1 );
}

//==================================================================================

/*
=============
ref_gl_R_DrawNullModel
=============
*/
void ref_gl_R_DrawNullModel( void )
{
    vec3_t shadelight;
    int i;

    if ( quake2::getInstance()->currententity->flags & RF_FULLBRIGHT )
        shadelight[0] = shadelight[1] = shadelight[2] = 1.0F;
    else
        ref_gl_R_LightPoint( quake2::getInstance()->currententity->origin,
                             shadelight );

    qglPushMatrix();
    ref_gl_R_RotateForEntity( quake2::getInstance()->currententity );

    qglDisable( GL_TEXTURE_2D );
    qglColor3fv( shadelight );

    qglBegin( GL_TRIANGLE_FAN );
    qglVertex3f( 0, 0, -16 );
    for ( i = 0; i <= 4; i++ )
        qglVertex3f( 16 * cos( i * M_PI / 2 ), 16 * sin( i * M_PI / 2 ), 0 );
    qglEnd();

    qglBegin( GL_TRIANGLE_FAN );
    qglVertex3f( 0, 0, 16 );
    for ( i = 4; i >= 0; i-- )
        qglVertex3f( 16 * cos( i * M_PI / 2 ), 16 * sin( i * M_PI / 2 ), 0 );
    qglEnd();

    qglColor3f( 1, 1, 1 );
    qglPopMatrix();
    qglEnable( GL_TEXTURE_2D );
}

void ref_gl_R_DrawEntitiesOnList( void )
{
    int i;

    if ( !r_drawentities->value )
        return;

    // draw non-transparent first
    for ( i = 0; i < r_newrefdef.num_entities; i++ )
    {
        quake2::getInstance()->currententity = &r_newrefdef.entities[i];
        if ( quake2::getInstance()->currententity->flags & RF_TRANSLUCENT )
            continue;  // solid

        if ( quake2::getInstance()->currententity->flags & RF_BEAM )
        {
            ref_gl_R_DrawBeam( quake2::getInstance()->currententity );
        }
        else
        {
            currentmodel = quake2::getInstance()->currententity->model;
            if ( !currentmodel )
            {
                ref_gl_R_DrawNullModel();
                continue;
            }
            switch ( currentmodel->type )
            {
                case mod_alias:
                    ref_gl_R_DrawAliasModel(
                        quake2::getInstance()->currententity );
                    break;
                case mod_brush:
                    ref_gl_R_DrawBrushModel(
                        quake2::getInstance()->currententity );
                    break;
                case mod_sprite:
                    ref_gl_R_DrawSpriteModel(
                        quake2::getInstance()->currententity );
                    break;
                default:
                    ri.Sys_Error( ERR_DROP, "Bad modeltype" );
                    break;
            }
        }
    }

    // draw transparent entities
    // we could sort these if it ever becomes a problem...
    qglDepthMask( 0 );  // no z writes
    for ( i = 0; i < r_newrefdef.num_entities; i++ )
    {
        quake2::getInstance()->currententity = &r_newrefdef.entities[i];
        if ( !( quake2::getInstance()->currententity->flags & RF_TRANSLUCENT ) )
            continue;  // solid

        if ( quake2::getInstance()->currententity->flags & RF_BEAM )
        {
            ref_gl_R_DrawBeam( quake2::getInstance()->currententity );
        }
        else
        {
            currentmodel = quake2::getInstance()->currententity->model;

            if ( !currentmodel )
            {
                ref_gl_R_DrawNullModel();
                continue;
            }
            switch ( currentmodel->type )
            {
                case mod_alias:
                    ref_gl_R_DrawAliasModel(
                        quake2::getInstance()->currententity );
                    break;
                case mod_brush:
                    ref_gl_R_DrawBrushModel(
                        quake2::getInstance()->currententity );
                    break;
                case mod_sprite:
                    ref_gl_R_DrawSpriteModel(
                        quake2::getInstance()->currententity );
                    break;
                default:
                    ri.Sys_Error( ERR_DROP, "Bad modeltype" );
                    break;
            }
        }
    }
    qglDepthMask( 1 );  // back to writing
}

/*
** GL_DrawParticles
**
*/
void GL_DrawParticles( int num_particles, const particle_t particles[],
                       const unsigned colortable[768] )
{
    const particle_t* p;
    int i;
    vec3_t up, right;
    float scale;
    byte color[4];

    ref_gl_GL_Bind( r_particletexture->texnum );
    qglDepthMask( GL_FALSE );  // no z buffering
    qglEnable( GL_BLEND );
    ref_gl_GL_TexEnv( GL_MODULATE );
    qglBegin( GL_TRIANGLES );

    VectorScale( quake2::getInstance()->vup, 1.5, up );
    VectorScale( quake2::getInstance()->vright, 1.5, right );

    for ( p = particles, i = 0; i < num_particles; i++, p++ )
    {
        // hack a scale up to keep particles from disapearing
        scale = ( p->origin[0] - quake2::getInstance()->r_origin[0] ) *
                    quake2::getInstance()->vpn[0] +
                ( p->origin[1] - quake2::getInstance()->r_origin[1] ) *
                    quake2::getInstance()->vpn[1] +
                ( p->origin[2] - quake2::getInstance()->r_origin[2] ) *
                    quake2::getInstance()->vpn[2];

        if ( scale < 20 )
            scale = 1;
        else
            scale = 1 + scale * 0.004;

        *(int*)color = colortable[p->color];
        color[3] = p->alpha * 255;

        qglColor4ubv( color );

        qglTexCoord2f( 0.0625, 0.0625 );
        qglVertex3fv( p->origin );

        qglTexCoord2f( 1.0625, 0.0625 );
        qglVertex3f( p->origin[0] + up[0] * scale, p->origin[1] + up[1] * scale,
                     p->origin[2] + up[2] * scale );

        qglTexCoord2f( 0.0625, 1.0625 );
        qglVertex3f( p->origin[0] + right[0] * scale,
                     p->origin[1] + right[1] * scale,
                     p->origin[2] + right[2] * scale );
    }

    qglEnd();
    qglDisable( GL_BLEND );
    qglColor4f( 1, 1, 1, 1 );
    qglDepthMask( 1 );  // back to normal Z buffering
    ref_gl_GL_TexEnv( GL_REPLACE );
}

/*
===============
R_DrawParticles
===============
*/
void ref_gl_R_DrawParticles( void )
{
    if ( gl_ext_pointparameters->value && qglPointParameterfEXT )
    {
        int i;
        unsigned char color[4];
        const particle_t* p;

        qglDepthMask( GL_FALSE );
        qglEnable( GL_BLEND );
        qglDisable( GL_TEXTURE_2D );

        qglPointSize( gl_particle_size->value );

        qglBegin( GL_POINTS );
        for ( i = 0, p = r_newrefdef.particles; i < r_newrefdef.num_particles;
              i++, p++ )
        {
            *(int*)color = d_8to24table[p->color];
            color[3] = p->alpha * 255;

            qglColor4ubv( color );

            qglVertex3fv( p->origin );
        }
        qglEnd();

        qglDisable( GL_BLEND );
        qglColor4f( 1.0F, 1.0F, 1.0F, 1.0F );
        qglDepthMask( GL_TRUE );
        qglEnable( GL_TEXTURE_2D );
    }
    else
    {
        GL_DrawParticles( r_newrefdef.num_particles, r_newrefdef.particles,
                          d_8to24table );
    }
}

/*
============
ref_gl_R_PolyBlend
============
*/
void ref_gl_R_PolyBlend( void )
{
    if ( !gl_polyblend->value )
        return;
    if ( !v_blend[3] )
        return;

    qglDisable( GL_ALPHA_TEST );
    qglEnable( GL_BLEND );
    qglDisable( GL_DEPTH_TEST );
    qglDisable( GL_TEXTURE_2D );

    qglLoadIdentity();

    // FIXME: get rid of these
    qglRotatef( -90, 1, 0, 0 );  // put Z going up
    qglRotatef( 90, 0, 0, 1 );   // put Z going up

    qglColor4fv( v_blend );

    qglBegin( GL_QUADS );

    qglVertex3f( 10, 100, 100 );
    qglVertex3f( 10, -100, 100 );
    qglVertex3f( 10, -100, -100 );
    qglVertex3f( 10, 100, -100 );
    qglEnd();

    qglDisable( GL_BLEND );
    qglEnable( GL_TEXTURE_2D );
    qglEnable( GL_ALPHA_TEST );

    qglColor4f( 1, 1, 1, 1 );
}

//=======================================================================

int SignbitsForPlane( plane_s* out )
{
    int bits, j;

    // for fast box on planeside test

    bits = 0;
    for ( j = 0; j < 3; j++ )
    {
        if ( out->normal[j] < 0 )
            bits |= 1 << j;
    }
    return bits;
}

void ref_gl_R_SetFrustum( void )
{
    int i;

#if 0
    /*
    ** this code is wrong, since it presume a 90 degree FOV both in the
    ** horizontal and vertical plane
    */
    // front side is visible
    VectorAdd (quake2::getInstance()->vpn, quake2::getInstance()->vright, frustum[0].normal);
    VectorSubtract (quake2::getInstance()->vpn, quake2::getInstance()->vright, frustum[1].normal);
    VectorAdd (quake2::getInstance()->vpn, quake2::getInstance()->vup, frustum[2].normal);
    VectorSubtract (quake2::getInstance()->vpn, quake2::getInstance()->vup, frustum[3].normal);

    // we theoretically don't need to normalize these vectors, but I do it
    // anyway so that debugging is a little easier
    VectorNormalize( frustum[0].normal );
    VectorNormalize( frustum[1].normal );
    VectorNormalize( frustum[2].normal );
    VectorNormalize( frustum[3].normal );
#else
    // rotate VPN right by FOV_X/2 degrees
    RotatePointAroundVector( frustum[0].normal, quake2::getInstance()->vup,
                             quake2::getInstance()->vpn,
                             -( 90 - r_newrefdef.fov_x / 2 ) );
    // rotate VPN left by FOV_X/2 degrees
    RotatePointAroundVector( frustum[1].normal, quake2::getInstance()->vup,
                             quake2::getInstance()->vpn,
                             90 - r_newrefdef.fov_x / 2 );
    // rotate VPN up by FOV_X/2 degrees
    RotatePointAroundVector( frustum[2].normal, quake2::getInstance()->vright,
                             quake2::getInstance()->vpn,
                             90 - r_newrefdef.fov_y / 2 );
    // rotate VPN down by FOV_X/2 degrees
    RotatePointAroundVector( frustum[3].normal, quake2::getInstance()->vright,
                             quake2::getInstance()->vpn,
                             -( 90 - r_newrefdef.fov_y / 2 ) );
#endif

    for ( i = 0; i < 4; i++ )
    {
        frustum[i].type = PLANE_ANYZ;
        frustum[i].dist =
            DotProduct( quake2::getInstance()->r_origin, frustum[i].normal );
        frustum[i].signbits = SignbitsForPlane( &frustum[i] );
    }
}

//=======================================================================

/*
===============
R_SetupFrame
===============
*/
void ref_gl_R_SetupFrame( void )
{
    int i;
    mleaf_t* leaf;

    quake2::getInstance()->r_framecount++;

    // build the transformation matrix for the given view angles
    VectorCopy( r_newrefdef.vieworg, quake2::getInstance()->r_origin );

    AngleVectors( r_newrefdef.viewangles, quake2::getInstance()->vpn,
                  quake2::getInstance()->vright, quake2::getInstance()->vup );

    // current viewcluster
    if ( !( r_newrefdef.rdflags & RDF_NOWORLDMODEL ) )
    {
        quake2::getInstance()->r_oldviewcluster =
            quake2::getInstance()->r_viewcluster;
        quake2::getInstance()->r_oldviewcluster2 =
            quake2::getInstance()->r_viewcluster2;
        leaf = ref_gl_Mod_PointInLeaf( quake2::getInstance()->r_origin,
                                       r_worldmodel );
        quake2::getInstance()->r_viewcluster =
            quake2::getInstance()->r_viewcluster2 = leaf->cluster;

        // check above and below so crossing solid water doesn't draw wrong
        if ( !leaf->contents )
        {  // look down a bit
            vec3_t temp;

            VectorCopy( quake2::getInstance()->r_origin, temp );
            temp[2] -= 16;
            leaf = ref_gl_Mod_PointInLeaf( temp, r_worldmodel );
            if ( !( leaf->contents & CONTENTS_SOLID ) &&
                 ( leaf->cluster != quake2::getInstance()->r_viewcluster2 ) )
                quake2::getInstance()->r_viewcluster2 = leaf->cluster;
        }
        else
        {  // look up a bit
            vec3_t temp;

            VectorCopy( quake2::getInstance()->r_origin, temp );
            temp[2] += 16;
            leaf = ref_gl_Mod_PointInLeaf( temp, r_worldmodel );
            if ( !( leaf->contents & CONTENTS_SOLID ) &&
                 ( leaf->cluster != quake2::getInstance()->r_viewcluster2 ) )
                quake2::getInstance()->r_viewcluster2 = leaf->cluster;
        }
    }

    for ( i = 0; i < 4; i++ ) v_blend[i] = r_newrefdef.blend[i];

    c_brush_polys = 0;
    c_alias_polys = 0;

    // clear out the portion of the screen that the NOWORLDMODEL defines
    if ( r_newrefdef.rdflags & RDF_NOWORLDMODEL )
    {
        qglEnable( GL_SCISSOR_TEST );
        qglClearColor( 0.3, 0.3, 0.3, 1 );
        qglScissor( r_newrefdef.x,
                    quake2::getInstance()->vid.height - r_newrefdef.height -
                        r_newrefdef.y,
                    r_newrefdef.width, r_newrefdef.height );
        qglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        qglClearColor( 1, 0, 0.5, 0.5 );
        qglDisable( GL_SCISSOR_TEST );
    }
}

void MYgluPerspective( GLdouble fovy, GLdouble aspect, GLdouble zNear,
                       GLdouble zFar )
{
    GLdouble xmin, xmax, ymin, ymax;

    ymax = zNear * tan( fovy * M_PI / 360.0 );
    ymin = -ymax;

    xmin = ymin * aspect;
    xmax = ymax * aspect;

    xmin += -( 2 * gl_state.camera_separation ) / zNear;
    xmax += -( 2 * gl_state.camera_separation ) / zNear;

    qglFrustum( xmin, xmax, ymin, ymax, zNear, zFar );
}

/*
=============
R_SetupGL
=============
*/
void ref_gl_R_SetupGL( void )
{
    float screenaspect;
    //    float    yfov;
    int x, x2, y2, y, w, h;

    //
    // set up viewport
    //
    x = floor( r_newrefdef.x * quake2::getInstance()->vid.width /
               quake2::getInstance()->vid.width );
    x2 = ceil( ( r_newrefdef.x + r_newrefdef.width ) *
               quake2::getInstance()->vid.width /
               quake2::getInstance()->vid.width );
    y = floor( quake2::getInstance()->vid.height -
               r_newrefdef.y * quake2::getInstance()->vid.height /
                   quake2::getInstance()->vid.height );
    y2 = ceil( quake2::getInstance()->vid.height -
               ( r_newrefdef.y + r_newrefdef.height ) *
                   quake2::getInstance()->vid.height /
                   quake2::getInstance()->vid.height );

    w = x2 - x;
    h = y - y2;

    qglViewport( x, y2, w, h );

    //
    // set up projection matrix
    //
    screenaspect = (float)r_newrefdef.width / r_newrefdef.height;
    //    yfov = 2*atan((float)r_newrefdef.height/r_newrefdef.width)*180/M_PI;
    qglMatrixMode( GL_PROJECTION );
    qglLoadIdentity();
    MYgluPerspective( r_newrefdef.fov_y, screenaspect, 4, 4096 );

    qglCullFace( GL_FRONT );

    qglMatrixMode( GL_MODELVIEW );
    qglLoadIdentity();

    qglRotatef( -90, 1, 0, 0 );  // put Z going up
    qglRotatef( 90, 0, 0, 1 );   // put Z going up
    qglRotatef( -r_newrefdef.viewangles[2], 1, 0, 0 );
    qglRotatef( -r_newrefdef.viewangles[0], 0, 1, 0 );
    qglRotatef( -r_newrefdef.viewangles[1], 0, 0, 1 );
    qglTranslatef( -r_newrefdef.vieworg[0], -r_newrefdef.vieworg[1],
                   -r_newrefdef.vieworg[2] );

    //    if ( gl_state.camera_separation != 0 && gl_state.stereo_enabled )
    //        qglTranslatef ( gl_state.camera_separation, 0, 0 );

    qglGetFloatv( GL_MODELVIEW_MATRIX, r_world_matrix );

    //
    // set drawing parms
    //
    if ( gl_cull->value )
        qglEnable( GL_CULL_FACE );
    else
        qglDisable( GL_CULL_FACE );

    qglDisable( GL_BLEND );
    qglDisable( GL_ALPHA_TEST );
    qglEnable( GL_DEPTH_TEST );
}

void ref_gl_R_Clear( void )
{
    if ( gl_ztrick->value )
    {
        static int trickframe;

        if ( gl_clear->value )
            qglClear( GL_COLOR_BUFFER_BIT );

        trickframe++;
        if ( trickframe & 1 )
        {
            gldepthmin = 0;
            gldepthmax = 0.49999;
            qglDepthFunc( GL_LEQUAL );
        }
        else
        {
            gldepthmin = 1;
            gldepthmax = 0.5;
            qglDepthFunc( GL_GEQUAL );
        }
    }
    else
    {
        if ( gl_clear->value )
            qglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        else
            qglClear( GL_DEPTH_BUFFER_BIT );
        gldepthmin = 0;
        gldepthmax = 1;
        qglDepthFunc( GL_LEQUAL );
    }

    qglDepthRange( gldepthmin, gldepthmax );
}

void ref_gl_R_Flash( void ) { ref_gl_R_PolyBlend(); }

/*
================
ref_gl_R_RenderView

r_newrefdef must be set before the first call
================
*/
void ref_gl_R_RenderView( refdef_t* fd )
{
    if ( r_norefresh->value )
        return;

    r_newrefdef = *fd;

    if ( !r_worldmodel && !( r_newrefdef.rdflags & RDF_NOWORLDMODEL ) )
        ri.Sys_Error( ERR_DROP, "ref_gl_R_RenderView: NULL worldmodel" );

    if ( r_speeds->value )
    {
        c_brush_polys = 0;
        c_alias_polys = 0;
    }

    ref_gl_R_PushDlights();

    if ( gl_finish->value )
        qglFinish();

    ref_gl_R_SetupFrame();

    ref_gl_R_SetFrustum();

    ref_gl_R_SetupGL();

    ref_gl_R_MarkLeaves();  // done here so we know if we're in water

    ref_gl_R_DrawWorld();

    ref_gl_R_DrawEntitiesOnList();

    ref_gl_R_RenderDlights();

    ref_gl_R_DrawParticles();

    ref_gl_R_DrawAlphaSurfaces();

    ref_gl_R_Flash();

    if ( r_speeds->value )
    {
        ri.Con_Printf( PRINT_ALL, "%4i wpoly %4i epoly %i tex %i lmaps\n",
                       c_brush_polys, c_alias_polys, c_visible_textures,
                       c_visible_lightmaps );
    }
}

void R_SetGL2D( void )
{
    // set 2D virtual screen size
    qglViewport( 0, 0, quake2::getInstance()->vid.width,
                 quake2::getInstance()->vid.height );
    qglMatrixMode( GL_PROJECTION );
    qglLoadIdentity();
    qglOrtho( 0, quake2::getInstance()->vid.width,
              quake2::getInstance()->vid.height, 0, -99999, 99999 );
    qglMatrixMode( GL_MODELVIEW );
    qglLoadIdentity();
    qglDisable( GL_DEPTH_TEST );
    qglDisable( GL_CULL_FACE );
    qglDisable( GL_BLEND );
    qglEnable( GL_ALPHA_TEST );
    qglColor4f( 1, 1, 1, 1 );
}

static void GL_DrawColoredStereoLinePair( float r, float g, float b, float y )
{
    qglColor3f( r, g, b );
    qglVertex2f( 0, y );
    qglVertex2f( quake2::getInstance()->vid.width, y );
    qglColor3f( 0, 0, 0 );
    qglVertex2f( 0, y + 1 );
    qglVertex2f( quake2::getInstance()->vid.width, y + 1 );
}

static void GL_DrawStereoPattern( void )
{
    int i;

    if ( !( gl_config.renderer & GL_RENDERER_INTERGRAPH ) )
        return;

    if ( !gl_state.stereo_enabled )
        return;

    R_SetGL2D();

    qglDrawBuffer( GL_BACK_LEFT );

    for ( i = 0; i < 20; i++ )
    {
        qglBegin( GL_LINES );
        GL_DrawColoredStereoLinePair( 1, 0, 0, 0 );
        GL_DrawColoredStereoLinePair( 1, 0, 0, 2 );
        GL_DrawColoredStereoLinePair( 1, 0, 0, 4 );
        GL_DrawColoredStereoLinePair( 1, 0, 0, 6 );
        GL_DrawColoredStereoLinePair( 0, 1, 0, 8 );
        GL_DrawColoredStereoLinePair( 1, 1, 0, 10 );
        GL_DrawColoredStereoLinePair( 1, 1, 0, 12 );
        GL_DrawColoredStereoLinePair( 0, 1, 0, 14 );
        qglEnd();

        GLimp_EndFrame();
    }
}

void ref_gl_R_SetLightLevel( void )
{
    vec3_t shadelight;

    if ( r_newrefdef.rdflags & RDF_NOWORLDMODEL )
        return;

    // save off light value for server to look at (BIG HACK!)
    ref_gl_R_LightPoint( r_newrefdef.vieworg, shadelight );

    // pick the greatest component, which should be the same
    // as the mono value returned by software
    if ( shadelight[0] > shadelight[1] )
    {
        if ( shadelight[0] > shadelight[2] )
            r_lightlevel->value = 150 * shadelight[0];
        else
            r_lightlevel->value = 150 * shadelight[2];
    }
    else
    {
        if ( shadelight[1] > shadelight[2] )
            r_lightlevel->value = 150 * shadelight[1];
        else
            r_lightlevel->value = 150 * shadelight[2];
    }
}

/*
@@@@@@@@@@@@@@@@@@@@@
R_RenderFrame

@@@@@@@@@@@@@@@@@@@@@
*/
void ref_gl_R_RenderFrame( refdef_t* fd )
{
    ref_gl_R_RenderView( fd );
    ref_gl_R_SetLightLevel();
    R_SetGL2D();
}

void ref_gl_R_Register( void )
{
    quake2::getInstance()->r_lefthand =
        ri.Cvar_Get( "hand", "0", CVAR_USERINFO | CVAR_ARCHIVE );
    r_norefresh = ri.Cvar_Get( "r_norefresh", "0", 0 );
    quake2::getInstance()->r_fullbright = ri.Cvar_Get( "r_fullbright", "0", 0 );
    r_drawentities = ri.Cvar_Get( "r_drawentities", "1", 0 );
    quake2::getInstance()->r_drawworld = ri.Cvar_Get( "r_drawworld", "1", 0 );
    quake2::getInstance()->r_novis = ri.Cvar_Get( "r_novis", "0", 0 );
    quake2::getInstance()->r_nocull = ri.Cvar_Get( "r_nocull", "0", 0 );
    quake2::getInstance()->quake2::getInstance()->r_lerpmodels =
        ri.Cvar_Get( "r_lerpmodels", "1", 0 );
    r_speeds = ri.Cvar_Get( "r_speeds", "0", 0 );

    r_lightlevel = ri.Cvar_Get( "r_lightlevel", "0", 0 );

    gl_nosubimage = ri.Cvar_Get( "gl_nosubimage", "0", 0 );
    gl_allow_software = ri.Cvar_Get( "gl_allow_software", "0", 0 );

    gl_particle_min_size =
        ri.Cvar_Get( "gl_particle_min_size", "2", CVAR_ARCHIVE );
    gl_particle_max_size =
        ri.Cvar_Get( "gl_particle_max_size", "40", CVAR_ARCHIVE );
    gl_particle_size = ri.Cvar_Get( "gl_particle_size", "40", CVAR_ARCHIVE );
    gl_particle_att_a =
        ri.Cvar_Get( "gl_particle_att_a", "0.01", CVAR_ARCHIVE );
    gl_particle_att_b = ri.Cvar_Get( "gl_particle_att_b", "0.0", CVAR_ARCHIVE );
    gl_particle_att_c =
        ri.Cvar_Get( "gl_particle_att_c", "0.01", CVAR_ARCHIVE );

    gl_modulate = ri.Cvar_Get( "gl_modulate", "1", CVAR_ARCHIVE );
    gl_log = ri.Cvar_Get( "gl_log", "0", 0 );
    gl_bitdepth = ri.Cvar_Get( "gl_bitdepth", "0", 0 );
    gl_mode = ri.Cvar_Get( "gl_mode", "3", CVAR_ARCHIVE );
    gl_lightmap = ri.Cvar_Get( "gl_lightmap", "0", 0 );
    gl_shadows = ri.Cvar_Get( "gl_shadows", "0", CVAR_ARCHIVE );
    gl_dynamic = ri.Cvar_Get( "gl_dynamic", "1", 0 );
    gl_nobind = ri.Cvar_Get( "gl_nobind", "0", 0 );
    gl_round_down = ri.Cvar_Get( "gl_round_down", "1", 0 );
    gl_picmip = ri.Cvar_Get( "gl_picmip", "0", 0 );
    gl_skymip = ri.Cvar_Get( "gl_skymip", "0", 0 );
    gl_showtris = ri.Cvar_Get( "gl_showtris", "0", 0 );
    gl_ztrick = ri.Cvar_Get( "gl_ztrick", "0", 0 );
    gl_finish = ri.Cvar_Get( "gl_finish", "0", CVAR_ARCHIVE );
    gl_clear = ri.Cvar_Get( "gl_clear", "0", 0 );
    gl_cull = ri.Cvar_Get( "gl_cull", "1", 0 );
    gl_polyblend = ri.Cvar_Get( "gl_polyblend", "1", 0 );
    gl_flashblend = ri.Cvar_Get( "gl_flashblend", "0", 0 );
    gl_playermip = ri.Cvar_Get( "gl_playermip", "0", 0 );
    gl_monolightmap = ri.Cvar_Get( "gl_monolightmap", "0", 0 );
    gl_driver = ri.Cvar_Get( "gl_driver", "opengl32", CVAR_ARCHIVE );
    gl_texturemode = ri.Cvar_Get( "gl_texturemode", "GL_LINEAR_MIPMAP_NEAREST",
                                  CVAR_ARCHIVE );
    gl_texturealphamode =
        ri.Cvar_Get( "gl_texturealphamode", "default", CVAR_ARCHIVE );
    gl_texturesolidmode =
        ri.Cvar_Get( "gl_texturesolidmode", "default", CVAR_ARCHIVE );
    gl_lockpvs = ri.Cvar_Get( "gl_lockpvs", "0", 0 );

    gl_vertex_arrays = ri.Cvar_Get( "gl_vertex_arrays", "0", CVAR_ARCHIVE );

    gl_ext_swapinterval =
        ri.Cvar_Get( "gl_ext_swapinterval", "1", CVAR_ARCHIVE );
    gl_ext_palettedtexture =
        ri.Cvar_Get( "gl_ext_palettedtexture", "1", CVAR_ARCHIVE );
    gl_ext_multitexture =
        ri.Cvar_Get( "gl_ext_multitexture", "1", CVAR_ARCHIVE );
    gl_ext_pointparameters =
        ri.Cvar_Get( "gl_ext_pointparameters", "1", CVAR_ARCHIVE );
    gl_ext_compiled_vertex_array =
        ri.Cvar_Get( "gl_ext_compiled_vertex_array", "1", CVAR_ARCHIVE );

    gl_drawbuffer = ri.Cvar_Get( "gl_drawbuffer", "GL_BACK", 0 );
    gl_swapinterval = ri.Cvar_Get( "gl_swapinterval", "1", CVAR_ARCHIVE );

    gl_saturatelighting = ri.Cvar_Get( "gl_saturatelighting", "0", 0 );

    gl_3dlabs_broken = ri.Cvar_Get( "gl_3dlabs_broken", "1", CVAR_ARCHIVE );

    vid_fullscreen = ri.Cvar_Get( "vid_fullscreen", "0", CVAR_ARCHIVE );
    vid_gamma = ri.Cvar_Get( "vid_gamma", "1.0", CVAR_ARCHIVE );
    vid_ref = ri.Cvar_Get( "vid_ref", "soft", CVAR_ARCHIVE );

    ri.Cmd_AddCommand( "imagelist", ref_gl_GL_ImageList_f );
    ri.Cmd_AddCommand( "screenshot", ref_gl_GL_ScreenShot_f );
    ri.Cmd_AddCommand( "modellist", ref_gl_Mod_Modellist_f );
    ri.Cmd_AddCommand( "gl_strings", ref_gl_GL_Strings_f );
}

/*
==================
R_SetMode
==================
*/
bool R_SetMode( void )
{
    rserr_t err;
    bool fullscreen;

    if ( vid_fullscreen->modified && !gl_config.allow_cds )
    {
        ri.Con_Printf( PRINT_ALL,
                       "R_SetMode() - CDS not allowed with this driver\n" );
        ri.Cvar_SetValue( "vid_fullscreen", !vid_fullscreen->value );
        vid_fullscreen->modified = false;
    }

    fullscreen = vid_fullscreen->value;

    fullscreen = false;

    vid_fullscreen->modified = false;
    gl_mode->modified = false;

    if ( ( err = GLimp_SetMode( &quake2::getInstance()->vid.width,
                                &quake2::getInstance()->vid.height,
                                gl_mode->value, fullscreen ) ) == rserr_ok )
    {
        gl_state.prev_mode = gl_mode->value;
    }
    else
    {
        if ( err == rserr_invalid_fullscreen )
        {
            ri.Cvar_SetValue( "vid_fullscreen", 0 );
            vid_fullscreen->modified = false;
            ri.Con_Printf(
                PRINT_ALL,
                "R_SetMode() - fullscreen unavailable in this mode\n" );
            if ( ( err = GLimp_SetMode( &quake2::getInstance()->vid.width,
                                        &quake2::getInstance()->vid.height,
                                        gl_mode->value, false ) ) == rserr_ok )
                return true;
        }
        else if ( err == rserr_invalid_mode )
        {
            ri.Cvar_SetValue( "gl_mode", gl_state.prev_mode );
            gl_mode->modified = false;
            ri.Con_Printf( PRINT_ALL, "R_SetMode() - invalid mode\n" );
        }

        // try setting it back to something safe
        if ( ( err = GLimp_SetMode( &quake2::getInstance()->vid.width,
                                    &quake2::getInstance()->vid.height,
                                    gl_state.prev_mode, false ) ) != rserr_ok )
        {
            ri.Con_Printf( PRINT_ALL,
                           "R_SetMode() - could not revert to safe mode\n" );
            return false;
        }
    }
    return true;
}

/*
===============
R_Init
===============
*/
int ref_gl_R_Init( void* hinstance, void* hWnd )
{
    return 1;
    // TODO:
    //     char renderer_buffer[1000];
    //     char vendor_buffer[1000];
    //     int err;
    //     int j;
    //     extern float r_turbsin[256];

    //     for ( j = 0; j < 256; j++ )
    //     {
    //         r_turbsin[j] *= 0.5;
    //     }

    //     ri.Con_Printf( PRINT_ALL, "ref_gl version: " REF_VERSION "\n" );

    //     ref_gl_Draw_GetPalette();

    //     ref_gl_R_Register();

    //     // initialize our QGL dynamic bindings
    //     if ( !QGL_Init( gl_driver->string ) )
    //     {
    //         QGL_Shutdown();
    //         ri.Con_Printf( PRINT_ALL, "ref_gl::R_Init() - could not load
    //         \"%s\"\n",
    //                        gl_driver->string );
    //         return -1;
    //     }

    //     // initialize OS-specific parts of OpenGL
    //     if ( !GLimp_Init( hinstance, hWnd ) )
    //     {
    //         QGL_Shutdown();
    //         return -1;
    //     }

    //     // set our "safe" modes
    //     gl_state.prev_mode = 3;

    //     // create the window and set up the context
    //     if ( !R_SetMode() )
    //     {
    //         QGL_Shutdown();
    //         ri.Con_Printf( PRINT_ALL,
    //                        "ref_gl::R_Init() - could not
    //                        ref_gl_R_SetMode()\n" );
    //         return -1;
    //     }

    //     ri.Vid_MenuInit();

    //     /*
    //     ** get our various GL strings
    //     */
    //     gl_config.vendor_string = qglGetString( GL_VENDOR );
    //     ri.Con_Printf( PRINT_ALL, "GL_VENDOR: %s\n", gl_config.vendor_string
    //     ); gl_config.renderer_string = qglGetString( GL_RENDERER );
    //     ri.Con_Printf( PRINT_ALL, "GL_RENDERER: %s\n",
    //     gl_config.renderer_string ); gl_config.version_string = qglGetString(
    //     GL_VERSION ); ri.Con_Printf( PRINT_ALL, "GL_VERSION: %s\n",
    //     gl_config.version_string ); gl_config.extensions_string =
    //     qglGetString( GL_EXTENSIONS ); ri.Con_Printf( PRINT_ALL,
    //     "GL_EXTENSIONS: %s\n",
    //                    gl_config.extensions_string );

    //     strcpy( renderer_buffer, gl_config.renderer_string );
    //     strlwr( renderer_buffer );

    //     strcpy( vendor_buffer, gl_config.vendor_string );
    //     strlwr( vendor_buffer );

    //     if ( strstr( renderer_buffer, "voodoo" ) )
    //     {
    //         if ( !strstr( renderer_buffer, "rush" ) )
    //             gl_config.renderer = GL_RENDERER_VOODOO;
    //         else
    //             gl_config.renderer = GL_RENDERER_VOODOO_RUSH;
    //     }
    //     else if ( strstr( vendor_buffer, "sgi" ) )
    //         gl_config.renderer = GL_RENDERER_SGI;
    //     else if ( strstr( renderer_buffer, "permedia" ) )
    //         gl_config.renderer = GL_RENDERER_PERMEDIA2;
    //     else if ( strstr( renderer_buffer, "glint" ) )
    //         gl_config.renderer = GL_RENDERER_GLINT_MX;
    //     else if ( strstr( renderer_buffer, "glzicd" ) )
    //         gl_config.renderer = GL_RENDERER_REALIZM;
    //     else if ( strstr( renderer_buffer, "gdi" ) )
    //         gl_config.renderer = GL_RENDERER_MCD;
    //     else if ( strstr( renderer_buffer, "pcx2" ) )
    //         gl_config.renderer = GL_RENDERER_PCX2;
    //     else if ( strstr( renderer_buffer, "verite" ) )
    //         gl_config.renderer = GL_RENDERER_RENDITION;
    //     else
    //         gl_config.renderer = GL_RENDERER_OTHER;

    //     if ( toupper( gl_monolightmap->string[1] ) != 'F' )
    //     {
    //         if ( gl_config.renderer == GL_RENDERER_PERMEDIA2 )
    //         {
    //             ri.Cvar_Set( "gl_monolightmap", "A" );
    //             ri.Con_Printf( PRINT_ALL, "...using gl_monolightmap 'a'\n" );
    //         }
    //         else if ( gl_config.renderer & GL_RENDERER_POWERVR )
    //         {
    //             ri.Cvar_Set( "gl_monolightmap", "0" );
    //         }
    //         else
    //         {
    //             ri.Cvar_Set( "gl_monolightmap", "0" );
    //         }
    //     }

    //     // power vr can't have anything stay in the framebuffer, so
    //     // the screen needs to redraw the tiled background every frame
    //     if ( gl_config.renderer & GL_RENDERER_POWERVR )
    //     {
    //         ri.Cvar_Set( "scr_drawall", "1" );
    //     }
    //     else
    //     {
    //         ri.Cvar_Set( "scr_drawall", "0" );
    //     }

    //     // MCD has buffering issues
    //     if ( gl_config.renderer == GL_RENDERER_MCD )
    //     {
    //         ri.Cvar_SetValue( "gl_finish", 1 );
    //     }

    //     if ( gl_config.renderer & GL_RENDERER_3DLABS )
    //     {
    //         if ( gl_3dlabs_broken->value )
    //             gl_config.allow_cds = false;
    //         else
    //             gl_config.allow_cds = true;
    //     }
    //     else
    //     {
    //         gl_config.allow_cds = true;
    //     }

    //     if ( gl_config.allow_cds )
    //         ri.Con_Printf( PRINT_ALL, "...allowing CDS\n" );
    //     else
    //         ri.Con_Printf( PRINT_ALL, "...disabling CDS\n" );

    //         /*
    //          ** grab extensions
    //          */
    // #ifdef WIN32
    //     if ( strstr( gl_config.extensions_string,
    //                  "GL_EXT_compiled_vertex_array" ) ||
    //          strstr( gl_config.extensions_string,
    //          "GL_SGI_compiled_vertex_array" ) )
    //     {
    //         ri.Con_Printf( PRINT_ALL,
    //                        "...enabling GL_EXT_compiled_vertex_array\n" );
    //         qglLockArraysEXT = (void*)qwglGetProcAddress( "glLockArraysEXT"
    //         ); qglUnlockArraysEXT = (void*)qwglGetProcAddress(
    //         "glUnlockArraysEXT" );
    //     }
    //     else
    //     {
    //         ri.Con_Printf( PRINT_ALL,
    //                        "...GL_EXT_compiled_vertex_array not found\n" );
    //     }

    //     if ( strstr( gl_config.extensions_string, "WGL_EXT_swap_control" ) )
    //     {
    //         qwglSwapIntervalEXT =
    //             (BOOL( WINAPI* )( int ))qwglGetProcAddress(
    //             "wglSwapIntervalEXT" );
    //         ri.Con_Printf( PRINT_ALL, "...enabling WGL_EXT_swap_control\n" );
    //     }
    //     else
    //     {
    //         ri.Con_Printf( PRINT_ALL, "...WGL_EXT_swap_control not found\n"
    //         );
    //     }

    //     if ( strstr( gl_config.extensions_string, "GL_EXT_point_parameters" )
    //     )
    //     {
    //         if ( gl_ext_pointparameters->value )
    //         {
    //             qglPointParameterfEXT = (void( APIENTRY* )(
    //                 GLenum, GLfloat ))qwglGetProcAddress(
    //                 "glPointParameterfEXT" );
    //             qglPointParameterfvEXT =
    //                 (void( APIENTRY* )( GLenum, const GLfloat*
    //                 ))qwglGetProcAddress(
    //                     "glPointParameterfvEXT" );
    //             ri.Con_Printf( PRINT_ALL, "...using
    //             GL_EXT_point_parameters\n" );
    //         }
    //         else
    //         {
    //             ri.Con_Printf( PRINT_ALL, "...ignoring
    //             GL_EXT_point_parameters\n" );
    //         }
    //     }
    //     else
    //     {
    //         ri.Con_Printf( PRINT_ALL, "...GL_EXT_point_parameters not
    //         found\n" );
    //     }

    //     if ( strstr( gl_config.extensions_string, "GL_EXT_paletted_texture" )
    //     &&
    //          strstr( gl_config.extensions_string,
    //                  "GL_EXT_shared_texture_palette" ) )
    //     {
    //         if ( gl_ext_palettedtexture->value )
    //         {
    //             ri.Con_Printf( PRINT_ALL,
    //                            "...using GL_EXT_shared_texture_palette\n" );
    //             qglColorTableEXT = (void( APIENTRY* )(
    //                 int, int, int, int, int,
    //                 const void* ))qwglGetProcAddress( "glColorTableEXT" );
    //         }
    //         else
    //         {
    //             ri.Con_Printf( PRINT_ALL,
    //                            "...ignoring GL_EXT_shared_texture_palette\n"
    //                            );
    //         }
    //     }
    //     else
    //     {
    //         ri.Con_Printf( PRINT_ALL,
    //                        "...GL_EXT_shared_texture_palette not found\n" );
    //     }

    //     if ( strstr( gl_config.extensions_string, "GL_SGIS_multitexture" ) )
    //     {
    //         if ( gl_ext_multitexture->value )
    //         {
    //             ri.Con_Printf( PRINT_ALL, "...using GL_SGIS_multitexture\n"
    //             ); qglMTexCoord2fSGIS =
    //                 (void*)qwglGetProcAddress( "glMTexCoord2fSGIS" );
    //             qglSelectTextureSGIS =
    //                 (void*)qwglGetProcAddress( "glSelectTextureSGIS" );
    //         }
    //         else
    //         {
    //             ri.Con_Printf( PRINT_ALL, "...ignoring
    //             GL_SGIS_multitexture\n" );
    //         }
    //     }
    //     else
    //     {
    //         ri.Con_Printf( PRINT_ALL, "...GL_SGIS_multitexture not found\n"
    //         );
    //     }
    // #endif

    //     GL_SetDefaultState();

    //     /*
    //     ** draw our stereo patterns
    //     */
    // #if 0  // commented out until H3D pays us the money they owe us
    //     GL_DrawStereoPattern();
    // #endif

    //     ref_gl_GL_InitImages();
    //     ref_gl_Mod_Init();
    //     ref_gl_R_InitParticleTexture();
    //     ref_gl_Draw_InitLocal();

    //     err = qglGetError();
    //     if ( err != GL_NO_ERROR )
    //         ri.Con_Printf( PRINT_ALL, "glGetError() = 0x%x\n", err );
}

/*
===============
R_Shutdown
===============
*/
void ref_gl_R_Shutdown( void )
{
    ri.Cmd_RemoveCommand( "modellist" );
    ri.Cmd_RemoveCommand( "screenshot" );
    ri.Cmd_RemoveCommand( "imagelist" );
    ri.Cmd_RemoveCommand( "gl_strings" );

    ref_gl_Mod_FreeAll();

    ref_gl_GL_ShutdownImages();

    /*
    ** shut down OS specific OpenGL stuff like contexts, etc.
    */
    GLimp_Shutdown();

    /*
    ** shutdown our QGL subsystem
    */
    QGL_Shutdown();
}

void ref_gl_R_BeginFrame( float camera_separation )
{
    gl_state.camera_separation = camera_separation;

    /*
    ** change modes if necessary
    */
    if ( gl_mode->modified || vid_fullscreen->modified )
    {  // FIXME: only restart if CDS is required
        cvar* ref;

        ref = ri.Cvar_Get( "vid_ref", "gl", 0 );
        ref->modified = true;
    }

    if ( gl_log->modified )
    {
        GLimp_EnableLogging( gl_log->value );
        gl_log->modified = false;
    }

    if ( gl_log->value )
    {
        GLimp_LogNewFrame();
    }

    /*
    ** update 3Dfx gamma -- it is expected that a user will do a vid_restart
    ** after tweaking this value
    */
    if ( vid_gamma->modified )
    {
        vid_gamma->modified = false;

        if ( gl_config.renderer & ( GL_RENDERER_VOODOO ) )
        {
            char envbuffer[1024];
            float g;

            g = 2.00 * ( 0.8 - ( vid_gamma->value - 0.5 ) ) + 1.0F;
            // Com_sprintf( envbuffer, sizeof(envbuffer), "SSTV2_GAMMA=%f", g );
            putenv( envbuffer );
            // Com_sprintf( envbuffer, sizeof(envbuffer), "SST_GAMMA=%f", g );
            putenv( envbuffer );
        }
    }

    GLimp_BeginFrame( camera_separation );

    /*
    ** go into 2D mode
    */
    qglViewport( 0, 0, quake2::getInstance()->vid.width,
                 quake2::getInstance()->vid.height );
    qglMatrixMode( GL_PROJECTION );
    qglLoadIdentity();
    qglOrtho( 0, quake2::getInstance()->vid.width,
              quake2::getInstance()->vid.height, 0, -99999, 99999 );
    qglMatrixMode( GL_MODELVIEW );
    qglLoadIdentity();
    qglDisable( GL_DEPTH_TEST );
    qglDisable( GL_CULL_FACE );
    qglDisable( GL_BLEND );
    qglEnable( GL_ALPHA_TEST );
    qglColor4f( 1, 1, 1, 1 );

    /*
    ** draw buffer stuff
    */
    if ( gl_drawbuffer->modified )
    {
        gl_drawbuffer->modified = false;

        if ( gl_state.camera_separation == 0 || !gl_state.stereo_enabled )
        {
            if ( gl_drawbuffer->string == "GL_FRONT" )
                qglDrawBuffer( GL_FRONT );
            else
                qglDrawBuffer( GL_BACK );
        }
    }

    /*
    ** texturemode stuff
    */
    if ( gl_texturemode->modified )
    {
        ref_gl_GL_TextureMode( toChar( gl_texturemode->string ) );
        gl_texturemode->modified = false;
    }

    if ( gl_texturealphamode->modified )
    {
        ref_gl_GL_TextureAlphaMode( toChar( gl_texturealphamode->string ) );
        gl_texturealphamode->modified = false;
    }

    if ( gl_texturesolidmode->modified )
    {
        ref_gl_GL_TextureSolidMode( toChar( gl_texturesolidmode->string ) );
        gl_texturesolidmode->modified = false;
    }

    /*
    ** swapinterval stuff
    */
    GL_UpdateSwapInterval();

    //
    // clear screen if desired
    //
    ref_gl_R_Clear();
}

/*
=============
R_SetPalette
=============
*/
unsigned r_rawpalette[256];

void ref_gl_R_SetPalette( const unsigned char* palette )
{
    int i;

    byte* rp = (byte*)r_rawpalette;

    if ( palette )
    {
        for ( i = 0; i < 256; i++ )
        {
            rp[i * 4 + 0] = palette[i * 3 + 0];
            rp[i * 4 + 1] = palette[i * 3 + 1];
            rp[i * 4 + 2] = palette[i * 3 + 2];
            rp[i * 4 + 3] = 0xff;
        }
    }
    else
    {
        for ( i = 0; i < 256; i++ )
        {
            rp[i * 4 + 0] = d_8to24table[i] & 0xff;
            rp[i * 4 + 1] = ( d_8to24table[i] >> 8 ) & 0xff;
            rp[i * 4 + 2] = ( d_8to24table[i] >> 16 ) & 0xff;
            rp[i * 4 + 3] = 0xff;
        }
    }
    ref_gl_GL_SetTexturePalette( r_rawpalette );

    qglClearColor( 0, 0, 0, 0 );
    qglClear( GL_COLOR_BUFFER_BIT );
    qglClearColor( 1, 0, 0.5, 0.5 );
}

/*
** ref_gl_R_DrawBeam
*/
void ref_gl_R_DrawBeam( entity_s* e )
{
#define NUM_BEAM_SEGS 6

    int i;
    float r, g, b;

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

    for ( i = 0; i < 6; i++ )
    {
        RotatePointAroundVector( start_points[i], normalized_direction, perpvec,
                                 ( 360.0 / NUM_BEAM_SEGS ) * i );
        VectorAdd( start_points[i], origin, start_points[i] );
        VectorAdd( start_points[i], direction, end_points[i] );
    }

    qglDisable( GL_TEXTURE_2D );
    qglEnable( GL_BLEND );
    qglDepthMask( GL_FALSE );

    r = ( d_8to24table[e->skinnum & 0xFF] ) & 0xFF;
    g = ( d_8to24table[e->skinnum & 0xFF] >> 8 ) & 0xFF;
    b = ( d_8to24table[e->skinnum & 0xFF] >> 16 ) & 0xFF;

    r *= 1 / 255.0F;
    g *= 1 / 255.0F;
    b *= 1 / 255.0F;

    qglColor4f( r, g, b, e->alpha );

    qglBegin( GL_TRIANGLE_STRIP );
    for ( i = 0; i < NUM_BEAM_SEGS; i++ )
    {
        qglVertex3fv( start_points[i] );
        qglVertex3fv( end_points[i] );
        qglVertex3fv( start_points[( i + 1 ) % NUM_BEAM_SEGS] );
        qglVertex3fv( end_points[( i + 1 ) % NUM_BEAM_SEGS] );
    }
    qglEnd();

    qglEnable( GL_TEXTURE_2D );
    qglDisable( GL_BLEND );
    qglDepthMask( GL_TRUE );
}

//===================================================================

void ref_gl_R_BeginRegistration( char* map );
struct model_s* ref_gl_R_RegisterModel( char* name );
struct image_s* ref_gl_R_RegisterSkin( char* name );
void ref_gl_R_SetSky( char* name, float rotate, vec3_t axis );
void ref_gl_R_EndRegistration( void );

void ref_gl_R_RenderFrame( refdef_t* fd );

struct image_s* ref_gl_Draw_FindPic( char* name );

void ref_gl_Draw_Pic( int x, int y, char* name );
void ref_gl_Draw_Char( int x, int y, int c );
void ref_gl_Draw_TileClear( int x, int y, int w, int h, char* name );
void ref_gl_Draw_Fill( int x, int y, int w, int h, int c );
void ref_gl_Draw_FadeScreen( void );

void dupa() {}

/*
GetRefAPI
*/
refexport_t ref_gl_GetRefAPI( refimport_t rimp )
{
    refexport_t currentRe;

    ri = rimp;

    currentRe.api_version = API_VERSION;

    currentRe.BeginRegistration = ref_gl_R_BeginRegistration;
    currentRe.RegisterModel = ref_gl_R_RegisterModel;
    currentRe.RegisterSkin = ref_gl_R_RegisterSkin;
    currentRe.RegisterPic = ref_gl_Draw_FindPic;
    currentRe.SetSky = ref_gl_R_SetSky;
    currentRe.EndRegistration = ref_gl_R_EndRegistration;

    currentRe.RenderFrame = ref_gl_R_RenderFrame;

    currentRe.DrawGetPicSize = ref_gl_Draw_GetPicSize;
    currentRe.DrawPic = ref_gl_Draw_Pic;
    currentRe.DrawStretchPic = ref_gl_Draw_StretchPic;
    currentRe.DrawChar = ref_gl_Draw_Char;
    currentRe.DrawTileClear = ref_gl_Draw_TileClear;
    currentRe.DrawFill = ref_gl_Draw_Fill;
    currentRe.DrawFadeScreen = ref_gl_Draw_FadeScreen;

    currentRe.DrawStretchRaw = ref_gl_Draw_StretchRaw;

    currentRe.Init = ref_gl_R_Init;
    currentRe.Shutdown = ref_gl_R_Shutdown;

    currentRe.CinematicSetPalette = ref_gl_R_SetPalette;
    currentRe.BeginFrame = ref_gl_R_BeginFrame;
    currentRe.EndFrame = GLimp_EndFrame;

    currentRe.AppActivate = GLimp_AppActivate;

    Swap_Init();

    return currentRe;
}