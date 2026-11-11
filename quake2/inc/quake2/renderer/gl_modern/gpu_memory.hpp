#pragma once

#include "shared/config.h"

#include <cstdint>

EXTERNC void GL_GpuMemory_f (void);

namespace Q2
{
// Process GPU memory currently attributed to this app (bytes). Returns false if unavailable.
bool query_process_gpu_memory (std::uint64_t &out_bytes);
}  // namespace Q2
