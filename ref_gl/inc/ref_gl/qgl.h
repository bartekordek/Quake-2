#ifndef __QGL_H__
#define __QGL_H__

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
** QGL.H
*/

#include "shared/config.h"

#include <GL/gl.h>

EXTERNC qboolean QGL_Init (const char *dllname);
EXTERNC void	 QGL_Shutdown (void);

#ifndef APIENTRY
#define APIENTRY
#endif

EXTERNC void (APIENTRY *qglArrayElement) (GLint i);
EXTERNC void (APIENTRY *qglBegin) (GLenum mode);
EXTERNC void (APIENTRY *qglBindTexture) (GLenum target, GLuint texture);
EXTERNC void (APIENTRY *qglBlendFunc) (GLenum sfactor, GLenum dfactor);
EXTERNC void (APIENTRY *qglClear) (GLbitfield mask);
EXTERNC void (APIENTRY *qglClearColor) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
EXTERNC void (APIENTRY *qglColor3f) (GLfloat red, GLfloat green, GLfloat blue);
EXTERNC void (APIENTRY *qglColor3fv) (const GLfloat *v);
EXTERNC void (APIENTRY *qglColor4f) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
EXTERNC void (APIENTRY *qglColor4fv) (const GLfloat *v);
EXTERNC void (APIENTRY *qglColor4ub) (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
EXTERNC void (APIENTRY *qglColor4ubv) (const GLubyte *v);
EXTERNC void (APIENTRY *qglColorPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
EXTERNC void (APIENTRY *qglCullFace) (GLenum mode);
EXTERNC void (APIENTRY *qglDeleteTextures) (GLsizei n, const GLuint *textures);
EXTERNC void (APIENTRY *qglDepthFunc) (GLenum func);
EXTERNC void (APIENTRY *qglDepthMask) (GLboolean flag);
EXTERNC void (APIENTRY *qglDepthRange) (GLclampd zNear, GLclampd zFar);
EXTERNC void (APIENTRY *qglDisable) (GLenum cap);
EXTERNC void (APIENTRY *qglDrawBuffer) (GLenum mode);
EXTERNC void (APIENTRY *qglEnable) (GLenum cap);
EXTERNC void (APIENTRY *qglEnableClientState) (GLenum array);
EXTERNC void (APIENTRY *qglEnd) (void);
EXTERNC void (APIENTRY *qglFinish) (void);
EXTERNC void (APIENTRY *qglFrustum) (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
EXTERNC GLenum (APIENTRY *qglGetError) (void);
EXTERNC void (APIENTRY *qglGetFloatv) (GLenum pname, GLfloat *params);
EXTERNC const GLubyte *(APIENTRY *qglGetString) (GLenum name);
EXTERNC void (APIENTRY *qglLoadIdentity) (void);
EXTERNC void (APIENTRY *qglLoadMatrixf) (const GLfloat *m);
EXTERNC void (APIENTRY *qglMatrixMode) (GLenum mode);
EXTERNC void (APIENTRY *qglOrtho) (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
EXTERNC void (APIENTRY *qglPointSize) (GLfloat size);
EXTERNC void (APIENTRY *qglPolygonMode) (GLenum face, GLenum mode);
EXTERNC void (APIENTRY *qglPopMatrix) (void);
EXTERNC void (APIENTRY *qglPushMatrix) (void);
EXTERNC void (APIENTRY *qglReadPixels) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
EXTERNC void (APIENTRY *qglRotatef) (GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
EXTERNC void (APIENTRY *qglScalef) (GLfloat x, GLfloat y, GLfloat z);
EXTERNC void (APIENTRY *qglScissor) (GLint x, GLint y, GLsizei width, GLsizei height);
EXTERNC void (APIENTRY *qglShadeModel) (GLenum mode);
EXTERNC void (APIENTRY *qglTexCoord2f) (GLfloat s, GLfloat t);
EXTERNC void (APIENTRY *qglTexEnvf) (GLenum target, GLenum pname, GLfloat param);
EXTERNC void (APIENTRY *qglTexImage2D) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border,
										GLenum format, GLenum type, const GLvoid *pixels);
EXTERNC void (APIENTRY *qglTexParameterf) (GLenum target, GLenum pname, GLfloat param);
EXTERNC void (APIENTRY *qglTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height,
										   GLenum format, GLenum type, const GLvoid *pixels);
EXTERNC void (APIENTRY *qglTranslatef) (GLfloat x, GLfloat y, GLfloat z);
EXTERNC void (APIENTRY *qglVertex2f) (GLfloat x, GLfloat y);
EXTERNC void (APIENTRY *qglVertex3f) (GLfloat x, GLfloat y, GLfloat z);
EXTERNC void (APIENTRY *qglVertex3fv) (const GLfloat *v);
EXTERNC void (APIENTRY *qglVertexPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
EXTERNC void (APIENTRY *qglViewport) (GLint x, GLint y, GLsizei width, GLsizei height);

EXTERNC void (APIENTRY *qglPointParameterfEXT) (GLenum param, GLfloat value);
EXTERNC void (APIENTRY *qglPointParameterfvEXT) (GLenum param, const GLfloat *value);
EXTERNC void (APIENTRY *qglColorTableEXT) (int, int, int, int, int, const void *);

EXTERNC void (APIENTRY *qglLockArraysEXT) (int, int);
EXTERNC void (APIENTRY *qglUnlockArraysEXT) (void);

EXTERNC void (APIENTRY *qglMTexCoord2fSGIS) (GLenum, GLfloat, GLfloat);
EXTERNC void (APIENTRY *qglSelectTextureSGIS) (GLenum);

#ifdef _WIN32
EXTERNC BOOL (WINAPI *qwglSwapIntervalEXT) (int interval);

#endif

/*
** extension constants
*/
#define GL_POINT_SIZE_MIN_EXT			 0x8126
#define GL_POINT_SIZE_MAX_EXT			 0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE_EXT 0x8128
#define GL_DISTANCE_ATTENUATION_EXT		 0x8129

#ifdef __sgi
#define GL_SHARED_TEXTURE_PALETTE_EXT GL_TEXTURE_COLOR_TABLE_SGI
#else
#define GL_SHARED_TEXTURE_PALETTE_EXT 0x81FB
#endif

#ifndef GL_TEXTURE0_SGIS
#define GL_TEXTURE0_SGIS 0x835E
#endif
#ifndef GL_TEXTURE1_SGIS
#define GL_TEXTURE1_SGIS 0x835F
#endif

#endif	// __QGL_H__
