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
/*
** GLW_IMP.C
**
** This file contains ALL Win32 specific stuff having to do with the
** OpenGL refresh.  When a port is being made the following functions
** must be implemented by the port:
**
** GLimp_EndFrame
** GLimp_Init
** GLimp_Shutdown
**
*/

#include "quake2/renderer/gl_modern/gl_local.h"
#include "quake2/renderer/gl_modern/gl_state.hpp"
#include "quake2/renderer/gl_modern/frame_buffer.hpp"
#include "quake2/renderer/gl_modern/gl_rmain.h"
#include "quake2/renderer/gl_modern/renderer.hpp"

#include "shared/assert.h"

#ifdef _WIN32
#include <assert.h>
#include <windows.h>

#include "quake2/renderer/gl_modern/glw_win.h"
#include <string>
// ToDo: do we really need to have this here?
#include "../../quake2/inc/quake2/windows/winquake.h"
#include "../../quake2/inc/quake2/video/VideoSettings.h"

glwstate_t glw_state;

extern cvar_t  *vid_fullscreen;
extern cvar_t  *vid_ref;
EXTERNC cvar_t *gl_allow_software;
extern Q2::Framebuffer g_framebuffer;

qboolean GLimp_InitGL (void);

static qboolean VerifyDriver (void)
{
	return e_true;
}

/*
** GLimp_SetMode
*/
rserr_t GLimp_SetMode (int *pwidth, int *pheight, int mode, qboolean fullscreen)
{
	int			width, height;
	const char *win_fs[] = {"W", "FS"};

	ri.Con_Printf (PRINT_ALL, "Initializing OpenGL display\n");

	ri.Con_Printf (PRINT_ALL, "...setting mode %d:", mode);

	if (!ri.Vid_GetModeInfo (&width, &height, mode))
	{
		ri.Con_Printf (PRINT_ALL, " invalid mode\n");
		return rserr_invalid_mode;
	}

	ri.Con_Printf (PRINT_ALL, " %d %d %s\n", width, height, win_fs[fullscreen]);

	// do a CDS if needed
	if (fullscreen)
	{
		VideoSettings vs;

		ri.Con_Printf (PRINT_ALL, "...attempting fullscreen\n");

		vs.dmPelsWidth	= width;
		vs.dmPelsHeight = height;
		vs.dmFields		= DM_PELSWIDTH | DM_PELSHEIGHT;

		if (gl_bitdepth->value != 0)
		{
			vs.dmBitsPerPel = gl_bitdepth->value;
			vs.dmFields |= DM_BITSPERPEL;
			ri.Con_Printf (PRINT_ALL, "...using gl_bitdepth of %d\n", (int) gl_bitdepth->value);
		}
		else
		{
			HDC hdc		  = GetDC (NULL);
			int bitspixel = GetDeviceCaps (hdc, BITSPIXEL);

			ri.Con_Printf (PRINT_ALL, "...using desktop display depth of %d\n", bitspixel);

			ReleaseDC (0, hdc);
		}

		Q2_Assert (false, "TODO: implement");

		ri.Con_Printf (PRINT_ALL, "...calling CDS: ");
		// if (ChangeDisplaySettings (&dm, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL)
		{
			*pwidth									  = width;
			*pheight								  = height;

			Q2::glstate_t::get_instance ().fullscreen = e_true;

			ri.Con_Printf (PRINT_ALL, "ok\n");

			if (!ri.create_window (0, 0, width, height, e_true))
			{
				return rserr_invalid_mode;
			}

			return rserr_ok;
		}
		// else
		{
			*pwidth	 = width;
			*pheight = height;

			ri.Con_Printf (PRINT_ALL, "failed\n");

			ri.Con_Printf (PRINT_ALL, "...calling CDS assuming dual monitors:");

			// dm.dmPelsWidth	= width * 2;
			// dm.dmPelsHeight = height;
			// dm.dmFields		= DM_PELSWIDTH | DM_PELSHEIGHT;

			if (gl_bitdepth->value != 0)
			{
				// dm.dmBitsPerPel = gl_bitdepth->value;
				// dm.dmFields |= DM_BITSPERPEL;
			}

			/*
			** our first CDS failed, so maybe we're running on some weird dual monitor
			** system
			*/
			// if (ChangeDisplaySettings (&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			{
				ri.Con_Printf (PRINT_ALL, " failed\n");

				ri.Con_Printf (PRINT_ALL, "...setting windowed mode\n");

				ChangeDisplaySettings (0, 0);

				*pwidth									  = width;
				*pheight								  = height;
				Q2::glstate_t::get_instance ().fullscreen = e_false;

				if (!ri.create_window (0, 0, width, height, e_false))
					return rserr_invalid_mode;
				return rserr_invalid_fullscreen;
			}
			// else
			{
				ri.Con_Printf (PRINT_ALL, " ok\n");
				if (!ri.create_window (0, 0, width, height, e_true))
					return rserr_invalid_mode;

				Q2::glstate_t::get_instance ().fullscreen = e_true;
				return rserr_ok;
			}
		}
	}
	else
	{
		ri.Con_Printf (PRINT_ALL, "...setting windowed mode\n");

		// ChangeDisplaySettings (0, 0);

		*pwidth									  = width;
		*pheight								  = height;
		Q2::glstate_t::get_instance ().fullscreen = e_false;
		if (ri.create_window (512, 64, width, height, e_false))
		{
			if (GLimp_InitGL () == e_false)
			{
				return rserr_invalid_mode;
			}
			InitGlew ();
			ri.Vid_NewWindow (width, height);
		}
		else
		{
			return rserr_invalid_mode;
		}
	}

	return rserr_ok;
}

/*
** GLimp_Shutdown
**
** This routine does all OS specific shutdown procedures for the OpenGL
** subsystem.  Under OpenGL this means NULLing out the current DC and
** HGLRC, deleting the rendering context, and releasing the DC acquired
** for the window.  The state structure is also nulled out.
**
*/
void GLimp_Shutdown (void)
{
	if (qwglMakeCurrent && !qwglMakeCurrent (NULL, NULL))
		ri.Con_Printf (PRINT_ALL, "ref_gl::R_Shutdown() - wglMakeCurrent failed\n");

	if (glw_state.log_fp)
	{
		fclose (glw_state.log_fp);
		glw_state.log_fp = 0;
	}
}

/*
** GLimp_Init
**
** This routine is responsible for initializing the OS specific portions
** of OpenGL.  Under Win32 this means dealing with the pixelformats and
** doing the wgl interface stuff.
*/
qboolean GLimp_Init (void)
{
#define OSR2_BUILD_NUMBER 1111
	return e_true;
}

qboolean GLimp_InitGL (void)
{
	if (!VerifyDriver ())
	{
		ri.Con_Printf (PRINT_ALL, "GLimp_Init() - no hardware acceleration detected\n");
		return e_false;
	}

	return e_true;
}

/*
** GLimp_BeginFrame
*/
EXTERNC void GLimp_BeginFrame (void)
{
	qglDrawBuffer (GL_BACK);
}

/*
** GLimp_EndFrame
**
** Responsible for doing a swapbuffers and possibly for other stuff
** as yet to be determined.  Probably better not to make this a GLimp
** function and instead do a call to GLimp_SwapBuffers.
*/
void GLimp_EndFrame (void)
{
	gl_modern::Renderer::get_instance ().end_frame ();
}

/*
** GLimp_AppActivate
*/
void GLimp_AppActivate (qboolean active)
{
	auto x = 0;
}

#endif	// _WIN32
