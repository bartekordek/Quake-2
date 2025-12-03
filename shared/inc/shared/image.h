#ifndef __shared__image_h__
#define __shared__image_h__

#include "shared/shared.h"

typedef enum
{
	it_skin,
	it_sprite,
	it_wall,
	it_pic,
	it_sky
} imagetype_t;


typedef struct image_s
{
	char			   name[MAX_QPATH];	 // game path, including extension
	imagetype_t		   type;
	int				   width, height;				 // source image
	int				   upload_width, upload_height;	 // after power of two and picmip
	int				   registration_sequence;		 // 0 = free
	struct msurface_s *texturechain;				 // for sort-by-texture world drawing
	int				   texnum;						 // gl texture binding
	float			   sl, tl, sh, th;				 // 0,0 - 1,1 unless part of the scrap
	qboolean		   scrap;
	qboolean		   has_alpha;

	qboolean paletted;
} image_t;

#endif	// __shared__image_h__