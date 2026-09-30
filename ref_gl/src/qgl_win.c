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
** QGL_WIN.C
**
** This file implements the operating system binding of GL to QGL function
** pointers.  When doing a port of Quake2 you must implement the following
** two functions:
**
** QGL_Init() - loads libraries, assigns function pointers, etc.
** QGL_Shutdown() - unloads libraries, NULLs function pointers
*/
#include <float.h>
#include "ref_gl/gl_local.h"
#include "ref_gl/glw_win.h"

void (APIENTRY *qglBindTexture) (GLenum target, GLuint texture);
void (APIENTRY *qglBlendFunc) (GLenum sfactor, GLenum dfactor);
void (APIENTRY *qglClear) (GLbitfield mask);
void (APIENTRY *qglClearColor) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void (APIENTRY *qglColor3f) (GLfloat red, GLfloat green, GLfloat blue);
void (APIENTRY *qglColor3fv) (const GLfloat *v);
void (APIENTRY *qglColor4f) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void (APIENTRY *qglColor4fv) (const GLfloat *v);
void (APIENTRY *qglColor4ub) (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
void (APIENTRY *qglColor4ubv) (const GLubyte *v);
void (APIENTRY *qglColorPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void (APIENTRY *qglCullFace) (GLenum mode);
void (APIENTRY *qglDeleteTextures) (GLsizei n, const GLuint *textures);
void (APIENTRY *qglDepthFunc) (GLenum func);
void (APIENTRY *qglDepthMask) (GLboolean flag);
void (APIENTRY *qglDepthRange) (GLclampd zNear, GLclampd zFar);
void (APIENTRY *qglDisable) (GLenum cap);
void (APIENTRY *qglDrawBuffer) (GLenum mode);
void (APIENTRY *qglEnable) (GLenum cap);
void (APIENTRY *qglEnableClientState) (GLenum array);
void (APIENTRY *qglEnd) (void);
void (APIENTRY *qglFinish) (void);
void (APIENTRY *qglFrustum) (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
GLenum (APIENTRY *qglGetError) (void);
void (APIENTRY *qglGetFloatv) (GLenum pname, GLfloat *params);
const GLubyte *(APIENTRY *qglGetString) (GLenum name);
void (APIENTRY *qglLoadIdentity) (void);
void (APIENTRY *qglLoadMatrixf) (const GLfloat *m);
void (APIENTRY *qglMatrixMode) (GLenum mode);
void (APIENTRY *qglOrtho) (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
void (APIENTRY *qglPointSize) (GLfloat size);
void (APIENTRY *qglPolygonMode) (GLenum face, GLenum mode);
void (APIENTRY *qglPopMatrix) (void);
void (APIENTRY *qglPushMatrix) (void);
void (APIENTRY *qglReadPixels) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
void (APIENTRY *qglRotatef) (GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *qglScalef) (GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *qglScissor) (GLint x, GLint y, GLsizei width, GLsizei height);
void (APIENTRY *qglShadeModel) (GLenum mode);
void (APIENTRY *qglTexCoord2f) (GLfloat s, GLfloat t);
void (APIENTRY *qglTexEnvf) (GLenum target, GLenum pname, GLfloat param);
void (APIENTRY *qglTexImage2D) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border,
								GLenum format, GLenum type, const GLvoid *pixels);
void (APIENTRY *qglTexParameterf) (GLenum target, GLenum pname, GLfloat param);
void (APIENTRY *qglTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format,
								   GLenum type, const GLvoid *pixels);
void (APIENTRY *qglTranslatef) (GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *qglVertex2f) (GLfloat x, GLfloat y);
void (APIENTRY *qglVertex3f) (GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *qglVertex3fv) (const GLfloat *v);
void (APIENTRY *qglVertexPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void (APIENTRY *qglViewport) (GLint x, GLint y, GLsizei width, GLsizei height);

void (APIENTRY *qglLockArraysEXT) (int, int);
void (APIENTRY *qglUnlockArraysEXT) (void);

BOOL (WINAPI *qwglSwapIntervalEXT) (int interval);
void (APIENTRY *qglPointParameterfEXT) (GLenum param, GLfloat value);
void (APIENTRY *qglPointParameterfvEXT) (GLenum param, const GLfloat *value);
void (APIENTRY *qglColorTableEXT) (int, int, int, int, int, const void *);
void (APIENTRY *qglSelectTextureSGIS) (GLenum);
void (APIENTRY *qglMTexCoord2fSGIS) (GLenum, GLfloat, GLfloat);

static void (APIENTRY *dllAccum) (GLenum op, GLfloat value);
static void (APIENTRY *dllAlphaFunc) (GLenum func, GLclampf ref);
static void (APIENTRY *dllArrayElement) (GLint i);
static void (APIENTRY *dllBegin) (GLenum mode);
static void (APIENTRY *dllBindTexture) (GLenum target, GLuint texture);
static void (APIENTRY *dllBlendFunc) (GLenum sfactor, GLenum dfactor);
static void (APIENTRY *dllClear) (GLbitfield mask);
static void (APIENTRY *dllClearColor) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
static void (APIENTRY *dllColor3f) (GLfloat red, GLfloat green, GLfloat blue);
static void (APIENTRY *dllColor3fv) (const GLfloat *v);
static void (APIENTRY *dllColor4f) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
static void (APIENTRY *dllColor4fv) (const GLfloat *v);
static void (APIENTRY *dllColor4ub) (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
static void (APIENTRY *dllColor4ubv) (const GLubyte *v);
static void (APIENTRY *dllColorPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
static void (APIENTRY *dllCullFace) (GLenum mode);
static void (APIENTRY *dllDeleteTextures) (GLsizei n, const GLuint *textures);
static void (APIENTRY *dllDepthFunc) (GLenum func);
static void (APIENTRY *dllDepthMask) (GLboolean flag);
static void (APIENTRY *dllDepthRange) (GLclampd zNear, GLclampd zFar);
static void (APIENTRY *dllDisable) (GLenum cap);
static void (APIENTRY *dllDrawBuffer) (GLenum mode);
static void (APIENTRY *dllEnable) (GLenum cap);
static void (APIENTRY *dllEnableClientState) (GLenum array);
static void (APIENTRY *dllEnd) (void);
static void (APIENTRY *dllFinish) (void);
static void (APIENTRY *dllFrustum) (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
GLenum (APIENTRY *dllGetError) (void);
static void (APIENTRY *dllGetFloatv) (GLenum pname, GLfloat *params);
const GLubyte *(APIENTRY *dllGetString) (GLenum name);
static void (APIENTRY *dllLoadIdentity) (void);
static void (APIENTRY *dllLoadMatrixf) (const GLfloat *m);
static void (APIENTRY *dllMatrixMode) (GLenum mode);
static void (APIENTRY *dllOrtho) (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
static void (APIENTRY *dllPointSize) (GLfloat size);
static void (APIENTRY *dllPolygonMode) (GLenum face, GLenum mode);
static void (APIENTRY *dllPopMatrix) (void);
static void (APIENTRY *dllPushMatrix) (void);
static void (APIENTRY *dllReadPixels) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
static void (APIENTRY *dllRotatef) (GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
static void (APIENTRY *dllScalef) (GLfloat x, GLfloat y, GLfloat z);
static void (APIENTRY *dllScissor) (GLint x, GLint y, GLsizei width, GLsizei height);
static void (APIENTRY *dllShadeModel) (GLenum mode);
static void (APIENTRY *dllTexCoord2f) (GLfloat s, GLfloat t);
static void (APIENTRY *dllTexEnvf) (GLenum target, GLenum pname, GLfloat param);
static void (APIENTRY *dllTexImage2D) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border,
									   GLenum format, GLenum type, const GLvoid *pixels);
static void (APIENTRY *dllTexParameterf) (GLenum target, GLenum pname, GLfloat param);
static void (APIENTRY *dllTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height,
										  GLenum format, GLenum type, const GLvoid *pixels);
static void (APIENTRY *dllTranslatef) (GLfloat x, GLfloat y, GLfloat z);
static void (APIENTRY *dllVertex2f) (GLfloat x, GLfloat y);
static void (APIENTRY *dllVertex3f) (GLfloat x, GLfloat y, GLfloat z);
static void (APIENTRY *dllVertex3fv) (const GLfloat *v);
static void (APIENTRY *dllVertexPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
static void (APIENTRY *dllViewport) (GLint x, GLint y, GLsizei width, GLsizei height);

/*
** QGL_Shutdown
**
** Unloads the specified DLL then nulls out all the proc pointers.
*/
void QGL_Shutdown (void)
{
	qglBindTexture		 = NULL;
	qglBlendFunc		 = NULL;
	qglClear			 = NULL;
	qglClearColor		 = NULL;
	qglColor3f			 = NULL;
	qglColor3fv			 = NULL;
	qglColor4f			 = NULL;
	qglColor4fv			 = NULL;
	qglColor4ub			 = NULL;
	qglColor4ubv		 = NULL;
	qglColorPointer		 = NULL;
	qglCullFace			 = NULL;
	qglDeleteTextures	 = NULL;
	qglDepthFunc		 = NULL;
	qglDepthMask		 = NULL;
	qglDepthRange		 = NULL;
	qglDisable			 = NULL;
	qglDrawBuffer		 = NULL;
	qglEnable			 = NULL;
	qglEnableClientState = NULL;
	qglEnd				 = NULL;
	qglFinish			 = NULL;
	qglFrustum			 = NULL;
	qglGetError			 = NULL;
	qglGetFloatv		 = NULL;
	qglGetString		 = NULL;
	qglLoadIdentity		 = NULL;
	qglLoadMatrixf		 = NULL;
	qglMatrixMode		 = NULL;
	qglOrtho			 = NULL;
	qglPointSize		 = NULL;
	qglPolygonMode		 = NULL;
	qglPopMatrix		 = NULL;
	qglPushMatrix		 = NULL;
	qglReadPixels		 = NULL;
	qglRotatef			 = NULL;
	qglScalef			 = NULL;
	qglScissor			 = NULL;
	qglShadeModel		 = NULL;
	qglTexCoord2f		 = NULL;
	qglTexEnvf			 = NULL;
	qglTexImage2D		 = NULL;
	qglTexParameterf	 = NULL;
	qglTexSubImage2D	 = NULL;
	qglTranslatef		 = NULL;
	qglVertex2f			 = NULL;
	qglVertex3f			 = NULL;
	qglVertex3fv		 = NULL;
	qglVertexPointer	 = NULL;
	qglViewport			 = NULL;

	qwglSwapIntervalEXT	 = NULL;
}

#pragma warning(disable : 4113 4133 4047)
#define GPA(a) GetProcAddress (glw_state.hinstOpenGL, a)

/*
** QGL_Init
**
** This is responsible for binding our qgl function pointers to
** the appropriate GL stuff.  In Windows this means doing a
** LoadLibrary and a bunch of calls to GetProcAddress.  On other
** operating systems we need to do the right thing, whatever that
** might be.
**
*/
qboolean QGL_Init (const char *dllname)
{
	// update 3Dfx gamma irrespective of underlying DLL
	{
		char  envbuffer[1024];
		float g;

		g = 2.00 * (0.8 - (vid_gamma->value - 0.5)) + 1.0F;
		Com_sprintf (envbuffer, sizeof (envbuffer), "SSTV2_GAMMA=%f", g);
		putenv (envbuffer);
		Com_sprintf (envbuffer, sizeof (envbuffer), "SST_GAMMA=%f", g);
		putenv (envbuffer);
	}

	if ((glw_state.hinstOpenGL = LoadLibrary (dllname)) == 0)
	{
		char *buf = NULL;

		FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError (),
					   MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &buf, 0, NULL);
		ri.Con_Printf (PRINT_ALL, "%s\n", buf);
		return e_false;
	}

	gl_config.allow_cds = e_true;

	qglBindTexture = dllBindTexture = GPA ("glBindTexture");
	qglBlendFunc = dllBlendFunc = GPA ("glBlendFunc");
	qglClear = dllClear = GPA ("glClear");
	qglClearColor = dllClearColor = GPA ("glClearColor");
	qglColor3f = dllColor3f = GPA ("glColor3f");
	qglColor3fv = dllColor3fv = GPA ("glColor3fv");
	qglColor4f = dllColor4f = GPA ("glColor4f");
	qglColor4fv = dllColor4fv = GPA ("glColor4fv");
	qglColor4ub = dllColor4ub = GPA ("glColor4ub");
	qglColor4ubv = dllColor4ubv = GPA ("glColor4ubv");
	qglColorPointer = dllColorPointer = GPA ("glColorPointer");
	qglCullFace = dllCullFace = GPA ("glCullFace");
	qglDeleteTextures = dllDeleteTextures = GPA ("glDeleteTextures");
	qglDepthFunc = dllDepthFunc = GPA ("glDepthFunc");
	qglDepthMask = dllDepthMask = GPA ("glDepthMask");
	qglDepthRange = dllDepthRange = GPA ("glDepthRange");
	qglDisable = dllDisable = GPA ("glDisable");
	qglDrawBuffer = dllDrawBuffer = GPA ("glDrawBuffer");
	qglEnable = dllEnable = GPA ("glEnable");
	qglEnableClientState = dllEnableClientState = GPA ("glEnableClientState");
	qglEnd = dllEnd = GPA ("glEnd");
	qglFinish = dllFinish = GPA ("glFinish");
	qglFrustum = dllFrustum = GPA ("glFrustum");
	qglGetError = dllGetError = GPA ("glGetError");
	qglGetFloatv = dllGetFloatv = GPA ("glGetFloatv");
	qglGetString = dllGetString = GPA ("glGetString");
	qglLoadIdentity = dllLoadIdentity = GPA ("glLoadIdentity");
	qglLoadMatrixf = dllLoadMatrixf = GPA ("glLoadMatrixf");
	qglMatrixMode = dllMatrixMode = GPA ("glMatrixMode");
	qglOrtho = dllOrtho = GPA ("glOrtho");
	qglPointSize = dllPointSize = GPA ("glPointSize");
	qglPolygonMode = dllPolygonMode = GPA ("glPolygonMode");
	qglPopMatrix = dllPopMatrix = GPA ("glPopMatrix");
	qglPushMatrix = dllPushMatrix = GPA ("glPushMatrix");
	qglReadPixels = dllReadPixels = GPA ("glReadPixels");
	qglRotatef = dllRotatef = GPA ("glRotatef");
	qglScalef = dllScalef = GPA ("glScalef");
	qglScissor = dllScissor = GPA ("glScissor");
	qglShadeModel = dllShadeModel = GPA ("glShadeModel");
	qglTexCoord2f = dllTexCoord2f = GPA ("glTexCoord2f");
	qglTexEnvf = dllTexEnvf = GPA ("glTexEnvf");
	qglTexImage2D = dllTexImage2D = GPA ("glTexImage2D");
	qglTexParameterf = dllTexParameterf = GPA ("glTexParameterf");
	qglTexSubImage2D = dllTexSubImage2D = GPA ("glTexSubImage2D");
	qglTranslatef = dllTranslatef = GPA ("glTranslatef");
	qglVertex2f = dllVertex2f = GPA ("glVertex2f");
	qglVertex3f = dllVertex3f = GPA ("glVertex3f");
	qglVertex3fv = dllVertex3fv = GPA ("glVertex3fv");
	qglVertexPointer = dllVertexPointer = GPA ("glVertexPointer");
	qglViewport = dllViewport = GPA ("glViewport");

	qwglSwapIntervalEXT		  = 0;
	qglPointParameterfEXT	  = 0;
	qglPointParameterfvEXT	  = 0;
	qglColorTableEXT		  = 0;
	qglSelectTextureSGIS	  = 0;
	qglMTexCoord2fSGIS		  = 0;

	return e_true;
}

#pragma warning(default : 4113 4133 4047)
