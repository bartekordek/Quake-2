#ifndef ____SHARED_MODEL_H__
#define ____SHARED_MODEL_H__

#include "shared/dmodel.h"
#include "shared/plane.h"
#include "shared/mleaf.h"
#include "shared/mnode.h"

#include "qcommon/qfiles.h"

//
// Whole model
//
typedef struct model_s
{
	char name[MAX_QPATH];

	int registration_sequence;

	modtype_t type;
	int numframes;

	int flags;

	/* volume occupied by the model graphics */
	vec3_t mins, maxs;
	float radius;

// solid volume for clipping (sent from server)
	qboolean clipbox;
	vec3_t clipmins, clipmaxs;

	/* brush model */
	int firstmodelsurface, nummodelsurfaces;
	int lightmap; /* only for submodels */

	int numsubmodels;
	dmodel_t* submodels;

	int numplanes;
	plane_t* planes;

	int numleafs;// number of visible leafs, not counting 0
	mleaf_t* leafs;

	int			numvertexes;
	mvertex_t	*vertexes;

	int			numedges;
	medge_t		*edges;

	int			numnodes;
	int			firstnode;
	mnode_t		*nodes;

	int			numtexinfo;
	mtexinfo_t	*texinfo;

	int			numsurfaces;
	msurface_t	*surfaces;

	int numsurfedges;
	int* surfedges;

	int nummarksurfaces;
	msurface_t** marksurfaces;

	dvis_t* vis;

	byte* lightdata;

	// for alias models, sprites and skins
	image_t* skins[MAX_MD2SKINS];

	int extradatasize;
	void *extradata;

	// submodules
	vec3_t origin;	// for sounds or lights
} model_t;

mleaf_t* ref_gl_Mod_PointInLeaf( vec3_t p, model_t* model );
byte* ref_gl_Mod_ClusterPVS( int cluster, model_t* model );

#endif // ____SHARED_MODEL_H__