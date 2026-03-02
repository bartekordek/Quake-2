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
#  define APIENTRY
#endif

EXTERNC  void ( APIENTRY * qglAccum )(GLenum op, GLfloat value);
EXTERNC  void ( APIENTRY * qglAlphaFunc )(GLenum func, GLclampf ref);
EXTERNC  GLboolean ( APIENTRY * qglAreTexturesResident )(GLsizei n, const GLuint *textures, GLboolean *residences);
EXTERNC  void ( APIENTRY * qglArrayElement )(GLint i);
EXTERNC  void ( APIENTRY * qglBegin )(GLenum mode);
EXTERNC  void ( APIENTRY * qglBindTexture )(GLenum target, GLuint texture);
EXTERNC  void ( APIENTRY * qglBitmap )(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
EXTERNC  void ( APIENTRY * qglBlendFunc )(GLenum sfactor, GLenum dfactor);
EXTERNC  void ( APIENTRY * qglCallList )(GLuint list);
EXTERNC  void ( APIENTRY * qglCallLists )(GLsizei n, GLenum type, const GLvoid *lists);
EXTERNC  void ( APIENTRY * qglClear )(GLbitfield mask);
EXTERNC  void ( APIENTRY * qglClearAccum )(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
EXTERNC  void ( APIENTRY * qglClearColor )(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
EXTERNC  void ( APIENTRY * qglClearDepth )(GLclampd depth);
EXTERNC  void ( APIENTRY * qglClearIndex )(GLfloat c);
EXTERNC  void ( APIENTRY * qglClearStencil )(GLint s);
EXTERNC  void ( APIENTRY * qglClipPlane )(GLenum plane, const GLdouble *equation);
EXTERNC  void ( APIENTRY * qglColor3b )(GLbyte red, GLbyte green, GLbyte blue);
EXTERNC  void ( APIENTRY * qglColor3bv )(const GLbyte *v);
EXTERNC  void ( APIENTRY * qglColor3d )(GLdouble red, GLdouble green, GLdouble blue);
EXTERNC  void ( APIENTRY * qglColor3dv )(const GLdouble *v);
EXTERNC  void ( APIENTRY * qglColor3f )(GLfloat red, GLfloat green, GLfloat blue);
EXTERNC  void ( APIENTRY * qglColor3fv )(const GLfloat *v);
EXTERNC  void ( APIENTRY * qglColor3i )(GLint red, GLint green, GLint blue);
EXTERNC  void ( APIENTRY * qglColor3iv )(const GLint *v);
EXTERNC  void ( APIENTRY * qglColor3s )(GLshort red, GLshort green, GLshort blue);
EXTERNC  void ( APIENTRY * qglColor3sv )(const GLshort *v);
EXTERNC  void ( APIENTRY * qglColor3ub )(GLubyte red, GLubyte green, GLubyte blue);
EXTERNC  void ( APIENTRY * qglColor3ubv )(const GLubyte *v);
EXTERNC  void ( APIENTRY * qglColor3ui )(GLuint red, GLuint green, GLuint blue);
EXTERNC  void ( APIENTRY * qglColor3uiv )(const GLuint *v);
EXTERNC  void ( APIENTRY * qglColor3us )(GLushort red, GLushort green, GLushort blue);
EXTERNC  void ( APIENTRY * qglColor3usv )(const GLushort *v);
EXTERNC  void ( APIENTRY * qglColor4b )(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
EXTERNC  void ( APIENTRY * qglColor4bv )(const GLbyte *v);
EXTERNC  void ( APIENTRY * qglColor4d )(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
EXTERNC  void ( APIENTRY * qglColor4dv )(const GLdouble *v);
EXTERNC  void ( APIENTRY * qglColor4f )(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
EXTERNC  void ( APIENTRY * qglColor4fv )(const GLfloat *v);
EXTERNC  void ( APIENTRY * qglColor4i )(GLint red, GLint green, GLint blue, GLint alpha);
EXTERNC  void ( APIENTRY * qglColor4iv )(const GLint *v);
EXTERNC  void ( APIENTRY * qglColor4s )(GLshort red, GLshort green, GLshort blue, GLshort alpha);
EXTERNC  void ( APIENTRY * qglColor4sv )(const GLshort *v);
EXTERNC  void ( APIENTRY * qglColor4ub )(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
EXTERNC  void ( APIENTRY * qglColor4ubv )(const GLubyte *v);
EXTERNC  void ( APIENTRY * qglColor4ui )(GLuint red, GLuint green, GLuint blue, GLuint alpha);
EXTERNC  void ( APIENTRY * qglColor4uiv )(const GLuint *v);
EXTERNC  void ( APIENTRY * qglColor4us )(GLushort red, GLushort green, GLushort blue, GLushort alpha);
EXTERNC  void ( APIENTRY * qglColor4usv )(const GLushort *v);
EXTERNC  void ( APIENTRY * qglColorMask )(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
EXTERNC  void ( APIENTRY * qglColorMaterial )(GLenum face, GLenum mode);
EXTERNC  void ( APIENTRY * qglColorPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
EXTERNC  void ( APIENTRY * qglCopyPixels )(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
EXTERNC  void ( APIENTRY * qglCopyTexImage1D )(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border);
EXTERNC  void ( APIENTRY * qglCopyTexImage2D )(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
EXTERNC  void ( APIENTRY * qglCopyTexSubImage1D )(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
EXTERNC  void ( APIENTRY * qglCopyTexSubImage2D )(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
EXTERNC  void ( APIENTRY * qglCullFace )(GLenum mode);
EXTERNC  void ( APIENTRY * qglDeleteLists )(GLuint list, GLsizei range);
EXTERNC  void ( APIENTRY * qglDeleteTextures )(GLsizei n, const GLuint *textures);
EXTERNC  void ( APIENTRY * qglDepthFunc )(GLenum func);
EXTERNC  void ( APIENTRY * qglDepthMask )(GLboolean flag);
EXTERNC  void ( APIENTRY * qglDepthRange )(GLclampd zNear, GLclampd zFar);
EXTERNC  void ( APIENTRY * qglDisable )(GLenum cap);
EXTERNC  void ( APIENTRY * qglDisableClientState )(GLenum array);
EXTERNC  void ( APIENTRY * qglDrawArrays )(GLenum mode, GLint first, GLsizei count);
EXTERNC  void ( APIENTRY * qglDrawBuffer )(GLenum mode);
EXTERNC  void ( APIENTRY * qglDrawElements )(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
EXTERNC  void ( APIENTRY * qglDrawPixels )(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
EXTERNC  void ( APIENTRY * qglEdgeFlag )(GLboolean flag);
EXTERNC  void ( APIENTRY * qglEdgeFlagPointer )(GLsizei stride, const GLvoid *pointer);
EXTERNC  void ( APIENTRY * qglEdgeFlagv )(const GLboolean *flag);
EXTERNC  void ( APIENTRY * qglEnable )(GLenum cap);
EXTERNC  void ( APIENTRY * qglEnableClientState )(GLenum array);
EXTERNC  void ( APIENTRY * qglEnd )(void);
EXTERNC  void ( APIENTRY * qglEndList )(void);
EXTERNC  void ( APIENTRY * qglEvalCoord1d )(GLdouble u);
EXTERNC  void ( APIENTRY * qglEvalCoord1dv )(const GLdouble *u);
EXTERNC  void ( APIENTRY * qglEvalCoord1f )(GLfloat u);
EXTERNC  void ( APIENTRY * qglEvalCoord1fv )(const GLfloat *u);
EXTERNC  void ( APIENTRY * qglEvalCoord2d )(GLdouble u, GLdouble v);
EXTERNC  void ( APIENTRY * qglEvalCoord2dv )(const GLdouble *u);
EXTERNC  void ( APIENTRY * qglEvalCoord2f )(GLfloat u, GLfloat v);
EXTERNC  void ( APIENTRY * qglEvalCoord2fv )(const GLfloat *u);
EXTERNC  void ( APIENTRY * qglEvalMesh1 )(GLenum mode, GLint i1, GLint i2);
EXTERNC  void ( APIENTRY * qglEvalMesh2 )(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
EXTERNC  void ( APIENTRY * qglEvalPoint1 )(GLint i);
EXTERNC  void ( APIENTRY * qglEvalPoint2 )(GLint i, GLint j);
EXTERNC  void ( APIENTRY * qglFeedbackBuffer )(GLsizei size, GLenum type, GLfloat *buffer);
EXTERNC  void ( APIENTRY * qglFinish )(void);
EXTERNC  void ( APIENTRY * qglFlush )(void);
EXTERNC  void ( APIENTRY * qglFogf )(GLenum pname, GLfloat param);
EXTERNC  void ( APIENTRY * qglFogfv )(GLenum pname, const GLfloat *params);
EXTERNC  void ( APIENTRY * qglFogi )(GLenum pname, GLint param);
EXTERNC  void ( APIENTRY * qglFogiv )(GLenum pname, const GLint *params);
EXTERNC  void ( APIENTRY * qglFrontFace )(GLenum mode);
EXTERNC  void ( APIENTRY * qglFrustum )(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
EXTERNC  GLuint ( APIENTRY * qglGenLists )(GLsizei range);
EXTERNC  void ( APIENTRY * qglGenTextures )(GLsizei n, GLuint *textures);
EXTERNC  void ( APIENTRY * qglGetBooleanv )(GLenum pname, GLboolean *params);
EXTERNC  void ( APIENTRY * qglGetClipPlane )(GLenum plane, GLdouble *equation);
EXTERNC  void ( APIENTRY * qglGetDoublev )(GLenum pname, GLdouble *params);
EXTERNC  GLenum ( APIENTRY * qglGetError )(void);
EXTERNC  void ( APIENTRY * qglGetFloatv )(GLenum pname, GLfloat *params);
EXTERNC  void ( APIENTRY * qglGetIntegerv )(GLenum pname, GLint *params);
EXTERNC  void ( APIENTRY * qglGetLightfv )(GLenum light, GLenum pname, GLfloat *params);
EXTERNC  void ( APIENTRY * qglGetLightiv )(GLenum light, GLenum pname, GLint *params);
EXTERNC  void ( APIENTRY * qglGetMapdv )(GLenum target, GLenum query, GLdouble *v);
EXTERNC  void ( APIENTRY * qglGetMapfv )(GLenum target, GLenum query, GLfloat *v);
EXTERNC  void ( APIENTRY * qglGetMapiv )(GLenum target, GLenum query, GLint *v);
EXTERNC  void ( APIENTRY * qglGetMaterialfv )(GLenum face, GLenum pname, GLfloat *params);
EXTERNC  void ( APIENTRY * qglGetMaterialiv )(GLenum face, GLenum pname, GLint *params);
EXTERNC  void ( APIENTRY * qglGetPixelMapfv )(GLenum map, GLfloat *values);
EXTERNC  void ( APIENTRY * qglGetPixelMapuiv )(GLenum map, GLuint *values);
EXTERNC  void ( APIENTRY * qglGetPixelMapusv )(GLenum map, GLushort *values);
EXTERNC  void ( APIENTRY * qglGetPointerv )(GLenum pname, GLvoid* *params);
EXTERNC  void ( APIENTRY * qglGetPolygonStipple )(GLubyte *mask);
EXTERNC  const GLubyte * ( APIENTRY * qglGetString )(GLenum name);
EXTERNC  void ( APIENTRY * qglGetTexEnvfv )(GLenum target, GLenum pname, GLfloat *params);
EXTERNC  void ( APIENTRY * qglGetTexEnviv )(GLenum target, GLenum pname, GLint *params);
EXTERNC  void ( APIENTRY * qglGetTexGendv )(GLenum coord, GLenum pname, GLdouble *params);
EXTERNC  void ( APIENTRY * qglGetTexGenfv )(GLenum coord, GLenum pname, GLfloat *params);
EXTERNC  void ( APIENTRY * qglGetTexGeniv )(GLenum coord, GLenum pname, GLint *params);
EXTERNC  void ( APIENTRY * qglGetTexImage )(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
EXTERNC  void ( APIENTRY * qglGetTexLevelParameterfv )(GLenum target, GLint level, GLenum pname, GLfloat *params);
EXTERNC  void ( APIENTRY * qglGetTexLevelParameteriv )(GLenum target, GLint level, GLenum pname, GLint *params);
EXTERNC  void ( APIENTRY * qglGetTexParameterfv )(GLenum target, GLenum pname, GLfloat *params);
EXTERNC  void ( APIENTRY * qglGetTexParameteriv )(GLenum target, GLenum pname, GLint *params);
EXTERNC  void ( APIENTRY * qglHint )(GLenum target, GLenum mode);
EXTERNC  void ( APIENTRY * qglIndexMask )(GLuint mask);
EXTERNC  void ( APIENTRY * qglIndexPointer )(GLenum type, GLsizei stride, const GLvoid *pointer);
EXTERNC  void ( APIENTRY * qglIndexd )(GLdouble c);
EXTERNC  void ( APIENTRY * qglIndexdv )(const GLdouble *c);
EXTERNC  void ( APIENTRY * qglIndexf )(GLfloat c);
EXTERNC  void ( APIENTRY * qglIndexfv )(const GLfloat *c);
EXTERNC  void ( APIENTRY * qglIndexi )(GLint c);
EXTERNC  void ( APIENTRY * qglIndexiv )(const GLint *c);
EXTERNC  void ( APIENTRY * qglIndexs )(GLshort c);
EXTERNC  void ( APIENTRY * qglIndexsv )(const GLshort *c);
EXTERNC  void ( APIENTRY * qglIndexub )(GLubyte c);
EXTERNC  void ( APIENTRY * qglIndexubv )(const GLubyte *c);
EXTERNC  void ( APIENTRY * qglInitNames )(void);
EXTERNC  void ( APIENTRY * qglInterleavedArrays )(GLenum format, GLsizei stride, const GLvoid *pointer);
EXTERNC  GLboolean ( APIENTRY * qglIsEnabled )(GLenum cap);
EXTERNC  GLboolean ( APIENTRY * qglIsList )(GLuint list);
EXTERNC  GLboolean ( APIENTRY * qglIsTexture )(GLuint texture);
EXTERNC  void ( APIENTRY * qglLightModelf )(GLenum pname, GLfloat param);
EXTERNC  void ( APIENTRY * qglLightModelfv )(GLenum pname, const GLfloat *params);
EXTERNC  void ( APIENTRY * qglLightModeli )(GLenum pname, GLint param);
EXTERNC  void ( APIENTRY * qglLightModeliv )(GLenum pname, const GLint *params);
EXTERNC  void ( APIENTRY * qglLightf )(GLenum light, GLenum pname, GLfloat param);
EXTERNC  void ( APIENTRY * qglLightfv )(GLenum light, GLenum pname, const GLfloat *params);
EXTERNC  void ( APIENTRY * qglLighti )(GLenum light, GLenum pname, GLint param);
EXTERNC  void ( APIENTRY * qglLightiv )(GLenum light, GLenum pname, const GLint *params);
EXTERNC  void ( APIENTRY * qglLineStipple )(GLint factor, GLushort pattern);
EXTERNC  void ( APIENTRY * qglLineWidth )(GLfloat width);
EXTERNC  void ( APIENTRY * qglListBase )(GLuint base);
EXTERNC  void ( APIENTRY * qglLoadIdentity )(void);
EXTERNC  void ( APIENTRY * qglLoadMatrixd )(const GLdouble *m);
EXTERNC  void ( APIENTRY * qglLoadMatrixf )(const GLfloat *m);
EXTERNC  void ( APIENTRY * qglLoadName )(GLuint name);
EXTERNC  void ( APIENTRY * qglLogicOp )(GLenum opcode);
EXTERNC  void ( APIENTRY * qglMap1d )(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
EXTERNC  void ( APIENTRY * qglMap1f )(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
EXTERNC  void ( APIENTRY * qglMap2d )(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
EXTERNC  void ( APIENTRY * qglMap2f )(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
EXTERNC  void ( APIENTRY * qglMapGrid1d )(GLint un, GLdouble u1, GLdouble u2);
EXTERNC  void ( APIENTRY * qglMapGrid1f )(GLint un, GLfloat u1, GLfloat u2);
EXTERNC  void ( APIENTRY * qglMapGrid2d )(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
EXTERNC  void ( APIENTRY * qglMapGrid2f )(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
EXTERNC  void ( APIENTRY * qglMaterialf )(GLenum face, GLenum pname, GLfloat param);
EXTERNC  void ( APIENTRY * qglMaterialfv )(GLenum face, GLenum pname, const GLfloat *params);
EXTERNC  void ( APIENTRY * qglMateriali )(GLenum face, GLenum pname, GLint param);
EXTERNC  void ( APIENTRY * qglMaterialiv )(GLenum face, GLenum pname, const GLint *params);
EXTERNC  void ( APIENTRY * qglMatrixMode )(GLenum mode);
EXTERNC  void ( APIENTRY * qglMultMatrixd )(const GLdouble *m);
EXTERNC  void ( APIENTRY * qglMultMatrixf )(const GLfloat *m);
EXTERNC  void ( APIENTRY * qglNewList )(GLuint list, GLenum mode);
EXTERNC  void ( APIENTRY * qglNormal3b )(GLbyte nx, GLbyte ny, GLbyte nz);
EXTERNC  void ( APIENTRY * qglNormal3bv )(const GLbyte *v);
EXTERNC  void ( APIENTRY * qglNormal3d )(GLdouble nx, GLdouble ny, GLdouble nz);
EXTERNC  void ( APIENTRY * qglNormal3dv )(const GLdouble *v);
EXTERNC  void ( APIENTRY * qglNormal3f )(GLfloat nx, GLfloat ny, GLfloat nz);
EXTERNC  void ( APIENTRY * qglNormal3fv )(const GLfloat *v);
EXTERNC  void ( APIENTRY * qglNormal3i )(GLint nx, GLint ny, GLint nz);
EXTERNC  void ( APIENTRY * qglNormal3iv )(const GLint *v);
EXTERNC  void ( APIENTRY * qglNormal3s )(GLshort nx, GLshort ny, GLshort nz);
EXTERNC  void ( APIENTRY * qglNormal3sv )(const GLshort *v);
EXTERNC  void ( APIENTRY * qglNormalPointer )(GLenum type, GLsizei stride, const GLvoid *pointer);
EXTERNC  void ( APIENTRY * qglOrtho )(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
EXTERNC  void ( APIENTRY * qglPassThrough )(GLfloat token);
EXTERNC  void ( APIENTRY * qglPixelMapfv )(GLenum map, GLsizei mapsize, const GLfloat *values);
EXTERNC  void ( APIENTRY * qglPixelMapuiv )(GLenum map, GLsizei mapsize, const GLuint *values);
EXTERNC  void ( APIENTRY * qglPixelMapusv )(GLenum map, GLsizei mapsize, const GLushort *values);
EXTERNC  void ( APIENTRY * qglPixelStoref )(GLenum pname, GLfloat param);
EXTERNC  void ( APIENTRY * qglPixelStorei )(GLenum pname, GLint param);
EXTERNC  void ( APIENTRY * qglPixelTransferf )(GLenum pname, GLfloat param);
EXTERNC  void ( APIENTRY * qglPixelTransferi )(GLenum pname, GLint param);
EXTERNC  void ( APIENTRY * qglPixelZoom )(GLfloat xfactor, GLfloat yfactor);
EXTERNC  void ( APIENTRY * qglPointSize )(GLfloat size);
EXTERNC  void ( APIENTRY * qglPolygonMode )(GLenum face, GLenum mode);
EXTERNC  void ( APIENTRY * qglPolygonOffset )(GLfloat factor, GLfloat units);
EXTERNC  void ( APIENTRY * qglPolygonStipple )(const GLubyte *mask);
EXTERNC  void ( APIENTRY * qglPopAttrib )(void);
EXTERNC  void ( APIENTRY * qglPopClientAttrib )(void);
EXTERNC  void ( APIENTRY * qglPopMatrix )(void);
EXTERNC  void ( APIENTRY * qglPopName )(void);
EXTERNC  void ( APIENTRY * qglPrioritizeTextures )(GLsizei n, const GLuint *textures, const GLclampf *priorities);
EXTERNC  void ( APIENTRY * qglPushAttrib )(GLbitfield mask);
EXTERNC  void ( APIENTRY * qglPushClientAttrib )(GLbitfield mask);
EXTERNC  void ( APIENTRY * qglPushMatrix )(void);
EXTERNC  void ( APIENTRY * qglPushName )(GLuint name);
EXTERNC  void ( APIENTRY * qglRasterPos2d )(GLdouble x, GLdouble y);
EXTERNC  void ( APIENTRY * qglRasterPos2dv )(const GLdouble *v);
EXTERNC  void ( APIENTRY * qglRasterPos2f )(GLfloat x, GLfloat y);
EXTERNC  void ( APIENTRY * qglRasterPos2fv )(const GLfloat *v);
EXTERNC  void ( APIENTRY * qglRasterPos2i )(GLint x, GLint y);
EXTERNC  void ( APIENTRY * qglRasterPos2iv )(const GLint *v);
EXTERNC  void ( APIENTRY * qglRasterPos2s )(GLshort x, GLshort y);
EXTERNC  void ( APIENTRY * qglRasterPos2sv )(const GLshort *v);
EXTERNC  void ( APIENTRY * qglRasterPos3d )(GLdouble x, GLdouble y, GLdouble z);
EXTERNC  void ( APIENTRY * qglRasterPos3dv )(const GLdouble *v);
EXTERNC  void ( APIENTRY * qglRasterPos3f )(GLfloat x, GLfloat y, GLfloat z);
EXTERNC  void ( APIENTRY * qglRasterPos3fv )(const GLfloat *v);
EXTERNC  void ( APIENTRY * qglRasterPos3i )(GLint x, GLint y, GLint z);
EXTERNC  void ( APIENTRY * qglRasterPos3iv )(const GLint *v);
EXTERNC  void ( APIENTRY * qglRasterPos3s )(GLshort x, GLshort y, GLshort z);
EXTERNC  void ( APIENTRY * qglRasterPos3sv )(const GLshort *v);
EXTERNC  void ( APIENTRY * qglRasterPos4d )(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
EXTERNC  void ( APIENTRY * qglRasterPos4dv )(const GLdouble *v);
EXTERNC  void ( APIENTRY * qglRasterPos4f )(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
EXTERNC  void ( APIENTRY * qglRasterPos4fv )(const GLfloat *v);
EXTERNC  void ( APIENTRY * qglRasterPos4i )(GLint x, GLint y, GLint z, GLint w);
EXTERNC  void ( APIENTRY * qglRasterPos4iv )(const GLint *v);
EXTERNC  void ( APIENTRY * qglRasterPos4s )(GLshort x, GLshort y, GLshort z, GLshort w);
EXTERNC  void ( APIENTRY * qglRasterPos4sv )(const GLshort *v);
EXTERNC  void ( APIENTRY * qglReadBuffer )(GLenum mode);
EXTERNC  void ( APIENTRY * qglReadPixels )(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
EXTERNC  void ( APIENTRY * qglRectd )(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
EXTERNC  void ( APIENTRY * qglRectdv )(const GLdouble *v1, const GLdouble *v2);
EXTERNC  void ( APIENTRY * qglRectf )(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
EXTERNC  void ( APIENTRY * qglRectfv )(const GLfloat *v1, const GLfloat *v2);
EXTERNC  void ( APIENTRY * qglRecti )(GLint x1, GLint y1, GLint x2, GLint y2);
EXTERNC  void ( APIENTRY * qglRectiv )(const GLint *v1, const GLint *v2);
EXTERNC  void ( APIENTRY * qglRects )(GLshort x1, GLshort y1, GLshort x2, GLshort y2);
EXTERNC  void ( APIENTRY * qglRectsv )(const GLshort *v1, const GLshort *v2);
EXTERNC  GLint ( APIENTRY * qglRenderMode )(GLenum mode);
EXTERNC  void ( APIENTRY * qglRotated )(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
EXTERNC  void ( APIENTRY * qglRotatef )(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
EXTERNC  void ( APIENTRY * qglScaled )(GLdouble x, GLdouble y, GLdouble z);
EXTERNC  void ( APIENTRY * qglScalef )(GLfloat x, GLfloat y, GLfloat z);
EXTERNC  void ( APIENTRY * qglScissor )(GLint x, GLint y, GLsizei width, GLsizei height);
EXTERNC  void ( APIENTRY * qglSelectBuffer )(GLsizei size, GLuint *buffer);
EXTERNC  void ( APIENTRY * qglShadeModel )(GLenum mode);
EXTERNC  void ( APIENTRY * qglStencilFunc )(GLenum func, GLint ref, GLuint mask);
EXTERNC  void ( APIENTRY * qglStencilMask )(GLuint mask);
EXTERNC  void ( APIENTRY * qglStencilOp )(GLenum fail, GLenum zfail, GLenum zpass);
EXTERNC  void ( APIENTRY * qglTexCoord1d )(GLdouble s);
EXTERNC  void ( APIENTRY * qglTexCoord1dv )(const GLdouble *v);
EXTERNC  void ( APIENTRY * qglTexCoord1f )(GLfloat s);
EXTERNC  void ( APIENTRY * qglTexCoord1fv )(const GLfloat *v);
EXTERNC  void ( APIENTRY * qglTexCoord1i )(GLint s);
EXTERNC  void ( APIENTRY * qglTexCoord1iv )(const GLint *v);
EXTERNC  void ( APIENTRY * qglTexCoord1s )(GLshort s);
EXTERNC  void ( APIENTRY * qglTexCoord1sv )(const GLshort *v);
EXTERNC  void ( APIENTRY * qglTexCoord2d )(GLdouble s, GLdouble t);
EXTERNC  void ( APIENTRY * qglTexCoord2dv )(const GLdouble *v);
EXTERNC  void ( APIENTRY * qglTexCoord2f )(GLfloat s, GLfloat t);
EXTERNC  void ( APIENTRY * qglTexCoord2fv )(const GLfloat *v);
EXTERNC  void ( APIENTRY * qglTexCoord2i )(GLint s, GLint t);
EXTERNC  void ( APIENTRY * qglTexCoord2iv )(const GLint *v);
EXTERNC  void ( APIENTRY * qglTexCoord2s )(GLshort s, GLshort t);
EXTERNC  void ( APIENTRY * qglTexCoord2sv )(const GLshort *v);
EXTERNC  void ( APIENTRY * qglTexCoord3d )(GLdouble s, GLdouble t, GLdouble r);
EXTERNC  void ( APIENTRY * qglTexCoord3dv )(const GLdouble *v);
EXTERNC  void ( APIENTRY * qglTexCoord3f )(GLfloat s, GLfloat t, GLfloat r);
EXTERNC  void ( APIENTRY * qglTexCoord3fv )(const GLfloat *v);
EXTERNC  void ( APIENTRY * qglTexCoord3i )(GLint s, GLint t, GLint r);
EXTERNC  void ( APIENTRY * qglTexCoord3iv )(const GLint *v);
EXTERNC  void ( APIENTRY * qglTexCoord3s )(GLshort s, GLshort t, GLshort r);
EXTERNC  void ( APIENTRY * qglTexCoord3sv )(const GLshort *v);
EXTERNC  void ( APIENTRY * qglTexCoord4d )(GLdouble s, GLdouble t, GLdouble r, GLdouble q);
EXTERNC  void ( APIENTRY * qglTexCoord4dv )(const GLdouble *v);
EXTERNC  void ( APIENTRY * qglTexCoord4f )(GLfloat s, GLfloat t, GLfloat r, GLfloat q);
EXTERNC  void ( APIENTRY * qglTexCoord4fv )(const GLfloat *v);
EXTERNC  void ( APIENTRY * qglTexCoord4i )(GLint s, GLint t, GLint r, GLint q);
EXTERNC  void ( APIENTRY * qglTexCoord4iv )(const GLint *v);
EXTERNC  void ( APIENTRY * qglTexCoord4s )(GLshort s, GLshort t, GLshort r, GLshort q);
EXTERNC  void ( APIENTRY * qglTexCoord4sv )(const GLshort *v);
EXTERNC  void ( APIENTRY * qglTexCoordPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
EXTERNC  void ( APIENTRY * qglTexEnvf )(GLenum target, GLenum pname, GLfloat param);
EXTERNC  void ( APIENTRY * qglTexEnvfv )(GLenum target, GLenum pname, const GLfloat *params);
EXTERNC  void ( APIENTRY * qglTexEnvi )(GLenum target, GLenum pname, GLint param);
EXTERNC  void ( APIENTRY * qglTexEnviv )(GLenum target, GLenum pname, const GLint *params);
EXTERNC  void ( APIENTRY * qglTexGend )(GLenum coord, GLenum pname, GLdouble param);
EXTERNC  void ( APIENTRY * qglTexGendv )(GLenum coord, GLenum pname, const GLdouble *params);
EXTERNC  void ( APIENTRY * qglTexGenf )(GLenum coord, GLenum pname, GLfloat param);
EXTERNC  void ( APIENTRY * qglTexGenfv )(GLenum coord, GLenum pname, const GLfloat *params);
EXTERNC  void ( APIENTRY * qglTexGeni )(GLenum coord, GLenum pname, GLint param);
EXTERNC  void ( APIENTRY * qglTexGeniv )(GLenum coord, GLenum pname, const GLint *params);
EXTERNC  void ( APIENTRY * qglTexImage1D )(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
EXTERNC  void ( APIENTRY * qglTexImage2D )(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
EXTERNC  void ( APIENTRY * qglTexParameterf )(GLenum target, GLenum pname, GLfloat param);
EXTERNC  void ( APIENTRY * qglTexParameterfv )(GLenum target, GLenum pname, const GLfloat *params);
EXTERNC  void ( APIENTRY * qglTexParameteri )(GLenum target, GLenum pname, GLint param);
EXTERNC  void ( APIENTRY * qglTexParameteriv )(GLenum target, GLenum pname, const GLint *params);
EXTERNC  void ( APIENTRY * qglTexSubImage1D )(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
EXTERNC  void ( APIENTRY * qglTexSubImage2D )(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
EXTERNC  void ( APIENTRY * qglTranslated )(GLdouble x, GLdouble y, GLdouble z);
EXTERNC  void ( APIENTRY * qglTranslatef )(GLfloat x, GLfloat y, GLfloat z);
EXTERNC  void ( APIENTRY * qglVertex2d )(GLdouble x, GLdouble y);
EXTERNC  void ( APIENTRY * qglVertex2dv )(const GLdouble *v);
EXTERNC  void ( APIENTRY * qglVertex2f )(GLfloat x, GLfloat y);
EXTERNC  void ( APIENTRY * qglVertex2fv )(const GLfloat *v);
EXTERNC  void ( APIENTRY * qglVertex2i )(GLint x, GLint y);
EXTERNC  void ( APIENTRY * qglVertex2iv )(const GLint *v);
EXTERNC  void ( APIENTRY * qglVertex2s )(GLshort x, GLshort y);
EXTERNC  void ( APIENTRY * qglVertex2sv )(const GLshort *v);
EXTERNC  void ( APIENTRY * qglVertex3d )(GLdouble x, GLdouble y, GLdouble z);
EXTERNC  void ( APIENTRY * qglVertex3dv )(const GLdouble *v);
EXTERNC  void ( APIENTRY * qglVertex3f )(GLfloat x, GLfloat y, GLfloat z);
EXTERNC  void ( APIENTRY * qglVertex3fv )(const GLfloat *v);
EXTERNC  void ( APIENTRY * qglVertex3i )(GLint x, GLint y, GLint z);
EXTERNC  void ( APIENTRY * qglVertex3iv )(const GLint *v);
EXTERNC  void ( APIENTRY * qglVertex3s )(GLshort x, GLshort y, GLshort z);
EXTERNC  void ( APIENTRY * qglVertex3sv )(const GLshort *v);
EXTERNC  void ( APIENTRY * qglVertex4d )(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
EXTERNC  void ( APIENTRY * qglVertex4dv )(const GLdouble *v);
EXTERNC  void ( APIENTRY * qglVertex4f )(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
EXTERNC  void ( APIENTRY * qglVertex4fv )(const GLfloat *v);
EXTERNC  void ( APIENTRY * qglVertex4i )(GLint x, GLint y, GLint z, GLint w);
EXTERNC  void ( APIENTRY * qglVertex4iv )(const GLint *v);
EXTERNC  void ( APIENTRY * qglVertex4s )(GLshort x, GLshort y, GLshort z, GLshort w);
EXTERNC  void ( APIENTRY * qglVertex4sv )(const GLshort *v);
EXTERNC  void ( APIENTRY * qglVertexPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
EXTERNC  void ( APIENTRY * qglViewport )(GLint x, GLint y, GLsizei width, GLsizei height);

EXTERNC	void ( APIENTRY * qglPointParameterfEXT)( GLenum param, GLfloat value );
EXTERNC	void ( APIENTRY * qglPointParameterfvEXT)( GLenum param, const GLfloat *value );
EXTERNC	void ( APIENTRY * qglColorTableEXT)( int, int, int, int, int, const void * );

EXTERNC	void ( APIENTRY * qglLockArraysEXT) (int , int);
EXTERNC	void ( APIENTRY * qglUnlockArraysEXT) (void);

EXTERNC	void ( APIENTRY * qglMTexCoord2fSGIS)( GLenum, GLfloat, GLfloat );
EXTERNC	void ( APIENTRY * qglSelectTextureSGIS)( GLenum );

#ifdef _WIN32

EXTERNC  int   ( WINAPI * qwglChoosePixelFormat )(HDC, CONST PIXELFORMATDESCRIPTOR *);
EXTERNC  int   ( WINAPI * qwglDescribePixelFormat) (HDC, int, UINT, LPPIXELFORMATDESCRIPTOR);
EXTERNC  int   ( WINAPI * qwglGetPixelFormat)(HDC);
EXTERNC  BOOL  ( WINAPI * qwglSetPixelFormat)(HDC, int, CONST PIXELFORMATDESCRIPTOR *);
EXTERNC  BOOL  ( WINAPI * qwglSwapBuffers)(HDC);

EXTERNC BOOL  ( WINAPI * qwglCopyContext)(HGLRC, HGLRC, UINT);
EXTERNC HGLRC ( WINAPI * qwglCreateContext)(HDC);
EXTERNC HGLRC ( WINAPI * qwglCreateLayerContext)(HDC, int);
EXTERNC BOOL  ( WINAPI * qwglDeleteContext)(HGLRC);
EXTERNC HGLRC ( WINAPI * qwglGetCurrentContext)(VOID);
EXTERNC HDC   ( WINAPI * qwglGetCurrentDC)(VOID);
EXTERNC PROC  ( WINAPI * qwglGetProcAddress)(LPCSTR);
EXTERNC BOOL  ( WINAPI * qwglMakeCurrent)(HDC, HGLRC);
EXTERNC BOOL  ( WINAPI * qwglShareLists)(HGLRC, HGLRC);
EXTERNC BOOL  ( WINAPI * qwglUseFontBitmaps)(HDC, DWORD, DWORD, DWORD);

EXTERNC BOOL  ( WINAPI * qwglUseFontOutlines)(HDC, DWORD, DWORD, DWORD, FLOAT,
                                           FLOAT, int, LPGLYPHMETRICSFLOAT);

EXTERNC BOOL ( WINAPI * qwglDescribeLayerPlane)(HDC, int, int, UINT,
                                            LPLAYERPLANEDESCRIPTOR);
EXTERNC int  ( WINAPI * qwglSetLayerPaletteEntries)(HDC, int, int, int,
                                                CONST COLORREF *);
EXTERNC int  ( WINAPI * qwglGetLayerPaletteEntries)(HDC, int, int, int,
                                                COLORREF *);
EXTERNC BOOL ( WINAPI * qwglRealizeLayerPalette)(HDC, int, BOOL);
EXTERNC BOOL ( WINAPI * qwglSwapLayerBuffers)(HDC, UINT);

EXTERNC BOOL ( WINAPI * qwglSwapIntervalEXT)( int interval );

EXTERNC BOOL ( WINAPI * qwglGetDeviceGammaRampEXT ) ( unsigned char *pRed, unsigned char *pGreen, unsigned char *pBlue );
EXTERNC BOOL ( WINAPI * qwglSetDeviceGammaRampEXT ) ( const unsigned char *pRed, const unsigned char *pGreen, const unsigned char *pBlue );

#endif

/*
** extension constants
*/
#define GL_POINT_SIZE_MIN_EXT				0x8126
#define GL_POINT_SIZE_MAX_EXT				0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE_EXT	0x8128
#define GL_DISTANCE_ATTENUATION_EXT			0x8129

#ifdef __sgi
#define GL_SHARED_TEXTURE_PALETTE_EXT		GL_TEXTURE_COLOR_TABLE_SGI
#else
#define GL_SHARED_TEXTURE_PALETTE_EXT		0x81FB
#endif

#define GL_TEXTURE0_SGIS					0x835E
#define GL_TEXTURE1_SGIS					0x835F

#endif // __QGL_H__
