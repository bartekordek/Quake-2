#ifndef __ref_gl_gl_rmain_h__
#define __ref_gl_gl_rmain_h__

#include "shared/ref.h"

EXTERNC void MYgluPerspective (double fovy, double aspect, double zNear, double zFar);
EXTERNC __declspec (dllexport) refexport_t GetRefAPI (refimport_t rimp);

#endif	// __ref_gl_gl_rmain_h__