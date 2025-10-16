#ifndef __shared_cmodel_h__
#define __shared_cmodel_h__

#include "shared/config.h"
#include "shared/shared.h"

EXTERNC int numtexinfo;
EXTERNC mapsurface_t	map_surfaces[];
EXTERNC int c_traces;
EXTERNC int c_brush_traces;
EXTERNC int c_pointcontents;

#endif // __shared_cmodel_h__