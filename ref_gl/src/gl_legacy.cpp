#include "ref_gl/gl_legacy.hpp"
#include "ref_gl/gl_image.h"
#include "ref_gl/gl_local.h"
#include "shared/cvar.h"
#include "shared/ref.h"
#include <cstdio>
#if defined(_WIN32)
#include "ref_gl/glw_win.h"
#endif // defined(_WIN32)

cvar_t			  *r_lefthand;
cvar_t			  *r_norefresh;
cvar_t			  *r_fullbright;
cvar_t			  *r_drawentities;
cvar_t			  *r_drawworld;
cvar_t			  *r_speeds;

extern refimport_t ri;

GL_Legacy &GL_Legacy::Get_instance ()
{
	static GL_Legacy s_instance;
	return s_instance;
}

void GL_Legacy::LogNewFrame ()
{
#if defined(_WIN32)
	fprintf (glw_state.log_fp, "*** R_BeginFrame ***\n");
#endif // #if defined(_WIN32)
}

GL_Legacy::GL_Legacy ()
{
	// Constructor code here
}

void GL_Legacy::R_Register ()
{
	r_lefthand					 = ri.Cvar_Get ("hand", "0", CVAR_USERINFO | CVAR_ARCHIVE);
	r_norefresh					 = ri.Cvar_Get ("r_norefresh", "0", 0);
	r_fullbright				 = ri.Cvar_Get ("r_fullbright", "0", 0);
	r_drawentities				 = ri.Cvar_Get ("r_drawentities", "1", 0);
	r_drawworld					 = ri.Cvar_Get ("r_drawworld", "1", 0);
	r_novis						 = ri.Cvar_Get ("r_novis", "0", 0);
	r_nocull					 = ri.Cvar_Get ("r_nocull", "0", 0);
	r_lerpmodels				 = ri.Cvar_Get ("r_lerpmodels", "1", 0);
	r_speeds					 = ri.Cvar_Get ("r_speeds", "0", 0);

	r_lightlevel				 = ri.Cvar_Get ("r_lightlevel", "0", 0);

	gl_nosubimage				 = ri.Cvar_Get ("gl_nosubimage", "0", 0);
	gl_allow_software			 = ri.Cvar_Get ("gl_allow_software", "0", 0);

	gl_particle_min_size		 = ri.Cvar_Get ("gl_particle_min_size", "2", CVAR_ARCHIVE);
	gl_particle_max_size		 = ri.Cvar_Get ("gl_particle_max_size", "40", CVAR_ARCHIVE);
	gl_particle_size			 = ri.Cvar_Get ("gl_particle_size", "40", CVAR_ARCHIVE);
	gl_particle_att_a			 = ri.Cvar_Get ("gl_particle_att_a", "0.01", CVAR_ARCHIVE);
	gl_particle_att_b			 = ri.Cvar_Get ("gl_particle_att_b", "0.0", CVAR_ARCHIVE);
	gl_particle_att_c			 = ri.Cvar_Get ("gl_particle_att_c", "0.01", CVAR_ARCHIVE);

	gl_modulate					 = ri.Cvar_Get ("gl_modulate", "1", CVAR_ARCHIVE);
	gl_log						 = ri.Cvar_Get ("gl_log", "0", 0);
	gl_bitdepth					 = ri.Cvar_Get ("gl_bitdepth", "0", 0);

	gl_mode						 = ri.Cvar_Get ("gl_mode", "3", CVAR_ARCHIVE);
	gl_lightmap					 = ri.Cvar_Get ("gl_lightmap", "0", 0);
	gl_shadows					 = ri.Cvar_Get ("gl_shadows", "0", CVAR_ARCHIVE);
	gl_dynamic					 = ri.Cvar_Get ("gl_dynamic", "1", 0);
	gl_nobind					 = ri.Cvar_Get ("gl_nobind", "0", 0);
	gl_round_down				 = ri.Cvar_Get ("gl_round_down", "1", 0);
	gl_picmip					 = ri.Cvar_Get ("gl_picmip", "0", 0);
	gl_skymip					 = ri.Cvar_Get ("gl_skymip", "0", 0);
	gl_showtris					 = ri.Cvar_Get ("gl_showtris", "0", 0);
	gl_ztrick					 = ri.Cvar_Get ("gl_ztrick", "0", 0);
	gl_finish					 = ri.Cvar_Get ("gl_finish", "0", CVAR_ARCHIVE);
	gl_clear					 = ri.Cvar_Get ("gl_clear", "0", 0);
	gl_cull						 = ri.Cvar_Get ("gl_cull", "1", 0);
	gl_polyblend				 = ri.Cvar_Get ("gl_polyblend", "1", 0);
	gl_flashblend				 = ri.Cvar_Get ("gl_flashblend", "0", 0);
	gl_playermip				 = ri.Cvar_Get ("gl_playermip", "0", 0);
	gl_monolightmap				 = ri.Cvar_Get ("gl_monolightmap", "0", 0);
	gl_driver					 = ri.Cvar_Get ("gl_driver", "opengl32", CVAR_ARCHIVE);
	gl_texturemode				 = ri.Cvar_Get ("gl_texturemode", "GL_LINEAR_MIPMAP_NEAREST", CVAR_ARCHIVE);
	gl_texturealphamode			 = ri.Cvar_Get ("gl_texturealphamode", "default", CVAR_ARCHIVE);
	gl_texturesolidmode			 = ri.Cvar_Get ("gl_texturesolidmode", "default", CVAR_ARCHIVE);
	gl_lockpvs					 = ri.Cvar_Get ("gl_lockpvs", "0", 0);

	gl_vertex_arrays			 = ri.Cvar_Get ("gl_vertex_arrays", "0", CVAR_ARCHIVE);

	gl_ext_swapinterval			 = ri.Cvar_Get ("gl_ext_swapinterval", "1", CVAR_ARCHIVE);
	gl_ext_palettedtexture		 = ri.Cvar_Get ("gl_ext_palettedtexture", "1", CVAR_ARCHIVE);
	gl_ext_multitexture			 = ri.Cvar_Get ("gl_ext_multitexture", "1", CVAR_ARCHIVE);
	gl_ext_pointparameters		 = ri.Cvar_Get ("gl_ext_pointparameters", "1", CVAR_ARCHIVE);
	gl_ext_compiled_vertex_array = ri.Cvar_Get ("gl_ext_compiled_vertex_array", "1", CVAR_ARCHIVE);

	gl_drawbuffer				 = ri.Cvar_Get ("gl_drawbuffer", "GL_BACK", 0);
	gl_swapinterval				 = ri.Cvar_Get ("gl_swapinterval", "1", CVAR_ARCHIVE);

	gl_saturatelighting			 = ri.Cvar_Get ("gl_saturatelighting", "0", 0);

	gl_3dlabs_broken			 = ri.Cvar_Get ("gl_3dlabs_broken", "1", CVAR_ARCHIVE);

	vid_fullscreen				 = ri.Cvar_Get ("vid_fullscreen", "0", CVAR_ARCHIVE);
	vid_gamma					 = ri.Cvar_Get ("vid_gamma", "1.0", CVAR_ARCHIVE);
	vid_ref						 = ri.Cvar_Get ("vid_ref", "soft", CVAR_ARCHIVE);

	ri.Cmd_AddCommand ("imagelist", GL_ImageList_f);
	ri.Cmd_AddCommand ("screenshot", GL_ScreenShot_f);
	ri.Cmd_AddCommand ("modellist", Mod_Modellist_f);
	ri.Cmd_AddCommand ("gl_strings", GL_Strings_f);
}

GL_Legacy::~GL_Legacy ()
{
	// Destructor code here
}
