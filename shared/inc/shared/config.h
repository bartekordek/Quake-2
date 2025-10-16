#ifndef __shared_config_h__
#define __shared_config_h__

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC extern
#endif

#ifdef _WIN32
#define Q2_EXPORT __declspec(dllexport)
#else
#define Q2_EXPORT
#endif

#define MAXPRINTMSG 4096

#endif	// __shared_config_h__