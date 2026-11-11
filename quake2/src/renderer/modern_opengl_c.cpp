#include "renderer/modern_opengl_c_.h"
#include "shared/image.h"
#include "shared/assert.h"
#include "shared/cvar.h"
#include "quake2/windows/window_util.hpp"

void	 R_BeginRegistration (const char *model);
image_s *R_RegisterSkin (const char *name);
image_t *Draw_FindPic (const char *name);
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

cvar_t		   *opengl_mode;
EXTERNC cvar_t *vid_fullscreen = nullptr;

refexport_t ImportModernOpenglApi (refimport_t inImport)
{
	refexport_t re;

	ri					   = inImport;

	re.api_version		   = API_VERSION;
	re.BeginRegistration   = R_BeginRegistration;
	re.RegisterSkin		   = R_RegisterSkin;
	re.RegisterPic		   = Draw_FindPic;

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

void R_BeginRegistration (const char *model)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

image_s *R_RegisterSkin (const char *name)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");

	return nullptr;
}

image_t *Draw_FindPic (const char *name)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
	return nullptr;
}

void R_RenderFrame (refdef_t *fd)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

void Draw_GetPicSize (int *w, int *h, char *pic)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

void Draw_Pic (int x, int y, char *name)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

void Draw_StretchPic (int x, int y, int w, int h, char *name)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

void Draw_Char (int x, int y, int num)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

void Draw_TileClear (int x, int y, int w, int h, char *name)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

void Draw_Fill (int x, int y, int w, int h, int c)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

void Draw_FadeScreen (void)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

void Draw_StretchRaw (int x, int y, int w, int h, int cols, int rows, byte *data)
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

void R_Shutdown (void)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

void R_CinematicSetPalette (const unsigned char *palette)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

void R_BeginFrame (float camera_separation)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

void SWimp_EndFrame (void)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}

void SWimp_AppActivate (qboolean active)
{
	Q2_Assert (0, "R_BeginRegistration is not implemented");
}
