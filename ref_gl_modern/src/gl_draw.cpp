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
#include "ref_gl/gl_local.h"
#include "ref_gl/gl_image.h"
#include "ref_gl/gl_main.h"
#include "ref_gl/gl_draw.h"
#include "ref_gl/texture.hpp"
#include "ref_gl/quad.hpp"
#include "ref_gl/utils.hpp"
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
	const float		   width	 = font_atlas_tex.get_width ();
	const float		   height	 = font_atlas_tex.get_height ();
	constexpr float	   char_size = 8.f;
	const float		   fraction	 = char_size / width;
	const float		   frow		 = row * fraction;
	const float		   fcol		 = col * fraction;

	const float x0				 = Q2::to_x_normalized (int_x);
	const float y0				 = Q2::to_y_normalized (int_y);
	const float x1				 = Q2::to_x_normalized (int_x + char_size);
	const float y1				 = Q2::to_y_normalized (int_y + char_size);

	const float Upx				 = width * fcol;
	const float Vpx				 = height * frow;
	const float Upx2			 = Upx / 0.0625;
	const float Vpx2			 = Vpx / 0.0625;

	const float size			 = char_size / width;

	static Q2::PosAndUV pau;
	pau.Data[0].U = fcol + size;
	pau.Data[0].V = frow;
	pau.Data[0].X = x1;
	pau.Data[0].Y = y0;

	pau.Data[1].U = fcol;
	pau.Data[1].V = frow + size;
	pau.Data[1].X = x0;
	pau.Data[1].Y = y1;

	pau.Data[2].U = fcol + size;
	pau.Data[2].V = frow + size;
	pau.Data[2].X = x1;
	pau.Data[2].Y = y1;

	pau.Data[3].U = fcol;
	pau.Data[3].V = frow;
	pau.Data[3].X = x0;
	pau.Data[3].Y = y0;

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

	Q2::Texture* texture = Q2::TextureStore::get_instance ().get_or_create (pic);
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
	//Q2::RenderData rd;

	//Q2::Texture *texture = Q2::TextureStore::get_instance ().get (pic);
	//if (texture == nullptr)
	//{
	//	texture = Q2::TextureStore::get_instance ().get_or_create (pic);
	//	rd.alphaTest   = (gl_config.renderer == GL_RENDERER_MCD) || (gl_config.renderer & GL_RENDERER_RENDITION);
	//}

	//texture->draw ();

	//if (
	//	((gl_config.renderer == GL_RENDERER_MCD) || (gl_config.renderer & GL_RENDERER_RENDITION)) && !image->has_alpha
	//)
	//{
	//	qglDisable (GL_ALPHA_TEST);
	//}

	//GL_BindTexture (image->texnum);
	//qglBegin (GL_QUADS);
	//qglTexCoord2f (x / 64.0, y / 64.0);
	//qglVertex2f (x, y);
	//qglTexCoord2f ((x + w) / 64.0, y / 64.0);
	//qglVertex2f (x + w, y);
	//qglTexCoord2f ((x + w) / 64.0, (y + h) / 64.0);
	//qglVertex2f (x + w, y + h);
	//qglTexCoord2f (x / 64.0, (y + h) / 64.0);
	//qglVertex2f (x, y + h);
	//qglEnd ();

	//if (((gl_config.renderer == GL_RENDERER_MCD) || (gl_config.renderer & GL_RENDERER_RENDITION)) && !image->has_alpha)
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
	qglEnable (GL_BLEND);
	qglDisable (GL_TEXTURE_2D);
	qglColor4f (0, 0, 0, 0.8);
	qglBegin (GL_QUADS);

	qglVertex2f (0, 0);
	qglVertex2f (vid.width, 0);
	qglVertex2f (vid.width, vid.height);
	qglVertex2f (0, vid.height);

	qglEnd ();
	qglColor4f (1, 1, 1, 1);
	qglEnable (GL_TEXTURE_2D);
	qglDisable (GL_BLEND);
}

//====================================================================

/*
=============
Draw_StretchRaw
=============
*/

// TODO: Convert to modern
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

	rd.w		 = w;
	rd.h		 = h;
	rd.scale	 = t;

	static Q2::Texture stretchTexture ("Cinematic");
	stretchTexture.changeScale (t);
	stretchTexture.draw (rd);
}
