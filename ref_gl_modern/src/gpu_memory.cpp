#include "gl_modern/gpu_memory.hpp"
#include "gl_modern/gl_local.h"

#if defined( _WIN32 )
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dxgi1_4.h>
#pragma comment(lib, "dxgi.lib")
#endif // #if defined( _WIN32 )

#include <cstdint>

namespace Q2
{
bool query_process_gpu_memory (std::uint64_t &out_bytes)
{
	out_bytes = 0;

#if defined( _WIN32 )
	IDXGIFactory1 *factory = nullptr;
	if (FAILED (CreateDXGIFactory1 (__uuidof (IDXGIFactory1), reinterpret_cast<void **> (&factory))) || !factory)
		return false;

	std::uint64_t usage = 0;
	IDXGIAdapter1 *adapter1 = nullptr;
	for (UINT i = 0; factory->EnumAdapters1 (i, &adapter1) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		IDXGIAdapter3 *adapter3 = nullptr;
		if (SUCCEEDED (adapter1->QueryInterface (__uuidof (IDXGIAdapter3), reinterpret_cast<void **> (&adapter3))) && adapter3)
		{
			DXGI_QUERY_VIDEO_MEMORY_INFO info{};
			if (SUCCEEDED (adapter3->QueryVideoMemoryInfo (0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &info)))
				usage += info.CurrentUsage;
			adapter3->Release ();
		}
		adapter1->Release ();
		adapter1 = nullptr;
	}

	factory->Release ();
	out_bytes = usage;
	return true;
#else  // #if defined( _WIN32 )
	(void) out_bytes;
	return false;
#endif // #if defined( _WIN32 )
}
}  // namespace Q2

extern "C" void GL_GpuMemory_f (void)
{
	std::uint64_t bytes = 0;
	if (!Q2::query_process_gpu_memory (bytes))
	{
		ri.Con_Printf (PRINT_ALL, "gpu_mem: process GPU memory query unavailable on this platform\n");
		return;
	}

	ri.Con_Printf (PRINT_ALL, "GPU memory (this process): %llu bytes (%.2f MiB)\n",
				   static_cast<unsigned long long> (bytes), bytes / (1024.0 * 1024.0));
}
