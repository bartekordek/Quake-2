#pragma once

typedef struct cvar_s cvar_t;

class GL_Legacy
{
public:
	cvar_t *r_novis{nullptr};
	cvar_t *r_nocull{nullptr};

	cvar_t *r_lerpmodels{nullptr};

	cvar_t *r_lightlevel{nullptr};	// FIXME: This is a HACK to get the client's light level

	cvar_t *gl_vertex_arrays{nullptr};

	cvar_t *gl_ext_swapinterval{nullptr};
	cvar_t *gl_ext_palettedtexture{nullptr};
	cvar_t *gl_ext_multitexture{nullptr};
	cvar_t *gl_ext_pointparameters{nullptr};
	cvar_t *gl_ext_compiled_vertex_array{nullptr};

	cvar_t *gl_particle_min_size{nullptr};
	cvar_t *gl_particle_max_size{nullptr};
	cvar_t *gl_particle_size{nullptr};
	cvar_t *gl_particle_att_a{nullptr};
	cvar_t *gl_particle_att_b{nullptr};
	cvar_t *gl_particle_att_c{nullptr};

	cvar_t *gl_nosubimage{nullptr};
	cvar_t *gl_bitdepth{nullptr};
	cvar_t *gl_mode{nullptr};
	cvar_t *gl_log{nullptr};
	cvar_t *gl_lightmap{nullptr};
	cvar_t *gl_shadows{nullptr};
	cvar_t *gl_dynamic{nullptr};
	cvar_t *gl_monolightmap{nullptr};
	cvar_t *gl_nobind{nullptr};
	cvar_t *gl_round_down{nullptr};
	cvar_t *gl_picmip{nullptr};
	cvar_t *gl_skymip{nullptr};
	cvar_t *gl_showtris{nullptr};
	cvar_t *gl_finish{nullptr};
	cvar_t *gl_ztrick{nullptr};
	cvar_t *gl_clear{nullptr};
	cvar_t *gl_cull{nullptr};
	cvar_t *gl_poly{nullptr};
	cvar_t *gl_texsort{nullptr};
	cvar_t *gl_polyblend{nullptr};
	cvar_t *gl_flashblend{nullptr};
	cvar_t *gl_lightmaptype{nullptr};
	cvar_t *gl_modulate{nullptr};
	cvar_t *gl_playermip{nullptr};
	cvar_t *gl_drawbuffer{nullptr};
	cvar_t *gl_3dlabs_broken{nullptr};
	cvar_t *gl_driver{nullptr};
	cvar_t *gl_swapinterval{nullptr};
	cvar_t *gl_texturemode{nullptr};
	cvar_t *gl_texturealphamode{nullptr};
	cvar_t *gl_texturesolidmode{nullptr};
	cvar_t *gl_saturatelighting{nullptr};
	cvar_t *gl_lockpvs{nullptr};

	cvar_t *vid_fullscreen{nullptr};

	cvar_t *intensity{nullptr};

	cvar_t *gl_allow_software{nullptr};
	cvar_t *vid_ref{nullptr};

	static GL_Legacy &Get_instance ();
	void			  LogNewFrame ();
	void			  R_Register ();

protected:
private:
	GL_Legacy ();
	~GL_Legacy ();
};