#include "renderer/modern_opengl_c_.h"
#include "shared/image.h"
#include "shared/assert.h"
#include "shared/cvar.h"
#include "quake2/windows/window_util.h"

void R_BeginRegistration (char *model);
model_s *R_RegisterModel (char *name);
image_s *R_RegisterSkin (char *name);
image_t *Draw_FindPic (char *name);
void	 R_SetSky (char *name, float rotate, vec3_t axis);
void	 R_EndRegistration (void);
void	 R_RenderFrame (refdef_t *fd);
void	 Draw_GetPicSize (int *w, int *h, char *pic);
void	 Draw_Pic (int x, int y, char *name);
void	 Draw_StretchPic (int x, int y, int w, int h, char *name);
void	 Draw_Char (int x, int y, int num);
void	 Draw_TileClear (int x, int y, int w, int h, char *name);
void	 Draw_Fill (int x, int y, int w, int h, int c);
void	 Draw_FadeScreen (void);
void	 Draw_StretchRaw (int x, int y, int w, int h, int cols, int rows, byte *data);
qboolean R_Init ();
void	 R_Shutdown (void);
void	 R_CinematicSetPalette (const unsigned char *palette);
void	 R_BeginFrame (float camera_separation);
void	 SWimp_EndFrame (void);
void	 SWimp_AppActivate (qboolean active);

refimport_t ri;

typedef struct vidmode_s
{
	const char *description;
	int			width, height;
	int			mode;
} vidmode_t;

vidmode_t vid_modes[] = {
	{"Mode 0: 320x240", 320, 240, 0},
	{"Mode 1: 400x300", 400, 300, 1},
	{"Mode 2: 512x384", 512, 384, 2},
	{"Mode 3: 640x480", 640, 480, 3},
	{"Mode 4: 800x600", 800, 600, 4},
	{"Mode 5: 960x720", 960, 720, 5},
	{"Mode 6: 1024x768", 1024, 768, 6},
	{"Mode 7: 1152x864", 1152, 864, 7},
	{"Mode 8: 1280x960", 1280, 960, 8},
	{"Mode 9: 1600x1200", 1600, 1200, 9}};


cvar_t *opengl_mode;
cvar_t *vid_fullscreen;

refexport_t ImportModernOpenglApi (refimport_t inImport)
{
	refexport_t re;

	ri					   = inImport;

	re.api_version = API_VERSION;
	re.BeginRegistration = R_BeginRegistration;
	re.RegisterModel	   = R_RegisterModel;
	re.RegisterSkin		   = R_RegisterSkin;
	re.RegisterPic		   = Draw_FindPic;
	re.SetSky			   = R_SetSky;
	re.EndRegistration	   = R_EndRegistration;

	re.RenderFrame		   = R_RenderFrame;

	re.DrawGetPicSize	   = Draw_GetPicSize;
	re.DrawPic			   = Draw_Pic;
	re.DrawStretchPic	   = Draw_StretchPic;
	re.DrawChar			   = Draw_Char;
	re.DrawTileClear	   = Draw_TileClear;
	re.DrawFill			   = Draw_Fill;
	re.DrawFadeScreen	   = Draw_FadeScreen;

	re.DrawStretchRaw	   = Draw_StretchRaw;

	re.Init				   = R_Init;
	re.Shutdown			   = R_Shutdown;

	re.CinematicSetPalette = R_CinematicSetPalette;
	re.BeginFrame		   = R_BeginFrame;
	re.EndFrame			   = SWimp_EndFrame;

	re.AppActivate		   = SWimp_AppActivate;

	Swap_Init ();

	return re;
}


void R_BeginRegistration (char *model)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

model_s* R_RegisterModel(char* name)
{
	Q2_Assert (0, "R_RegisterModel is not implemented");

	return nullptr;
}

image_s *R_RegisterSkin (char *name)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");

	return nullptr;
}

image_t *Draw_FindPic (char *name)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
	return nullptr;
}

void	 R_SetSky (char *name, float rotate, vec3_t axis)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

void	 R_EndRegistration (void)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

void	 R_RenderFrame (refdef_t *fd)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

void	 Draw_GetPicSize (int *w, int *h, char *pic)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

void	 Draw_Pic (int x, int y, char *name)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

void	 Draw_StretchPic (int x, int y, int w, int h, char *name)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

void	 Draw_Char (int x, int y, int num)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

void	 Draw_TileClear (int x, int y, int w, int h, char *name)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

void	 Draw_Fill (int x, int y, int w, int h, int c)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

void	 Draw_FadeScreen (void)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

void	 Draw_StretchRaw (int x, int y, int w, int h, int cols, int rows, byte *data)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

qboolean R_Init ()
{
	int width{0};
	int height{0};
	int mode{0};


	constexpr int gl_version_major = 4;
	constexpr int gl_version_minor = 3;

	opengl_mode					   = Cvar_Get ("gl_mode", "3", CVAR_ARCHIVE);
	vid_fullscreen				   = Cvar_Get ("vid_fullscreen", "0", CVAR_ARCHIVE);
	ri.Vid_GetModeInfo (&width, &height, opengl_mode->value);
	ri.create_window (512, 64, width, height, vid_fullscreen->value ? e_true : e_false);

	char ref_version[512];
	snprintf (ref_version, 512, "GL %d.%d\n", gl_version_major, gl_version_minor);

	ri.Con_Printf (PRINT_ALL, ref_version);
	Set_opengl_version (gl_version_major, gl_version_minor);

	Q2_Assert (0, "R_BeginRegistration is not implemented");

	return e_false;
}

void	 R_Shutdown (void)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

void	 R_CinematicSetPalette (const unsigned char *palette)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

void	 R_BeginFrame (float camera_separation)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

void	 SWimp_EndFrame (void)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

void	 SWimp_AppActivate (qboolean active)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

