#include "ref_gl/gl_legacy.hpp"
#include <cstdio>
#if defined(_WIN32)
#include "ref_gl/glw_win.h"
#endif // defined(_WIN32)

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

GL_Legacy::~GL_Legacy ()
{
	// Destructor code here
}
