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

// draw.c

#include <GL/glew.h>
#include "gl_modern/gl_local.h"
#include "gl_modern/gl_image.h"
#include "gl_modern/gl_main.h"
#include "gl_modern/gl_draw.h"
#include "gl_modern/texture.hpp"
#include "gl_modern/quad.hpp"
#include "gl_modern/utils.hpp"
#include <cmath>

image_t *draw_chars;

/*
===============
Draw_InitLocal
===============
*/
void Draw_InitLocal (void)
{
	// load console characters (don't bilerp characters)
	draw_chars = GL_FindImage ("pics/conchars.pcx", it_pic);
	if (draw_chars)
	{
		GL_BindTexture (draw_chars->texnum);
		qglTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		qglTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
}

/*
================
Draw_Char

Draws one 8*8 graphics character with 0 being transparent.
It can be clipped to the top of the screen to allow the console to be
smoothly scrolled off.
================
*/
void Draw_Char (int int_x, int int_y, int num)
{
	num &= 255;

	if ((num & 127) == 32)
		return;	 // space

	if (int_y <= -8)
		return;	 // totally off screen

	const float row = num >> 4;
	const float col = num & 15;

	static Q2::Texture font_atlas_tex (draw_chars, "pics/conchars.pcx");
	constexpr float	   char_size = 8.f;
	constexpr float	   size		 = 0.0625f;	 // 16x16 cells in 128x128 atlas
	const float		   frow		 = row * size;
	const float		   fcol		 = col * size;

	const float x0 = Q2::to_x_normalized (static_cast<float> (int_x));
	const float y0 = Q2::to_y_normalized (static_cast<float> (int_y));
	const float x1 = Q2::to_x_normalized (static_cast<float> (int_x) + char_size);
	const float y1 = Q2::to_y_normalized (static_cast<float> (int_y) + char_size);

	// Classic Quake quad order: TL, TR, BR, BL — mapped into Texture EBO slots
	// EBO: 0 TR, 1 BL, 2 BR, 3 TL
	static Q2::PosAndUV pau;
	pau.Data[3].X = x0;
	pau.Data[3].Y = y0;
	pau.Data[3].U = fcol;
	pau.Data[3].V = frow;			 // TL

	pau.Data[0].X = x1;
	pau.Data[0].Y = y0;
	pau.Data[0].U = fcol + size;
	pau.Data[0].V = frow;			 // TR

	pau.Data[2].X = x1;
	pau.Data[2].Y = y1;
	pau.Data[2].U = fcol + size;
	pau.Data[2].V = frow + size;	 // BR

	pau.Data[1].X = x0;
	pau.Data[1].Y = y1;
	pau.Data[1].U = fcol;
	pau.Data[1].V = frow + size;	 // BL

	font_atlas_tex.draw (pau);
}

/*
=============
Draw_FindPic
=============
*/
image_t *Draw_FindPic (const char *name)
{
	image_t *gl;
	char	 fullname[MAX_QPATH];

	if (name[0] != '/' && name[0] != '\\')
	{
		Com_sprintf (fullname, sizeof (fullname), "pics/%s.pcx", name);
		gl = GL_FindImage (fullname, it_pic);
	}
	else
		gl = GL_FindImage (name + 1, it_pic);

	return gl;
}

/*
=============
Draw_GetPicSize
=============
*/
void Draw_GetPicSize (int *w, int *h, char *pic)
{
	image_t *gl;

	gl = Draw_FindPic (pic);
	if (!gl)
	{
		*w = *h = -1;
		return;
	}
	*w = gl->width;
	*h = gl->height;
}

/*
=============
Draw_StretchPic
=============
*/
void Draw_StretchPic (int x, int y, int w, int h, char *pic)
{
	if (scrap_dirty)
	{
		Scrap_Upload ();
	}

	Q2::Texture *texture = Q2::TextureStore::get_instance ().get_or_create (pic);
	texture->fetch_uv_and_apply_them ();
	texture->set_pos_global (x, y);
	texture->set_size (w, h);
	texture->draw ();
}

/*
=============
Draw_Pic
=============
*/
void Draw_Pic (int x, int y, char *pic)
{
	if (scrap_dirty)
		Scrap_Upload ();

	Q2::Texture *texture = Q2::TextureStore::get_instance ().get_or_create (pic);
	texture->fetch_uv_and_apply_them ();
	texture->set_pos_global (x, y);
	texture->draw ();
}

/*
=============
Draw_TileClear

This repeats a 64*64 tile graphic to fill the screen around a sized down
refresh window.
=============
*/
void Draw_TileClear (int x, int y, int w, int h, char *pic)
{
	// Q2::RenderData rd;

	// Q2::Texture *texture = Q2::TextureStore::get_instance ().get (pic);
	// if (texture == nullptr)
	//{
	//	texture = Q2::TextureStore::get_instance ().get_or_create (pic);
	//	rd.alphaTest   = (gl_config.renderer == GL_RENDERER_MCD) || (gl_config.renderer & GL_RENDERER_RENDITION);
	// }

	// texture->draw ();

	// if (
	//	((gl_config.renderer == GL_RENDERER_MCD) || (gl_config.renderer & GL_RENDERER_RENDITION)) && !image->has_alpha
	//)
	//{
	//	qglDisable (GL_ALPHA_TEST);
	// }

	// GL_BindTexture (image->texnum);
	// qglBegin (GL_QUADS);
	// qglTexCoord2f (x / 64.0, y / 64.0);
	// qglVertex2f (x, y);
	// qglTexCoord2f ((x + w) / 64.0, y / 64.0);
	// qglVertex2f (x + w, y);
	// qglTexCoord2f ((x + w) / 64.0, (y + h) / 64.0);
	// qglVertex2f (x + w, y + h);
	// qglTexCoord2f (x / 64.0, (y + h) / 64.0);
	// qglVertex2f (x, y + h);
	// qglEnd ();

	// if (((gl_config.renderer == GL_RENDERER_MCD) || (gl_config.renderer & GL_RENDERER_RENDITION)) && !image->has_alpha)
	//	qglEnable (GL_ALPHA_TEST);
}

/*
=============
Draw_Fill

Fills a box of pixels with a single color
=============
*/
void Draw_Fill (int x, int y, int w, int h, int c)
{
	return;

	union
	{
		unsigned c;
		byte	 v[4];
	} color;

	if ((unsigned) c > 255)
		ri.Sys_Error (ERR_FATAL, "Draw_Fill: bad color");

	color.c = d_8to24table[c];

	static Q2::DrawData dd;
	static Q2::Quad		fillQuad ("fill_quad");

	dd.color.r = color.v[0] / 255.0f;
	dd.color.g = color.v[1] / 255.0f;
	dd.color.b = color.v[2] / 255.0f;
	dd.pos.x   = x;
	dd.pos.y   = y;
	dd.pos.z   = 0.f;
	dd.size.w  = w;
	dd.size.h  = h;

	fillQuad.draw (dd);
}

//=============================================================================

/*
================
Draw_FadeScreen

================
*/
void Draw_FadeScreen (void)
{
	static Q2::Quad fade ("fade_screen");
	Q2::DrawData	dd;

	dd.color.r = 0.f;
	dd.color.g = 0.f;
	dd.color.b = 0.f;
	dd.color.a = 0.8f;
	dd.pos.x   = -1.f;
	dd.pos.y   = -1.f;
	dd.size.w  = 2.f;
	dd.size.h  = 2.f;

	glDisable (GL_DEPTH_TEST);
	glDisable (GL_CULL_FACE);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	fade.draw (dd);
	glDisable (GL_BLEND);
}

//====================================================================

/*
=============
Draw_StretchRaw
=============
*/

void Draw_StretchRaw (int x, int y, int w, int h, int cols, int rows, byte *data)
{
	unsigned	  image32[256 * 256];
	unsigned char image8[256 * 256];
	int			  i, j, trows;
	byte		 *source;
	int			  frac, fracstep;
	float		  hscale;
	int			  row;

	GL_BindTexture (0);
	Q2::RenderData rd;
	rd.imgW = 256;
	rd.imgH = 256;

	if (rows <= 256)
	{
		hscale = 1;
		trows  = rows;
	}
	else
	{
		hscale = rows / 256.0;
		trows  = 256;
	}
	float t = rows * hscale / 256;

	if (!qglColorTableEXT)
	{
		unsigned *dest;

		for (i = 0; i < trows; i++)
		{
			row = (int) (i * hscale);
			if (row > rows)
				break;
			source	 = data + cols * row;
			dest	 = &image32[i * 256];
			fracstep = cols * 0x10000 / 256;
			frac	 = fracstep >> 1;
			for (j = 0; j < 256; j++)
			{
				dest[j] = r_rawpalette[source[frac >> 16]];
				frac += fracstep;
			}
		}

		rd.data_type	   = GL_UNSIGNED_BYTE;
		rd.format		   = GL_RGBA;
		rd.internal_format = GL_RGBA8;
		rd.data			   = &image32;
	}
	else
	{
		unsigned char *dest;

		for (i = 0; i < trows; i++)
		{
			row = (int) (i * hscale);
			if (row > rows)
				break;
			source	 = data + cols * row;
			dest	 = &image8[i * 256];
			fracstep = cols * 0x10000 / 256;
			frac	 = fracstep >> 1;
			for (j = 0; j < 256; j++)
			{
				dest[j] = source[frac >> 16];
				frac += fracstep;
			}
		}

		rd.data_type	   = GL_UNSIGNED_BYTE;
		rd.format		   = GL_COLOR_INDEX;
		rd.internal_format = GL_COLOR_INDEX8_EXT;
		rd.data			   = &image8;
	}

	rd.w	 = w;
	rd.h	 = h;
	rd.scale = t;

	static Q2::Texture stretchTexture ("Cinematic");
	stretchTexture.changeScale (t);
	stretchTexture.draw (rd);
}
