#include "quake2/renderer/irenderer.hpp"
#include "quake2/renderer/gl_modern/gl_draw.hpp"
#include "quake2/renderer/gl_modern/gl_local.h"
#include "quake2/renderer/gl_modern/gl_model.h"
#include "quake2/renderer/gl_modern/gl_warp.h"

refexport_t CreateRefAPI (RendererType in_type, refimport_t rimp)
{
	refexport_t re;

	if (in_type == RendererType::OpenGLModern)
	{
		ri					   = rimp;
		re.renderer_type	   = OpenGL_Modern;

		re.api_version		   = API_VERSION;

		re.BeginRegistration   = R_BeginRegistration;
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

		re.CinematicSetPalette = R_SetPalette;
		re.BeginFrame		   = R_BeginFrame;
		re.EndFrame			   = GLimp_EndFrame;

		re.AppActivate		   = GLimp_AppActivate;

		re.create_window	   = ri.create_window;
		re.Swap_buffers		   = ri.Swap_buffers;

		Swap_Init ();
	}
	return re;
}