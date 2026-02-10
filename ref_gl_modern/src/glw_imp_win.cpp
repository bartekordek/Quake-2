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
#include "ref_gl/gl_local.h"
#include "shared/assert.h"

#ifdef _WIN32
#include <assert.h>
#include <windows.h>

#include "ref_gl/glw_win.h"
#include "ref_gl/glw.h"
//ToDo: do we really need to have this here?
#include "../../quake2/inc/quake2/windows/winquake.h"
#include "../../quake2/inc/quake2/video/VideoSettings.h"

glwstate_t glw_state;

extern cvar_t *vid_fullscreen;
extern cvar_t *vid_ref;
EXTERNC cvar_t *gl_allow_software;

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
		//if (ChangeDisplaySettings (&dm, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL)
		{
			*pwidth				= width;
			*pheight			= height;

			gl_state.fullscreen = e_true;

			ri.Con_Printf (PRINT_ALL, "ok\n");

			if (!ri.create_window(0, 0, width, height, e_true))
			{
				return rserr_invalid_mode;
			}

			return rserr_ok;
		}
		//else
		{
			*pwidth	 = width;
			*pheight = height;

			ri.Con_Printf (PRINT_ALL, "failed\n");

			ri.Con_Printf (PRINT_ALL, "...calling CDS assuming dual monitors:");

			//dm.dmPelsWidth	= width * 2;
			//dm.dmPelsHeight = height;
			//dm.dmFields		= DM_PELSWIDTH | DM_PELSHEIGHT;

			if (gl_bitdepth->value != 0)
			{
				//dm.dmBitsPerPel = gl_bitdepth->value;
				//dm.dmFields |= DM_BITSPERPEL;
			}

			/*
			** our first CDS failed, so maybe we're running on some weird dual monitor
			** system
			*/
			//if (ChangeDisplaySettings (&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			{
				ri.Con_Printf (PRINT_ALL, " failed\n");

				ri.Con_Printf (PRINT_ALL, "...setting windowed mode\n");

				ChangeDisplaySettings (0, 0);

				*pwidth				= width;
				*pheight			= height;
				gl_state.fullscreen = e_false;

				if (!ri.create_window (0, 0, width, height, e_false))
					return rserr_invalid_mode;
				return rserr_invalid_fullscreen;
			}
			//else
			{
				ri.Con_Printf (PRINT_ALL, " ok\n");
				if (!ri.create_window (0, 0, width, height, e_true))
					return rserr_invalid_mode;

				gl_state.fullscreen = e_true;
				return rserr_ok;
			}
		}
	}
	else
	{
		ri.Con_Printf (PRINT_ALL, "...setting windowed mode\n");

		//ChangeDisplaySettings (0, 0);

		*pwidth				= width;
		*pheight			= height;
		gl_state.fullscreen = e_false;
		if (ri.create_window(512, 0, width, height, e_false))
		{
			if (GLimp_InitGL() == e_false)
			{
				return rserr_invalid_mode;
			}
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
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof (PIXELFORMATDESCRIPTOR),	 // size of this pfd
		1,								 // version number
		PFD_DRAW_TO_WINDOW |			 // support window
			PFD_SUPPORT_OPENGL |		 // support OpenGL
			PFD_DOUBLEBUFFER,			 // double buffered
		PFD_TYPE_RGBA,					 // RGBA type
		24,								 // 24-bit color depth
		0,
		0,
		0,
		0,
		0,
		0,	// color bits ignored
		0,	// no alpha buffer
		0,	// shift bit ignored
		0,	// no accumulation buffer
		0,
		0,
		0,
		0,				 // accum bits ignored
		32,				 // 32-bit z-buffer
		0,				 // no stencil buffer
		0,				 // no auxiliary buffer
		PFD_MAIN_PLANE,	 // main layer
		0,				 // reserved
		0,
		0,
		0  // layer masks ignored
	};
	int		pixelformat;
	cvar_t *stereo;

	stereo = ri.Cvar_Get ("cl_stereo", "0", 0);

	/*
	** set PFD_STEREO if necessary
	*/
	if (stereo->value != 0)
	{
		ri.Con_Printf (PRINT_ALL, "...attempting to use stereo\n");
		pfd.dwFlags |= PFD_STEREO;
		gl_state.stereo_enabled = e_true;
	}
	else
	{
		gl_state.stereo_enabled = e_false;
	}


	/*
	** report if stereo is desired but unavailable
	*/
	if (!(pfd.dwFlags & PFD_STEREO) && (stereo->value != 0))
	{
		ri.Con_Printf (PRINT_ALL, "...failed to select stereo pixel format\n");
		ri.Cvar_SetValue ("cl_stereo", 0);
		gl_state.stereo_enabled = e_false;
	}

	/*
	** startup the OpenGL subsystem by creating a context and making
	** it current
	*/
	//if ((glw_state.hGLRC = qwglCreateContext (glw_state.hDC)) == 0)
	//{
	//	ri.Con_Printf (PRINT_ALL, "GLimp_Init() - qwglCreateContext failed\n");

	//	goto fail;
	//}

	//if (!qwglMakeCurrent (glw_state.hDC, glw_state.hGLRC))
	//{
	//	ri.Con_Printf (PRINT_ALL, "GLimp_Init() - qwglMakeCurrent failed\n");

	//	goto fail;
	//}

	if (!VerifyDriver ())
	{
		ri.Con_Printf (PRINT_ALL, "GLimp_Init() - no hardware acceleration detected\n");
		goto fail;
	}

	/*
	** print out PFD specifics
	*/
	ri.Con_Printf (PRINT_ALL, "GL PFD: color(%d-bits) Z(%d-bit)\n", (int) pfd.cColorBits, (int) pfd.cDepthBits);

	return e_true;

fail:
	return e_false;
}

/*
** GLimp_BeginFrame
*/
EXTERNC void GLimp_BeginFrame (float camera_separation)
{
	if (camera_separation < 0 && gl_state.stereo_enabled)
	{
		qglDrawBuffer (GL_BACK_LEFT);
	}
	else if (camera_separation > 0 && gl_state.stereo_enabled)
	{
		qglDrawBuffer (GL_BACK_RIGHT);
	}
	else
	{
		qglDrawBuffer (GL_BACK);
	}
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
	
	int err;

	err = qglGetError ();
	assert (err == GL_NO_ERROR);

	if (stricmp (gl_drawbuffer->string, "GL_BACK") == 0)
	{
		ri.Swap_buffers ();
		//if (!qwglSwapBuffers (glw_state.hDC))
		//	ri.Sys_Error (ERR_FATAL, "GLimp_EndFrame() - SwapBuffers() failed!\n");
	}
	//ri.Swap_buffers ();
}


/*
** GLimp_AppActivate
*/
void GLimp_AppActivate (qboolean active)
{
}

#endif	// _WIN32